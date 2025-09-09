#pragma once
#include "MySqlPool.h"
#include "UserData.h"

class MysqlDao
{
public:
	MysqlDao();
	~MysqlDao();

	int RegisterUser(const std::string& user, const std::string& email, const std::string& password);
	bool CheckEmail(const std::string& user, const std::string& eamil);
	bool UpdatePassowrd(const std::string& user, const std::string& password);
	bool CheckPassword(const std::string& user, const std::string& password, UserInfo& userInfo);
	std::shared_ptr<UserInfo> GetUserInfo(int nUid);
	std::shared_ptr<UserInfo> GetUserInfo(const std::string& name);

private:
	std::unique_ptr<MySqlPool> m_pool;
};

