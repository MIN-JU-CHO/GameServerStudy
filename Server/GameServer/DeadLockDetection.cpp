#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include "PlayerManager.h"
#include "AccountManager2.h"

/*
//CoreGlobal Core;
class TestLock
{
	// 1�� ¥���� Lock�� ����ϰڴ�.
	USE_LOCK;

public:
	int32 TestRead()
	{
		READ_LOCK;
		// TestPush(); �� TestPop(); �Ұ��� (R -> W (X)) => CRASH
		if (_queue.empty())
			return -1;
		return _queue.front();
	}

	void TestPush()
	{
		WRITE_LOCK;
		// TestRead(); ���� (W -> R (O))
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
*/
int main()
{
	GThreadManager->Launch([=]
		{
			while (true)
			{
				cout << "PlayerThenAccount" << endl;
				GPlayerManager.PlayerThenAccount();
				this_thread::sleep_for(100ms);
			}
		});

	GThreadManager->Launch([=]
		{
			while (true)
			{
				cout << "AccountThenPlayer" << endl;
				GAccountManager.AccountThenPlayer();
				this_thread::sleep_for(100ms);
			}
		});

	/*for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch(ThreadWrite);
	}

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch(ThreadRead);
	}

	GThreadManager->Join();
	*/

	/*
	// �Ϻη� Crash �� ��
	// make crash delieberately
	CRASH("Test");

	// ���Ǻη� Crash �� ��
	// make crash by condition
	int32 a = 3;
	ASSERT_CRASH(a != 3);
	*/

}