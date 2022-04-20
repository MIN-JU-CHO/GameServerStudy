#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>

//atomic<bool> flag;
atomic<bool> ready;
int32 value;

void Producer()
{
	//value2 = 10;
	value = 10;
	//value2 = 10;

	ready.store(true, memory_order::memory_order_release);
	// ------------- 절취선 -------------
	//std::atomic_thread_fence(memory_order::memory_order_release);
}

void Consumer()
{
	// ------------- 절취선 -------------
	while (ready.load(memory_order::memory_order_acquire) == false)
		;

	cout << value << endl;
}

int main()
{
	/*flag = false;

	//flag = true;
	flag.store(true, memory_order::memory_order_seq_cst);

	//bool val = flag;
	bool val = flag.load(memory_order::memory_order_seq_cst);

	// 이전 flag 값을 prev에 넣고, flag 값을 수정
	{
		// 한 번에 (atomic exchange)
		bool prev = flag.exchange(true);

		//bool prev = flag;
		//flag = true;
	}

	// CAS (Compare-And-Swap) 조건부 수정
	{
		bool expected = false;
		bool desired = true;
		flag.compare_exchange_strong(expected, desired);
		// Spurious Failure
		// pseudo code
		if (flag == expected)
		{
			// 다른 쓰레드의 interruption을 받아 중간에 실패할 수 있음
			// other thread can interrupt, so it can be failed
			// -> weak
			// if (묘한 상황)
			//		return false;
			// 성공할 때까지 다시 반복
			// repeat this exchanging until success
			// -> strong

			//expected = flag;
			flag = desired;
			return true;
		}
		else
		{
			expected = flag;
			return false;
		}


		while (true)
		{
			bool expected = false;
			bool desired = true;
			flag.compare_exchange_weak(expected, desired);
		}
	}
	*/

	// Memory Model (정책)
	// 1) Suquentially Consistent (seq_cst)
	// 2) Aquire-Release (aquire, release, acq_rel)
	// 3) Relaxed (relaxed)

	ready = false;
	value = 0;
	thread t1(Producer);
	thread t2(Consumer);
	t1.join();
	t2.join();

	// 1) seq_cst (가장 엄격 = 컴파일러 최적화 여지 적음 = 직관적)
	// 가시성 문제 바로 해결! 코드 재배치 바로 해결!
	// 
	// 2) acquire-release
	// release 명령 이전의 메모리 명령들이, 해당 명령 이후로 재배치 되는 것을 금지
	// 그리고 acquire 로 같은 변수를 읽는 thread가 있다면
	// release 이전의 명령들이 -> acquire 하는 순간에 관찰 가능 (가시성 보장)
	// 
	// 3) relaxed (자유롭다 = 컴파일러 최적화 여지 많음 = 직관적이지 않음)
	// 코드 재배치도 멋대로 가능, 가시성 해결 NO
	// 가장 기본 조건 (동일 객체에 대한 동일 관전 순서만 보장)

	// Intel, AMD의 경우 애당초 순차적 일관성을 보장해서
	// seq_cst를 써도 별다른 부하가 없음
	// ARM의 경우 꽤 차이가 있다.
}
