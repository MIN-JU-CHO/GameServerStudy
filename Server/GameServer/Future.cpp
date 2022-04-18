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

void PromiseWorker(std::promise<string>&& promise)
{
	promise.set_value("Secret Message");
}

void TaskWorker(std::packaged_task<int64(void)>&& task)
{
	task();
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
		/*
		// 1) deferred -> lazy evaluation 지연해서 실행하세요. (나중 호출)
		// 2) async -> 별도의 Thread를 만들어서 실행하세요. (Parallel)
		// 3) deferred | async -> 둘 중 알아서 골라주세요.

		// 언젠가 미래에 결과물을 뱉어줄거야
		//std::future<int64> future = std::async(std::launch::async, Calculate);

		// TODO

		std::future_status status = future.wait_for(1ms);
		if (status == future_status::ready)
		{
			// async 일감 완료 상태
		}
		// 결과물이 이제서야 필요하다!
		future.wait(); // == WaitFor...(INFINITE)


		//안기다려도 알아서 wait & get
		//int64 sum = future.get(); // 결과물이 이제서야 필요하다!
		*/
	}

	// 객체 활용 예제
	/*
	class Knight
	{
	public:
		int64 GetHP() { return 100; }
	};
	Knight knight;	// knight.GetHP();
	std::future<int64> future = std::async(std::launch::async, &Knight::GetHP, knight);
	*/

	// std::promise
	{
		// 미래(std::future)에 결과물을 반환해줄거라 약속(std::promise)
		std::promise<string> promise;
		std::future<string> future = promise.get_future();

		thread t(PromiseWorker, std::move(promise));

		string message = future.get();
		cout << message << endl;

		t.join();
	}

	// std::packaged_task
	{
		std::packaged_task<int64(void)> task(Calculate);
		std::future<int64> future = task.get_future();

		std::thread t(TaskWorker, std::move(task));

		int64 sum = future.get();
		cout << sum << endl;
		t.join();
	}

	// Conclusion)
	// mutex, condition_variable까지 가지 않고 단순한 애들을 처리할 수 있는
	// just for calling once
	// 특히나, 한 번 발생하는 이벤트에 유용하다.
	// 1) async
	// 원하는 함수를 비동기적으로 실행 (Asynchronous Calling)
	// 2) promise
	// 결과물을 promise를 통해 future로 받아줌 (result comes from promise->future)
	// 3) packaged_task
	// 원하는 함수의 실행 결과를 packaged_task를 통해 future로 받아줌 (reults come from task->future)
}