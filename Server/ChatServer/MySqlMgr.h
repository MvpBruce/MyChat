#pragma once
#include "Singleton.h"
#include "MysqlDao.h"

class MySqlMgr : public Singleton<MySqlMgr>
{
	friend class Singleton<MySqlMgr>;
public:
	~MySqlMgr();
	int RegisterUser(const std::string& user, const std::string& email, const std::string& password);
	bool CheckEmail(const std::string& user, const std::string& email);
	bool UpdatePassword(const std::string& user, const std::string& password);
	bool CheckPassword(const std::string& user, const std::string& password, UserInfo& userInfo);
	std::shared_ptr<UserInfo> GetUserInfo(int uUid);
private:
	MySqlMgr();
	MysqlDao m_mySqlDao;
};

