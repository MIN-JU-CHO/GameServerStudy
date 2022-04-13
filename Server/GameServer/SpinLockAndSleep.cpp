#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

class SpinLock {
public:
	void lock()
	{
		// CAS (Compare-And-Swap)
		bool expected = false;
		bool desired = true;

		// CAS 의사 코드
		/*
		if (_locked == expected) {
			_locked = desired;

			return true;
		}
		else {
			expected = _locked
			return false;
		}
		*/
		while (_locked.compare_exchange_strong(expected, desired) == false)
		{
			expected = false;

			// System Call 포함
			// 
			// 일정 시간동안 재 스케줄링 X
			//this_thread::sleep_for(std::chrono::milliseconds(100));		
			this_thread::sleep_for(0ms);
			//this_thread::yield();
			// 받은 time slice 양보하고, 커널모드로 돌아가 스케줄링에 맡김
			// 언제든지 스케줄링 될 수 있음. sleep (0초) 와 같음
		}
	}

	void unlock()
	{
		//_locked = false;
		_locked.store(false);
	}

private:
	atomic<bool> _locked = false;
};

int32 sum = 0;
mutex m;
SpinLock spinLock;

void Add() {
	for (int32 i = 0; i < 10'0000; i++) {
		lock_guard<SpinLock> guard(spinLock);
		sum++;
	}
}

void Sub() {
	for (int32 i = 0; i < 10'0000; i++) {
		lock_guard<SpinLock> guard(spinLock);
		sum--;
	}
}

int main()
{
	// volatile은 컴파일러 최적화 중지 요청
	volatile bool flag = true;

	thread t1(Add);
	thread t2(Sub);

	t1.join();
	t2.join();

	cout << sum << endl;
}