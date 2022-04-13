#include "pch.h"
#include "AccountManager.h"
#include "UserManager.h"


void AccountManager::ProcessLogin() {
	// accountLock (from AccountManager::ProcessLogin())
	lock_guard<mutex> guard(_mutex);

	// userLock (from UserManager::GetUser(id))
	User* user = UserManager::Instance()->GetUser(100);
	// 두 개의 Lock 동시에 잡게 됨.
	// TODO
}