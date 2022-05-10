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
	Knight& _knight;	// 참조값이므로, RefCount를 늘리지 않는다.
	// KnightRef 로 적으면 Cycle 이 생긴다. (메모리 해제되지 않는 Memory Leak 현상 발생)
};
*/

int main()
{
	// SharedPtr 한계
	// 1) 이미 만들어진 클래스(외부 라이브러리 등) 대상으로 사용 불가
	// ex) 외부 클래스 : public RefCountable <--상속 넣기 불가능
	// 2) 순환 (Cycle) 문제

	KnightRef k1(new Knight());
	k1->ReleaseRef();
	//k1->_inventory = new Inventory(k1);

	// 'Shared_ptr' is from std library and it makes the same problem (Cycle)
	shared_ptr<Knight> spr;

	// 'Unique_ptr' prevents copying
	unique_ptr<Knight> k2 = make_unique<Knight>();
	unique_ptr<Knight> k3 = std::move(k2);	// 복사는 안되지만, 소유권 없애고 이동은 가능

	// shared_ptr doesn't need any Inheritance
	// (한계였던) 상속 불필요. 동작 조금 다름
	// [Knight][RefCountingBlock] 기존 수동코드

	// [T*][RefCountBlock*] shared_ptr도 포인터 두 개임
	shared_ptr<Knight> spr(new Knight());
	// make_shared 차이점: [Knight | RefCountingBlock(_Uses, _Weak)] RefCounting은 shared_ptr과 weak_ptr 모두 공통
	shared_ptr<Knight> spr = make_shared<Knight>();

	// [T*][RefCountBlock*] 복사 가능
	shared_ptr<Knight> spr2 = spr;


	
	// weak_ptr는 shared_ptr 받아서 저장 가능ㅋ
	// weak_ptr는 shared_ptr마냥 포인터로 사용할 수 없고, 존재하는지 아닌지 먼저 체크해야함

	weak_ptr<Knight> wpr = spr;

	// Check method #1
	bool expired = wpr.expired();
	// Check method #2 (convert to)shared_ptr로 변환
	shared_ptr<Knight> spr3 = wpr.lock();	// 존재하지 않을 때는 spr3 == nullptr
	// 따라서 weak_ptr은 존재여부를 확인 중시하는 반쪽짜리 shared_ptr이다.

	// RefCountBlock(useCount(shared), weakCount)
	// 여기서 useCount는 shared_ptr 개수 if==0: [Knight]존재X, Ref는 살아있음(if weakCount != 0)
	// weakCount는 weak_ptr 개수

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