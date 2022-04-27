#pragma once

class AccountManager2
{
	USE_LOCK;

public:
	// Account Lock and then Player Lock
	void AccountThenPlayer();
	void Lock();
};

extern AccountManager2 GAccountManager;