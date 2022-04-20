#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>

// Classic Method
//__declspec(thread) int32 value;

thread_local int32 LthreadId = 0;

void ThreadMain(int32 threadId)
{
	LthreadId = threadId;

	while (true)
	{
		cout << "Hi! I am Thread " << LthreadId << endl;
	}
}

int main()
{

}
