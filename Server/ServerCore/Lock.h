#pragma once
#include "Types.h"

/*-----------------
*	RW SpinLock
-----------------*/

/*-------------------------------------------
[WWWWWWWW][WWWWWWWW][RRRRRRRR][RRRRRRRR]
W : WriteFlag (Exclusive Lock Owner ThreadId) (W는 한 쓰레드만 가능하므로)
R : ReadFlag (Shared Lock Count) (R는 여러 번 가능하므로)
-------------------------------------------*/

// W -> W (O)
// W -> R (O)
// R -> W (X)
class Lock
{
	enum : uint32
	{
		// 최대로 기다려줄 tick
		ACQUIRE_TIMEOUT_TICK = 10000,
		// 최대 스핀 횟수
		MAX_SPIN_COUNT = 5000,
		// 상위 16비트 (WriteFlag)만 뽑기 위한 MASK
		WRITE_THREAD_MASK = 0xFFFF'0000,
		READ_COUNT_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000
	};

public:
	void WriteLock(const char* name);
	void WriteUnlock(const char* name);
	void ReadLock(const char* name);
	void ReadUnlock(const char* name);

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
	ReadLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.ReadLock(name); }
	~ReadLockGuard() { _lock.ReadUnlock(_name); }
private:
	Lock& _lock;
	const char* _name;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.WriteLock(name); }
	~WriteLockGuard() { _lock.WriteUnlock(_name); }
private:
	Lock& _lock;
	const char* _name;
};