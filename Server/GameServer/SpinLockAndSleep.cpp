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

		// CAS �ǻ� �ڵ�
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

			// System Call ����
			// 
			// ���� �ð����� �� �����ٸ� X
			//this_thread::sleep_for(std::chrono::milliseconds(100));		
			this_thread::sleep_for(0ms);
			//this_thread::yield();
			// ���� time slice �纸�ϰ�, Ŀ�θ��� ���ư� �����ٸ��� �ñ�
			// �������� �����ٸ� �� �� ����. sleep (0��) �� ����
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
	// volatile�� �����Ϸ� ����ȭ ���� ��û
	volatile bool flag = true;

	thread t1(Add);
	thread t2(Sub);

	t1.join();
	t2.join();

	cout << sum << endl;
}