#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

// ���ؽ� (mutex)
mutex m;
// ���� ������ ���� (public data manager)
queue<int32> q;

// ����) CV�� User-Level Object (Kernel Object (X))
// ���� ������ ���α׷� �������� �ۿ� (It can be used in only one program)
// MMO RPG Game�� �ٸ� ���α׷��� ������ �� ����. (doesn't need to interact with other programs)
// from mutex
condition_variable cv;
/*
* ���ȭ�� ���� (more common)
* #include <condition_variable>
* condition_variable_any
*/
// ������ ���� (produce data in #t1 thread)
void Producer()
{
	while (true)
	{
		// 1) Lock�� ���
		// 2) ���� ���� ���� ����
		// 3) Lock�� Ǯ��
		// 4) ���Ǻ��� ���� �ٸ� �����忡�� ����

		{
			// 1) locking
			unique_lock<mutex> lock(m);
			// 2) produce data
			q.push(100);
		}	// 3) unlocking

		// Set Signal (�Ķ���) After finishing producing data
		//::SetEvent(handle);

		// 4) Wait���� Thread�� ������ �� 1���� Wake�Ѵ�.
		cv.notify_one();

		// #t1 sleeps in kernel for 100ms
		//this_thread::sleep_for(10000ms);
	}
}

// ������ �Һ� (consume data in #t2 thread)
void Consumer()
{
	while (true)
	{
		// Check Signal(wake) / Non-Signal(sleep)
		//::WaitForSingleObject(handle, INFINITE);

		// locking
		unique_lock<mutex> lock(m);
		cv.wait(lock, []() {return q.empty() == false; });
		// 1) Lock�� ���
		// 2) ���� Ȯ��
		// - ����O ==> ���� ���ͼ� �̾ �ڵ带 ����
		// - ����X ==> Lock�� Ǯ���ְ�(requires unique_lock) ��� ����

		// �׷��� notify_one�� ������ �׻� ���ǽ��� �����ϴ°� �ƴұ�?
		// Spurious Wakeup (��¥ ���?)
		// notify_one�� �� lock�� ��� �ִ� ���� �ƴϱ� ����

		// if 'q' has data
		//if (q.empty() == false) 
		{
			// extract the front data and delete from 'q'
			int32 data = q.front();
			q.pop();
			// print it
			cout << q.size() << endl;
		}
	}
}


int main()
{

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

}