#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

//CoreGlobal Core;
class TestLock
{
	// 1개 짜리의 Lock을 사용하겠다.
	USE_LOCK;

public:
	int32 TestRead()
	{
		READ_LOCK;
		// TestPush(); 나 TestPop(); 불가능 (R -> W (X)) => CRASH
		if (_queue.empty())
			return -1;
		return _queue.front();
	}

	void TestPush()
	{
		WRITE_LOCK;
		// TestRead(); 가능 (W -> R (O))
		_queue.push(rand() % 100);
	}

	void TestPop()
	{
		WRITE_LOCK;

		if (_queue.empty() == false)
		{
			_queue.pop();
		}
	}
private:
	queue<int32> _queue;
};

TestLock testLock;

void ThreadWrite()
{
	while (true)
	{
		testLock.TestPush();
		this_thread::sleep_for(1ms);
		testLock.TestPop();
	}
}

void ThreadRead()
{
	while (true)
	{
		int32 value = testLock.TestRead();
		cout << value << endl;
		this_thread::sleep_for(1ms);
	}
}

int main()
{
	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch(ThreadWrite);
	}
	
	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch(ThreadRead);
	}

	GThreadManager->Join();

	/*
	// 일부러 Crash 낼 때
	// make crash delieberately
	CRASH("Test");

	// 조건부로 Crash 낼 때
	// make crash by condition
	int32 a = 3;
	ASSERT_CRASH(a != 3);
	*/
}