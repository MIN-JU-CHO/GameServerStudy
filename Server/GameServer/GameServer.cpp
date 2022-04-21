#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

CoreGlobal Core;

void ThreadMain()
{
	while (true)
	{
		cout << "Hello ! I am thread ... " << LThreadId << endl;
		this_thread::sleep_for(1s);
	}
}

int main()
{
	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch(ThreadMain);
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