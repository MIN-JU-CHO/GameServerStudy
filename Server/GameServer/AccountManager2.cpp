#include "pch.h"
#include "AccountManager2.h"
#include "PlayerManager.h"

AccountManager2 GAccountManager;

void AccountManager2::AccountThenPlayer()
{
	WRITE_LOCK;
	GPlayerManager.Lock();
}

void AccountManager2::Lock()
{
	WRITE_LOCK;
}