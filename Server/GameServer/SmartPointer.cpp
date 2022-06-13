#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include "RefCounting.h"

using KnightRef = TSharedPtr<class Knight>;
//using InventoryRef = TSharedPtr<class Inventory>;

class Knight	// : public RefCountable
{
public:
	Knight()
	{
		cout << "Knight()" << endl;
	}

	~Knight()
	{
		cout << "~Knight()" << endl;
	}

	/*
	void SetTarget(KnightRef target)
	{
		_target = target;
	}
	*/

	//KnightRef _target = nullptr;
	//InventoryRef _inventory = nullptr;
};

/*
class Inventory : public RefCountable
{
public:
	Inventory(KnightRef knight) : _knight(**knight) {

	}
	Knight& _knight;	// �������̹Ƿ�, RefCount�� �ø��� �ʴ´�.
	// KnightRef �� ������ Cycle �� �����. (�޸� �������� �ʴ� Memory Leak ���� �߻�)
};
*/

int main()
{
	// SharedPtr �Ѱ�
	// 1) �̹� ������� Ŭ����(�ܺ� ���̺귯�� ��) ������� ��� �Ұ�
	// ex) �ܺ� Ŭ���� : public RefCountable <--��� �ֱ� �Ұ���
	// 2) ��ȯ (Cycle) ����

	KnightRef k1(new Knight());
	//k1->ReleaseRef();
	//k1->_inventory = new Inventory(k1);

	// 'Shared_ptr' is from std library and it makes the same problem (Cycle)
	shared_ptr<Knight> spr;

	// 'Unique_ptr' prevents copying
	unique_ptr<Knight> k2 = make_unique<Knight>();
	unique_ptr<Knight> k3 = std::move(k2);	// ����� �ȵ�����, ������ ���ְ� �̵��� ����

	// shared_ptr doesn't need any Inheritance
	// (�Ѱ迴��) ��� ���ʿ�. ���� ���� �ٸ�
	// [Knight][RefCountingBlock] ���� �����ڵ�

	// [T*][RefCountBlock*] shared_ptr�� ������ �� ����
	shared_ptr<Knight> spr(new Knight());
	// make_shared ������: [Knight | RefCountingBlock(_Uses, _Weak)] RefCounting�� shared_ptr�� weak_ptr ��� ����
	shared_ptr<Knight> spr = make_shared<Knight>();

	// [T*][RefCountBlock*] ���� ����
	shared_ptr<Knight> spr2 = spr;



	// weak_ptr�� shared_ptr �޾Ƽ� ���� ���ɤ�
	// weak_ptr�� shared_ptr���� �����ͷ� ����� �� ����, �����ϴ��� �ƴ��� ���� üũ�ؾ���

	weak_ptr<Knight> wpr = spr;

	// Check method #1
	bool expired = wpr.expired();
	// Check method #2 (convert to)shared_ptr�� ��ȯ
	shared_ptr<Knight> spr3 = wpr.lock();	// �������� ���� ���� spr3 == nullptr
	// ���� weak_ptr�� ���翩�θ� Ȯ�� �߽��ϴ� ����¥�� shared_ptr�̴�.

	// RefCountBlock(useCount(shared), weakCount)
	// ���⼭ useCount�� shared_ptr ���� if==0: [Knight]����X, Ref�� �������(if weakCount != 0)
	// weakCount�� weak_ptr ����

	if (spr3 != nullptr) {

	}

	/*
	KnightRef k2(new Knight());
	k2->ReleaseRef();


	// Cycle
	k1->SetTarget(k2);
	k2->SetTarget(k1);

	// To solve Cycle
	k1->SetTarget(nullptr);
	k2->SetTarget(nullptr);


	k1 = nullptr;
	k2 = nullptr;
	*/
}