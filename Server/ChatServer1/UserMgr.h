#pragma once
#include "Singleton.h"
#include <mutex>
#include <unordered_map>

class Session;
class UserMgr : public Singleton<UserMgr>
{
	friend class Singleton<UserMgr>;
public:
	~UserMgr();
	std::shared_ptr<Session> GetSession(int uid);
	void SetSession(int uid, std::shared_ptr<Session> pSession);
	void DelSession(int uid);

private:
	UserMgr();

private:
	std::mutex m_Mutex;
	std::unordered_map<int, std::shared_ptr<Session>> m_mapSession;
};

