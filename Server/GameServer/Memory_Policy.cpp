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
	// ------------- ���뼱 -------------
	//std::atomic_thread_fence(memory_order::memory_order_release);
}

void Consumer()
{
	// ------------- ���뼱 -------------
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

	// ���� flag ���� prev�� �ְ�, flag ���� ����
	{
		// �� ���� (atomic exchange)
		bool prev = flag.exchange(true);

		//bool prev = flag;
		//flag = true;
	}

	// CAS (Compare-And-Swap) ���Ǻ� ����
	{
		bool expected = false;
		bool desired = true;
		flag.compare_exchange_strong(expected, desired);
		// Spurious Failure
		// pseudo code
		if (flag == expected)
		{
			// �ٸ� �������� interruption�� �޾� �߰��� ������ �� ����
			// other thread can interrupt, so it can be failed
			// -> weak
			// if (���� ��Ȳ)
			//		return false;
			// ������ ������ �ٽ� �ݺ�
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

	// Memory Model (��å)
	// 1) Suquentially Consistent (seq_cst)
	// 2) Aquire-Release (aquire, release, acq_rel)
	// 3) Relaxed (relaxed)

	ready = false;
	value = 0;
	thread t1(Producer);
	thread t2(Consumer);
	t1.join();
	t2.join();

	// 1) seq_cst (���� ���� = �����Ϸ� ����ȭ ���� ���� = ������)
	// ���ü� ���� �ٷ� �ذ�! �ڵ� ���ġ �ٷ� �ذ�!
	// 
	// 2) acquire-release
	// release ��� ������ �޸� ��ɵ���, �ش� ��� ���ķ� ���ġ �Ǵ� ���� ����
	// �׸��� acquire �� ���� ������ �д� thread�� �ִٸ�
	// release ������ ��ɵ��� -> acquire �ϴ� ������ ���� ���� (���ü� ����)
	// 
	// 3) relaxed (�����Ӵ� = �����Ϸ� ����ȭ ���� ���� = ���������� ����)
	// �ڵ� ���ġ�� �ڴ�� ����, ���ü� �ذ� NO
	// ���� �⺻ ���� (���� ��ü�� ���� ���� ���� ������ ����)

	// Intel, AMD�� ��� �ִ��� ������ �ϰ����� �����ؼ�
	// seq_cst�� �ᵵ ���ٸ� ���ϰ� ����
	// ARM�� ��� �� ���̰� �ִ�.
}
