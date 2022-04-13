#include "pch.h"
#include "UserManager.h"
#include "AccountManager.h"

void UserManager::ProcessSave()
{
	// accountLock (from AccountManager::GetAccount(id))
	Account* account = AccountManager::Instance()->GetAccount(100);

	// userLock (from UserManager::ProcessSave())
	lock_guard<mutex> guard(_mutex);
	// 두 개의 Lock을 동시에 잡게 됨.
	// accountLock -> userLock 순서 일치 시켜주면 교착 해결
	// TODO
}
