#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

// [1][2][3] //-> ���� �ø��� �������� Double Free Crash
// [1][2][3][][][] 

vector<int32> v;
// atomic<vector<int32>> a; : �Ұ���

// Mutual Exclusive (��ȣ��Ÿ��)
mutex m;

// RAII (Resource Acquisition Is Initialization)
template<typename T>
class LockGuard {
public:
	LockGuard(T& m) {
		_mutex = &m;
		_mutex->lock();
	}

	~LockGuard() {
		_mutex->unlock();
	}
private:
	T* _mutex;
};

void Push() {
	for (int32 i = 0; i < 10000; i++)
	{
		// �ڹ��� ��ױ� (��� ��׳Ŀ� ���� ��״� Ƚ���� ���� �޶���)
		// 
		// 0. ������ ���
		//m.lock();
		//m.lock();	// (���� �ٸ� �Լ�����) ����� ȣ�� �Ұ���
		// 
		// 1. ������ ����
		// ������ �Լ����� lock�ϰ�, ��� ������ �Ҹ�� ���ÿ� �ڵ����� unlock
		//LockGuard<std::mutex> lockGuard(m);	// mutex ���� ����ֱ�
		// 
		// 2. �⺻ ���� ������
		std::lock_guard<std::mutex> lockGuard(m);

		// 3. ����� lock ������ ������
		//std::unique_lock<std::mutex> uniqueLock(m, std::defer_lock);
		//uniqueLock.lock();

		v.push_back(i);

		if (i == 5000) {
			//m.unlock();
			break;
		}

		// �ڹ��� Ǯ��
		//m.unlock();
		//m.unlock();	// ����� ȣ�� �Ұ���
	}
}

int main()
{
	//v.reserve(20000);	// ���� ����� �÷�����-> �ߺ� Data write: Data �ս�

	std::thread t1(Push);
	std::thread t2(Push);

	t1.join();
	t2.join();

	cout << v.size() << endl;
}