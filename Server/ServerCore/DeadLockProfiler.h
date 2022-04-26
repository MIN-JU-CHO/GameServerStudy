#pragma once
#include <stack>
#include <map>
#include <vector>

/*--------------------
	DeadLockProfiler
---------------------*/

class DeadLockProfiler
{
public:
	void PushLock(const char* name);
	void PopLock(const char* name);
	void CheckCycle();				// == DfsAll

private:
	void Dfs(int32 index);

private:
	unordered_map<const char*, int32>	_nameToId;
	unordered_map<int32, const char*>	_idToName;
	stack<int32>						_lockStack;
	map<int32, set<int32>>				_lockHistory;	// Graph

	Mutex _lock;

private:
	vector<int32>	_discoveredOrder;		// 노드가 발견된 순서 기록하는 배열
	int32			_discoveredCount = 0;	// 노드가 발견된 순서
	vector<bool>	_finished;				// Dfs(i)가 종료 되었는지 여부 (for DfsAll)
	vector<int32>	_parent;				// 발견된 이후 본인의 부모 (parent of the node)
};

