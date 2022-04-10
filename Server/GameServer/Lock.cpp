#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

// [1][2][3] //-> 공간 늘리는 과정에서 Double Free Crash
// [1][2][3][][][] 

vector<int32> v;
// atomic<vector<int32>> a; : 불가능

// Mutual Exclusive (상호배타적)
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
		// 자물쇠 잠그기 (어디서 잠그냐에 따라 잠그는 횟수와 범위 달라짐)
		// 
		// 0. 원시적 잠금
		//m.lock();
		//m.lock();	// (내부 다른 함수에서) 재귀적 호출 불가능
		// 
		// 1. 락가드 구현
		// 생성자 함수에서 lock하고, 어디서 끝나든 소멸과 동시에 자동으로 unlock
		//LockGuard<std::mutex> lockGuard(m);	// mutex 변수 집어넣기
		// 
		// 2. 기본 제공 락가드
		std::lock_guard<std::mutex> lockGuard(m);

		// 3. 명시적 lock 가능한 락가드
		//std::unique_lock<std::mutex> uniqueLock(m, std::defer_lock);
		//uniqueLock.lock();

		v.push_back(i);

		if (i == 5000) {
			//m.unlock();
			break;
		}

		// 자물쇠 풀기
		//m.unlock();
		//m.unlock();	// 재귀적 호출 불가능
	}
}

int main()
{
	//v.reserve(20000);	// 공간 충분히 늘려놔도-> 중복 Data write: Data 손실

	std::thread t1(Push);
	std::thread t2(Push);

	t1.join();
	t2.join();

	cout << v.size() << endl;
}