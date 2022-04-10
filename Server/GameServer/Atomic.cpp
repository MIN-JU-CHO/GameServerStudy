#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>


// DB
// A라는 유저 인벤어서 집행검 빼고 --> transaction 중간에 연결 끊기면 데이터 손실
// B라는 유저 인벤어 집행검 추가
// 
// atomic : All-Or-Nothing
atomic<int32> sum = 0;

void Add() {
	for (int32 i = 0; i < 100'0000; i++) {
		sum.fetch_add(1);
		//sum++;

		/* sum++; 연산 과정(어셈블리)
		int32 eax = sum;
		// eax = 0
		eax = eax + 1;
		// eax = 1
		sum = eax;
		// sum = 1 */
	}
}

void Sub() {
	for (int32 i = 0; i < 100'0000; i++) {
		sum.fetch_add(-1);
		//sum--;

		/* sum++; 연산 과정(어셈블리)
		int32 eax = sum;
		// eax = 0
		eax = eax - 1;
		// eax = -1
		sum = eax;
		// sum = -1 */
	}
}

int main()
{
	Add();
	Sub();
	cout << sum << endl;

	std::thread t1(Add);
	std::thread t2(Sub);
	t1.join();
	t2.join();
	cout << sum << endl;
}