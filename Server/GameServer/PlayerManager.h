#pragma once

class PlayerManager
{
	USE_LOCK;

public:
	// Player Lock and then Account Lock
	void PlayerThenAccount();
	void Lock();
};

extern PlayerManager GPlayerManager;