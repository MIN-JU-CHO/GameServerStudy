#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>

// Classic Method
//__declspec(thread) int32 value;

// [ TLS ]
// 전역변수가 아닌 thread 고유의 변수
// It's not just a global var. Each threads has own var. 
thread_local int32 LthreadId = 0;
//thread_local queue<int32> q;

void ThreadMain(int32 threadId)
{
	LthreadId = threadId;

	while (true)
	{
		cout << "Hi! I am Thread " << LthreadId << endl;
		this_thread::sleep_for(1s);
	}
}

int main()
{
	vector<thread> threads;

	for (int32 i = 0; i < 10; i++)
	{
		int32 threadId = i + 1;
		threads.push_back(thread(ThreadMain, threadId));
	}

	for (thread& t : threads)
	{
		t.join();
	}
}
