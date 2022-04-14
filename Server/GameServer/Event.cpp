#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>

// 뮤텍스 (mutex)
mutex m;
// 공용 데이터 관리 (public data manager)
queue<int32> q;
// 핸들 넘버 (Number of handling Event)
HANDLE handle;

// 데이터 생성 (produce data in #t1 thread)
void Producer()
{
	while (true)
	{
		{
			// locking
			unique_lock<mutex> lock(m);
			// produce data
			q.push(100);
		}

		// Set Signal (파란불) After finishing producing data
		::SetEvent(handle);

		// #t1 sleeps in kernel for 100ms
		this_thread::sleep_for(10000ms);
	}
}

// 데이터 소비 (consume data in #t2 thread)
void Consumer()
{
	while (true)
	{
		// Check Signal(wake) / Non-Signal(sleep)
		::WaitForSingleObject(handle, INFINITE);
		// Auto(FALSE) : set Non-Signal automatically
		// Manual(TRUE) : need code "::ResetEvent(handle);"

		// locking
		unique_lock<mutex> lock(m);
		// if 'q' has data
		if (q.empty() == false)
		{
			// extract the front data and delete from 'q'
			int32 data = q.front();
			q.pop();
			// print it
			cout << data << endl;
		}
	}
}


int main()
{
	// 커널 오브젝트
	// Usage Count
	// Signal (파란불) / Non-Signal (빨간불) << bool 초기:bInitialState
	// Auto / Manual << bool bManualReset
	// Number of Event (Kernel Object)
	handle = ::CreateEvent(NULL/*보안속성(for Security)*/,
		FALSE/*bManualReset*/, FALSE/*bInitialState*/, NULL);

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(handle);
}