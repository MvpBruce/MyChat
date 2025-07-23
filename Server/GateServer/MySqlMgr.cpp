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

bool MySqlMgr::UpdatePassowrd(const std::string& user, const std::string& password)
{
	return m_mySqlDao.UpdatePassowrd(user, password);
}
