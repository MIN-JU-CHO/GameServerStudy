#pragma once
#include "Types.h"

/*-----------------
*	RW SpinLock
-----------------*/

/*-------------------------------------------
[WWWWWWWW][WWWWWWWW][RRRRRRRR][RRRRRRRR]
W : WriteFlag (Exclusive Lock Owner ThreadId) (W�� �� �����常 �����ϹǷ�)
R : ReadFlag (Shared Lock Count) (R�� ���� �� �����ϹǷ�)
-------------------------------------------*/

// W -> W (O)
// W -> R (O)
// R -> W (X)
class Lock
{
	enum : uint32
	{
		// �ִ�� ��ٷ��� tick
		ACQUIRE_TIMEOUT_TICK = 10000,
		// �ִ� ���� Ƚ��
		MAX_SPIN_COUNT = 5000,
		// ���� 16��Ʈ (WriteFlag)�� �̱� ���� MASK
		WRITE_THREAD_MASK = 0xFFFF'0000,
		READ_COUNT_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000
	};

public:
	void WriteLock();
	void WriteUnlock();
	void ReadLock();
	void ReadUnlock();

private:
	Atomic<uint32> _lockFlag = EMPTY_FLAG;
	uint16 _writeCount = 0;
};

/*----------------------
		LockGuards
----------------------*/
// RAII
class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock) : _lock(lock) { _lock.ReadLock(); }
	~ReadLockGuard() { _lock.ReadUnlock(); }
private:
	Lock& _lock;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock) : _lock(lock) { _lock.WriteLock(); }
	~WriteLockGuard() { _lock.WriteUnlock(); }
private:
	Lock& _lock;
};