#include "MySqlMgr.h"

MySqlMgr::MySqlMgr()
{

}

MySqlMgr::~MySqlMgr()
{

}

int MySqlMgr::RegisterUser(const std::string& user, const std::string& email, const std::string& password)
{
	return m_mySqlDao.RegisterUser(user, email, password);
}

bool MySqlMgr::CheckEmail(const std::string& user, const std::string& email)
{
	return m_mySqlDao.CheckEmail(user, email);
}

bool MySqlMgr::UpdatePassword(const std::string& user, const std::string& password)
{
	return m_mySqlDao.UpdatePassowrd(user, password);
}

bool MySqlMgr::CheckPassword(const std::string& user, const std::string& password, UserInfo& userInfo)
{
	return m_mySqlDao.CheckPassword(user, password, userInfo);
}

std::shared_ptr<UserInfo> MySqlMgr::GetUserInfo(int uUid)
{
	return m_mySqlDao.GetUserInfo(uUid);
}

std::shared_ptr<UserInfo> MySqlMgr::GetUserInfo(const std::string& name)
{
	return m_mySqlDao.GetUserInfo(name);
}

bool MySqlMgr::AddFriendApply(const int& from, const int& to)
{
	return m_mySqlDao.AddFriendApply(from, to);
}

bool MySqlMgr::GetApplyList(int toUId, std::vector<std::shared_ptr<ApplyInfo>>& vecApplyInfo, int offset, int limit)
{
	return m_mySqlDao.GetApplyList(toUId, vecApplyInfo, offset, limit);
}

bool MySqlMgr::GetFriendList(int uid, std::vector<std::shared_ptr<UserInfo>>& vUserInfo)
{
	return m_mySqlDao.GetFriendList(uid, vUserInfo);
}

bool MySqlMgr::AuthFriendApply(const int& from_uid, const int& to_uid)
{
	return m_mySqlDao.AuthFriendApply(from_uid, to_uid);
}

bool MySqlMgr::AddFriend(const int& from_uid, const int& to_uid, const std::string& strBackName)
{
	return m_mySqlDao.AddFriend(from_uid, to_uid, strBackName);
}

