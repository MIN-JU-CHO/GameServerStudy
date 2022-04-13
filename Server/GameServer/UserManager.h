#pragma once
#include <mutex>

class User 
{
	// TODO
};

class UserManager
{
public:
	// 언제 어디서든 사용할 수 있도록, 싱글톤 패턴으로 만들어줌
	static UserManager* Instance() 
	{
		static UserManager instance;
		return &instance;
	}

	User* GetUser(int32 id) {
		lock_guard<mutex> guard(_mutex);
		// 뭔가 갖고 옴
		return nullptr;
	}

	void ProcessSave();

private:
	mutex _mutex;
};

