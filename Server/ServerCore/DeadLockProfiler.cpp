#include "pch.h"
#include "DeadLockProfiler.h"

/*--------------------
	DeadLockProfiler
---------------------*/

void DeadLockProfiler::PushLock(const char* name)
{
	LockGuard guard(_lock);

	// 아이디를 찾거나 발급한다.
	// Find or create Id
	int32 lockId = 0;

	auto findIt = _nameToId.find(name);
	if (findIt == _nameToId.end())
	{
		lockId = static_cast<int32>(_nameToId.size());
		_nameToId[name] = lockId;
		_idToName[lockId] = name;
	}
	else
	{	//key : iter->first, value : iter->second
		lockId = findIt->second;
	}
	// 잡고 있는 락이 있었다면
	// If there was lock holded
	if (_lockStack.empty() == false)
	{
		// 기존에 발견되지 않은 케이스라면, 데드락 여부 다시 확인한다.
		const int32 prevId = _lockStack.top();
		// lockId==prevId is okay because recursive creating is available
		if (lockId != prevId)
		{
			// get history which prev owns
			set<int32>& history = _lockHistory[prevId];
			// if there was no lockId from the history
			if (history.find(lockId) == history.end())
			{
				history.insert(lockId);
				CheckCycle();
			}
		}
	}

	_lockStack.push(lockId);
}

void DeadLockProfiler::PopLock(const char* name)
{
	LockGuard guard(_lock);

	if (_lockStack.empty())
		CRASH("MULTIPLE_UNLOCK");

	int32 lockId = _nameToId[name];
	if (_lockStack.top() != lockId)
		CRASH("INVALID_UNLOCK");

	_lockStack.pop();
}

void DeadLockProfiler::CheckCycle()
{
	// Initializing
	const int32 lockCount = static_cast<int32>(_nameToId.size());
	_discoveredOrder = vector<int32>(lockCount, -1);
	_discoveredCount = 0;
	_finished = vector<bool>(lockCount, false);
	_parent = vector<int32>(lockCount, -1);

	// Check
	for (int32 lockId = 0; lockId < lockCount; lockId++)
		Dfs(lockId);

	// 연산이 끝났으면 정리한다.
	// After calculation, Clear
	_discoveredOrder.clear();
	_finished.clear();
	_parent.clear();
}

void DeadLockProfiler::Dfs(int32 here)
{
	// if it already has been visited
	if (_discoveredOrder[here] != -1)
		return;
	// Mark as visited here
	_discoveredOrder[here] = _discoveredCount++;

	// 모든 인접한 정점을 순회한다. (DFS)
	auto findIt = _lockHistory.find(here);
	// if it doesn't have any history
	if (findIt == _lockHistory.end())
	{
		// finish this DFS
		_finished[here] = true;
		return;
	}

	set<int32>& nextSet = findIt->second;

}
