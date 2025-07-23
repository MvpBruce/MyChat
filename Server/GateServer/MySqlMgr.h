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
	bool UpdatePassowrd(const std::string& user, const std::string& password);

private:
	MySqlMgr();
	MysqlDao m_mySqlDao;
};

