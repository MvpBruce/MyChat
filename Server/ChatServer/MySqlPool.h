#pragma once
#include "Const.h"
#include "jdbc/mysql_connection.h"
#include <atomic>

class MySqlPool
{
public:
	MySqlPool(const std::string& url, const std::string& user, const std::string& password, const std::string& schema, int nPoolSize);
	~MySqlPool();

	std::unique_ptr<sql::Connection> GetConnection();
	void ReturnConnection(std::unique_ptr<sql::Connection> con);
	void Close();

private:
	std::string m_url;
	std::string m_user;
	std::string m_passowrd;
	std::string m_schema;
	int m_nPoolSize;
	std::condition_variable m_cond;
	std::mutex m_mutex;
	std::queue <std::unique_ptr<sql::Connection>> m_pool;
	std::atomic<bool> m_stop;
};

