#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>

// ���ؽ� (mutex)
mutex m;
// ���� ������ ���� (public data manager)
queue<int32> q;
// �ڵ� �ѹ� (Number of handling Event)
HANDLE handle;

// ������ ���� (produce data in #t1 thread)
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

		// Set Signal (�Ķ���) After finishing producing data
		::SetEvent(handle);

		// #t1 sleeps in kernel for 100ms
		this_thread::sleep_for(10000ms);
	}
}

// ������ �Һ� (consume data in #t2 thread)
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
	// Ŀ�� ������Ʈ
	// Usage Count
	// Signal (�Ķ���) / Non-Signal (������) << bool �ʱ�:bInitialState
	// Auto / Manual << bool bManualReset
	// Number of Event (Kernel Object)
	handle = ::CreateEvent(NULL/*���ȼӼ�(for Security)*/,
		FALSE/*bManualReset*/, FALSE/*bInitialState*/, NULL);

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(handle);
}