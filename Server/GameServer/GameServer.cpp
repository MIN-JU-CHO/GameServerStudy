#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include "RefCounting.h"
#include "Memory.h"

class Knight
{
public:
	Knight()
	{
		cout << "Knight()" << endl;
	}

	Knight(int32 hp) : _hp(hp)
	{
		cout << "Knight(hp)" << endl;
	}

	~Knight()
	{
		cout << "~Knight()" << endl;
	}

	// new operator overloading (Global)
	// 클래스 안에서 재정의하면 자동으로 static이 된다
	// 메모리 할당/해제를 특수하게 관리하고 싶을 때 사용
	/*
	static void* operator new(size_t size)
	{
		cout << "Knight new! " << size << endl;
		void* ptr = ::malloc(size);
		return ptr;
	}

	static void operator delete(void* ptr)
	{
		cout << "Knight delete!" << endl;
		::free(ptr);
	}
	*/
	int32 _hp = 100;
	int32 _mp = 10;
};

//// new operator overloading (Global)
//void* operator new(size_t size) 
//{
//	cout << "new! " << size << endl;
//	void* ptr = ::malloc(size);
//	return ptr;
//}
//
//void operator delete(void* ptr)
//{
//	cout << "delete!" << endl;
//	::free(ptr);
//}
//
//// new[] operator overloading (Global)
//void* operator new[](size_t size) 
//{
//	cout << "new[]! " << size << endl;
//	void* ptr = ::malloc(size);
//	return ptr;
//}
//
//void operator delete[](void* ptr)
//{
//	cout << "delete[]!" << endl;
//	::free(ptr);
//}

int main()
{

	//Knight* knight = new Knight();
	Knight* knight = xnew<Knight>(100);

	//delete knight;
	xdelete(knight);
}