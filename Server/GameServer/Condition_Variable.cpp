#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

// 뮤텍스 (mutex)
mutex m;
// 공용 데이터 관리 (public data manager)
queue<int32> q;

// 참고) CV는 User-Level Object (Kernel Object (X))
// 따라서 동일한 프로그램 내에서만 작용 (It can be used in only one program)
// MMO RPG Game은 다른 프로그램과 교류할 일 없음. (doesn't need to interact with other programs)
// from mutex
condition_variable cv;
/*
* 상용화된 버전 (more common)
* #include <condition_variable>
* condition_variable_any
*/
// 데이터 생성 (produce data in #t1 thread)
void Producer()
{
	while (true)
	{
		// 1) Lock을 잡고
		// 2) 공유 변수 값을 수정
		// 3) Lock을 풀고
		// 4) 조건변수 통해 다른 쓰레드에게 통지

		{
			// 1) locking
			unique_lock<mutex> lock(m);
			// 2) produce data
			q.push(100);
		}	// 3) unlocking

		// Set Signal (파란불) After finishing producing data
		//::SetEvent(handle);

		// 4) Wait중인 Thread가 있으면 딱 1개를 Wake한다.
		cv.notify_one();

		// #t1 sleeps in kernel for 100ms
		//this_thread::sleep_for(10000ms);
	}
}

// 데이터 소비 (consume data in #t2 thread)
void Consumer()
{
	while (true)
	{
		// Check Signal(wake) / Non-Signal(sleep)
		//::WaitForSingleObject(handle, INFINITE);

		// locking
		unique_lock<mutex> lock(m);
		cv.wait(lock, []() {return q.empty() == false; });
		// 1) Lock을 잡고
		// 2) 조건 확인
		// - 만족O ==> 빠져 나와서 이어서 코드를 진행
		// - 만족X ==> Lock을 풀어주고(requires unique_lock) 대기 상태

		// 그런데 notify_one을 했으면 항상 조건식을 만족하는거 아닐까?
		// Spurious Wakeup (가짜 기상?)
		// notify_one할 때 lock을 잡고 있는 것이 아니기 때문

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