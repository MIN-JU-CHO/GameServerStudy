#include "pch.h"
#include "UserManager.h"
#include "AccountManager.h"

void UserManager::ProcessSave()
{
	// accountLock (from AccountManager::GetAccount(id))
	Account* account = AccountManager::Instance()->GetAccount(100);

	// userLock (from UserManager::ProcessSave())
	lock_guard<mutex> guard(_mutex);
	// �� ���� Lock�� ���ÿ� ��� ��.
	// accountLock -> userLock ���� ��ġ �����ָ� ���� �ذ�
	// TODO
}
