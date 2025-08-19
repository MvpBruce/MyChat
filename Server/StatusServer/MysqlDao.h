#pragma once
#include "MySqlPool.h"

struct UserInfo {
	std::string name;
	std::string pwd;
	int uid;
	std::string email;
};

class MysqlDao
{
public:
	MysqlDao();
	~MysqlDao();

	int RegisterUser(const std::string& user, const std::string& email, const std::string& password);
	bool CheckEmail(const std::string& user, const std::string& eamil);
	bool UpdatePassowrd(const std::string& user, const std::string& password);
	bool CheckPassword(const std::string& user, const std::string& password, UserInfo& userInfo);

private:
	std::unique_ptr<MySqlPool> m_pool;
};

