#pragma once
#include "MySqlPool.h"

class MysqlDao
{
public:
	MysqlDao();
	~MysqlDao();

	int RegisterUser(const std::string& user, const std::string& email, const std::string& password);
	bool CheckEmail(const std::string& user, const std::string eamil);
	bool UpdatePassowrd(const std::string& user, const std::string& password);

private:
	std::unique_ptr<MySqlPool> m_pool;
};

