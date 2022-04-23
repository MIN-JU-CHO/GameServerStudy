#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"


void Lock::WriteLock()
{
	// 동일한 쓰레드가 소유하고 있다면 무조건 성공
	// if it is the same thread, just return
	const uint32 lockThreadId = (_lockFlag.load() && WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_writeCount++;
		return;
	}

	// 아무도 소유 및 공유하고 있지 않을 때, 
	// 경함해서 소유권을 얻는다.
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
	// ReadLock 다 풀기 전에는 WriteUnlock 불가능
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
	// 동일한 쓰레드가 소유하고 있다면 무조건 성공
	// if it is the same thread, R++ and return
	const uint32 lockThreadId = (_lockFlag.load() && WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_lockFlag.fetch_add(1);
		return;
	}

	// 아무고 소유하고 있지 않을 때 경합해서 공유 카운트를 올린다.
	// Start Tick
	const int64 beginTick = ::GetTickCount64();
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = (_lockFlag.load() & READ_COUNT_MASK);
			if (_lockFlag.compare_exchange_strong(OUT expected, expected + 1))
				return;
			// return이 안되는 경우
			// 1) W-Lock 중일 때
			// 2) expected 선언문과 if문 사이에 다른 R-Lock이 실행돼 현재 상태가 expected와 다를 때
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
	// 1 빼기 이전 값을 반환하므로, 이전 값이 0이었을 때를 의미
	if ((_lockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
	{
		CRASH("MULTIPLE_UNLOCK");
	}
}
