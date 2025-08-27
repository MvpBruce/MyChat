#include "UserMgr.h"

UserMgr::~UserMgr()
{
	m_mapSession.clear();
}

std::shared_ptr<Session> UserMgr::GetSession(int uid)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	auto it = m_mapSession.find(uid);
	if (it == m_mapSession.end())
		return nullptr;

	return it->second;
}

void UserMgr::SetSession(int uid, std::shared_ptr<Session> pSession)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_mapSession[uid] = pSession;
}

void UserMgr::DelSession(int uid)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_mapSession.erase(uid);
}

UserMgr::UserMgr()
{
}
