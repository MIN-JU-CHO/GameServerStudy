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
	// ���� (Synchronous) ����
	//int64 sum = Calculate();
	//cout << sum << endl;

	/*
	// �񵿱� (Asynchronous) ����
	thread t(Calculate);

	// TODO

	t.join();
	*/

	// ����: ���� ������ (��������) �ʿ� -> requires lock
	// simple function�� heavy��(����, �Ҹ� �ʿ�) thread ������� �����ؾ��ϳ�?

	// Solution: Asynchronous
	// std::future
	{
		/*
		// 1) deferred -> lazy evaluation �����ؼ� �����ϼ���. (���� ȣ��)
		// 2) async -> ������ Thread�� ���� �����ϼ���. (Parallel)
		// 3) deferred | async -> �� �� �˾Ƽ� ����ּ���.

		// ������ �̷��� ������� ����ٰž�
		//std::future<int64> future = std::async(std::launch::async, Calculate);

		// TODO

		std::future_status status = future.wait_for(1ms);
		if (status == future_status::ready)
		{
			// async �ϰ� �Ϸ� ����
		}
		// ������� �������� �ʿ��ϴ�!
		future.wait(); // == WaitFor...(INFINITE)


		//�ȱ�ٷ��� �˾Ƽ� wait & get
		//int64 sum = future.get(); // ������� �������� �ʿ��ϴ�!
		*/
	}

	// ��ü Ȱ�� ����
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
		// �̷�(std::future)�� ������� ��ȯ���ٰŶ� ���(std::promise)
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
	// mutex, condition_variable���� ���� �ʰ� �ܼ��� �ֵ��� ó���� �� �ִ�
	// just for calling once
	// Ư����, �� �� �߻��ϴ� �̺�Ʈ�� �����ϴ�.
	// 1) async
	// ���ϴ� �Լ��� �񵿱������� ���� (Asynchronous Calling)
	// 2) promise
	// ������� promise�� ���� future�� �޾��� (result comes from promise->future)
	// 3) packaged_task
	// ���ϴ� �Լ��� ���� ����� packaged_task�� ���� future�� �޾��� (reults come from task->future)
}