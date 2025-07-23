#include "MySqlPool.h"
#include "jdbc/mysql_driver.h"

MySqlPool::MySqlPool(const std::string& url, const std::string& user, const std::string& password, const std::string& schema, int nPoolSize):
	m_url(url), m_user(user), m_passowrd(password), m_schema(schema), m_nPoolSize(nPoolSize), m_stop(false)
{
	try 
	{
		for (int i = 0; i < nPoolSize; i++)
		{
			sql::mysql::MySQL_Driver* pDriver = sql::mysql::get_mysql_driver_instance();
			std::unique_ptr<sql::Connection> con(pDriver->connect(url, user, password));
			con->setSchema(schema);
			m_pool.push(std::move(con));
		}
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		std::cout << "MySql pool failed" << std::endl;
	}
}

MySqlPool::~MySqlPool()
{
	Close();
	while (!m_pool.empty())
		m_pool.pop();
}

std::unique_ptr<sql::Connection> MySqlPool::GetConnection()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_cond.wait(lock, [this]() {
		if (m_stop)
			return true;

		if (!m_pool.empty())
			return true;
	});

	if (m_stop)
		return nullptr;

	auto con = std::move(m_pool.front());
	m_pool.pop();
	return con;
}

void MySqlPool::ReturnConnection(std::unique_ptr<sql::Connection> con)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_stop)
		return;

	m_pool.push(std::move(con));
	m_cond.notify_one();
}

void MySqlPool::Close()
{
	m_stop = true;
	m_cond.notify_all();
}