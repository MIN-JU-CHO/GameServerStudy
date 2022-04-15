#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>

//int64 result;

int64 Calculate() 
{
	int64 sum = 0;

	for (int32 i = 0; i < 100'000; i++) 
	{
		sum += i;
	}

	//result = sum;

	return sum;
}

int main()
{
	// 동기 (Synchronous) 실행
	//int64 sum = Calculate();
	//cout << sum << endl;

	/*
	// 비동기 (Asynchronous) 실행
	thread t(Calculate);

	// TODO

	t.join();
	*/
	
	// 단점: 공용 데이터 (전역변수) 필요 -> requires lock
	// simple function을 heavy한(생성, 소멸 필요) thread 방식으로 구현해야하나?
	
	// Solution: Asynchronous
	// std::future
	{
		// 1) deferred -> lazy evaluation 지연해서 실행하세요. (나중 호출)
		// 2) async -> 별도의 Thread를 만들어서 실행하세요. (Parallel)
		// 3) deferred | async -> 둘 중 알아서 골라주세요.
		std::future<int64> future = std::async(std::launch::async, Calculate);

		// TODO
		std::future_status status = future.wait_for(1ms);
		if (status == future_status::ready) 
		{
			// 완료 상태
		}
		// 결과물이 이제서야 필요하다!
		future.wait(); // == WaitFor...(INFINITE)
		
		
		//안기다려도 알아서 wait & get
		int64 sum = future.get(); // 결과물이 이제서야 필요하다!
	}
}