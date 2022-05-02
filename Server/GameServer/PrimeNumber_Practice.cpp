#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include <vector>
#include <thread>

// �Ҽ� ���ϱ�
bool isPrime(int number)
{
	if (number <= 1)
		return false;
	if (number == 2 || number == 3)
		return true;

	for (int i = 2; i < number; i++)
	{
		if ((number % i) == 0)
			return false;
	}

	return true;
}

// [start ~ end]
int CountPrime(int start, int end)
{
	int count = 0;

	for (int number = start; number < end; number++)
	{
		if (isPrime(number))
			count++;
	}

	return count;
}

// 1�� �ڱ� �ڽ����θ� ������ �װ��� �Ҽ���� ��

int main()
{
	const int MAX_NUMBER = 100'0000;
	// 1~MAX_NUMBER������ �Ҽ� ����
	// 1000 = 168
	// 1'0000 = 1229
	// 100'0000 = 78498
	vector<thread> threads;

	int coreCount = thread::hardware_concurrency();
	int jobCount = (MAX_NUMBER / coreCount) + 1;

	atomic<int> primeCount = 0;
	for (int i = 0; i < coreCount; i++)
	{
		int start = (i * jobCount) + 1;
		int end = min(MAX_NUMBER, ((i + 1) * jobCount));

		threads.push_back(thread([start, end, &primeCount]()
			{
				primeCount += CountPrime(start, end);
			}));
	}

	for (thread& t : threads)
		t.join();
	cout << primeCount << endl;
}