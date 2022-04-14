#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include "AccountManager.h"
#include "UserManager.h"

void Func1() {
	for (int32 i = 0; i < 100; i++) {
		UserManager::Instance()->ProcessSave();
	}
}

void Func2() {
	for (int32 i = 0; i < 100; i++) {
		AccountManager::Instance()->ProcessLogin();
	}
}

int main()
{
	std::thread t1(Func1);
	std::thread t2(Func2);

	t1.join();
	t2.join();

	cout << "Jobs Done" << endl;

	// ����
	mutex m1;
	mutex m2;

	std::lock(m1, m2);	//m1.lock(); m2.lock();

	// adopt_lock : �̹� lock�� ���´ϱ�, ���߿� �Ҹ� �� �� Ǯ���ֱ⸸ ��
	lock_guard<mutex> g1(m1, std::adopt_lock);
	lock_guard<mutex> g1(m2, std::adopt_lock);
}