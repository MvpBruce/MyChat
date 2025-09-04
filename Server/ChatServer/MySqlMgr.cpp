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
