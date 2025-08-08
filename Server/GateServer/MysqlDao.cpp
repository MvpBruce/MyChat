#include "MysqlDao.h"
#include "jdbc/cppconn/prepared_statement.h"
#include "ConfigMgr.h"

MysqlDao::MysqlDao()
{
	auto& configMgr = ConfigMgr::GetInstance();
	//Get config from ini, todo
	std::string url = configMgr["Mysql"]["host"];
	std::string port = configMgr["Mysql"]["port"];
	std::string user = configMgr["Mysql"]["user"];
	std::string password = configMgr["Mysql"]["pwd"];
	std::string schema = configMgr["Mysql"]["schema"];
	m_pool.reset(new MySqlPool(url + ":" + port, user, password, schema, 5));
}

MysqlDao::~MysqlDao()
{
	m_pool->Close();
}

int MysqlDao::RegisterUser(const std::string& user, const std::string& email, const std::string& password)
{
	auto con = m_pool->GetConnection();
	if (!con)
		return -1;

	Defer defer([this, &con]() {
		m_pool->ReturnConnection(std::move(con));
	});

	try
	{
		std::unique_ptr<sql::PreparedStatement> st(con->prepareStatement("CALL reg_user(?,?,?,@result)"));
		st->setString(1, user);
		st->setString(2, email);
		st->setString(3, password);

		st->execute();

		std::unique_ptr<sql::Statement> stm(con->createStatement());
		std::unique_ptr<sql::ResultSet> res(stm->executeQuery("SELECT @result AS result"));
		if (res->next())
		{
			int result = res->getInt("result");
			std::cout << "Result: " << result << std::endl;
			return result;
		}

		return -1;
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return -1;
	}
}

bool MysqlDao::CheckEmail(const std::string& user, const std::string& email)
{
	auto con = m_pool->GetConnection();
	if (!con)
		return false;

	Defer defer([this, &con]() {
		m_pool->ReturnConnection(std::move(con));
		});

	try {
		//Prepare script
		std::unique_ptr<sql::PreparedStatement> st(con->prepareStatement("SELECT email from user where name = ?"));
		st->setString(1, user);

		//Execute sql
		std::unique_ptr<sql::ResultSet> res(st->executeQuery());

		while (res->next())
		{
			std::cout << "Check email: " << res->getString("email") << std::endl;
			if (email != res->getString("email"))
				return false;

			return true;
		}
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
}

bool MysqlDao::UpdatePassowrd(const std::string& user, const std::string& password)
{
	auto con = m_pool->GetConnection();
	if (!con)
		return false;

	Defer defer([this, &con]() {
		m_pool->ReturnConnection(std::move(con));
	});

	try {
		std::unique_ptr<sql::PreparedStatement> st(con->prepareStatement("UPDATE user SET pwd = ? WHERE name = ?"));
		st->setString(1, password);
		st->setString(2, user);

		int updateCount = st->executeUpdate();
		std::cout << "Updated rows: " << updateCount << std::endl;
		return true;
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
}

bool MysqlDao::CheckPassword(const std::string& user, const std::string& password, UserInfo& userInfo)
{
	auto con = m_pool->GetConnection();
	if (!con)
		return false;

	Defer defer([this, &con]() {
		m_pool->ReturnConnection(std::move(con));
	});

	try {
		//Prepare script
		std::unique_ptr<sql::PreparedStatement> st(con->prepareStatement("SELECT * from user WHERE name = ?"));
		st->setString(1, user);

		//Execute sql
		std::unique_ptr<sql::ResultSet> res(st->executeQuery());
		std::string pwd = "";
		while (res->next())
		{
			pwd = res->getString("pwd");
			std::cout << "Password: " << pwd << std::endl;
			break;
		}

		if (pwd != password)
			return false;

		userInfo.name = res->getString("name");
		userInfo.email = res->getString("email");
		userInfo.uid = res->getInt("uid");
		userInfo.pwd = pwd;
		return true;
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
}