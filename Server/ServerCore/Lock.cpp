#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"


void Lock::WriteLock()
{
	// ������ �����尡 �����ϰ� �ִٸ� ������ ����
	// if it is the same thread, just return
	const uint32 lockThreadId = (_lockFlag.load() && WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_writeCount++;
		return;
	}

	// �ƹ��� ���� �� �����ϰ� ���� ���� ��, 
	// �����ؼ� �������� ��´�.
	/*// pseudo code
	if (_lockFlag == EMPTY_FLAG)
	{
		const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);
		_lockFlag = desired;
	}*/
	// Start Tick
	const int64 beginTick = ::GetTickCount64();
	// create desired value outside of condition
	const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = EMPTY_FLAG;
			if (_lockFlag.compare_exchange_strong(OUT expected, desired))
			{
				_writeCount++;
				return;
			}
		}

		// If END Tick has exceed 10000, crash it
		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		// If spinCount has exceed 5000, Yield
		this_thread::yield();
	}
}

void Lock::WriteUnlock()
{
	// ReadLock �� Ǯ�� ������ WriteUnlock �Ұ���
	// Can't W-Unlock before unlocking all R-Lock
	if ((_lockFlag.load() & READ_COUNT_MASK) != 0)
	{
		CRASH("INVALID_UNLOCK_ORDER");
	}

	const int32 lockCount = --_writeCount;
	// If no thread has W-Lock of it,
	if (lockCount == 0)
	{	// Reset the lockFlag
		_lockFlag.store(EMPTY_FLAG);
	}
}

void Lock::ReadLock()
{
	// ������ �����尡 �����ϰ� �ִٸ� ������ ����
	// if it is the same thread, R++ and return
	const uint32 lockThreadId = (_lockFlag.load() && WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_lockFlag.fetch_add(1);
		return;
	}

	// �ƹ��� �����ϰ� ���� ���� �� �����ؼ� ���� ī��Ʈ�� �ø���.
	// Start Tick
	const int64 beginTick = ::GetTickCount64();
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = (_lockFlag.load() & READ_COUNT_MASK);
			if (_lockFlag.compare_exchange_strong(OUT expected, expected + 1))
				return;
			// return�� �ȵǴ� ���
			// 1) W-Lock ���� ��
			// 2) expected ���𹮰� if�� ���̿� �ٸ� R-Lock�� ����� ���� ���°� expected�� �ٸ� ��
		}

		// If END Tick has exceed 10000, crash it
		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		// If spinCount has exceed 5000, Yield
		this_thread::yield();
	}
}

void Lock::ReadUnlock()
{
	// 1 ���� ���� ���� ��ȯ�ϹǷ�, ���� ���� 0�̾��� ���� �ǹ�
	if ((_lockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
	{
		CRASH("MULTIPLE_UNLOCK");
	}
}
