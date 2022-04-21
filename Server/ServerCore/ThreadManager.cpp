#include "pch.h"
#include "ThreadManager.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"

/*--------------
  ThreadManager
---------------*/

ThreadManager::ThreadManager()
{
	// Main Thread
	InitTLS();
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(function<void(void)> callback)
{
	LockGuard guard(_lock);

	_threads.push_back(thread([=]()
		{	// Launch �� TLS �ʱ�ȭ (Initializing TLS)
			InitTLS();
			callback();	// �μ��� �޾ƿ� �ݹ� �Լ� ȣ��
			DestroyTLS();
		}));
}

void ThreadManager::Join()
{
	for (thread& t : _threads)
	{
		if (t.joinable())
			t.join();
	}
	_threads.clear();
}

void ThreadManager::InitTLS()
{
	// Global and atomic
	static Atomic<uint32> SthreadId = 1;
	// L = ++S;
	LThreadId = SthreadId.fetch_add(1);
}

void ThreadManager::DestroyTLS()
{

}
