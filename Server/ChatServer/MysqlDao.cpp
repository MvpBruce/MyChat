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

		userInfo.name = user;
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

std::shared_ptr<UserInfo> MysqlDao::GetUserInfo(int nUid)
{
	auto con = m_pool->GetConnection();
	if (!con)
		return nullptr;

	Defer defer([this, &con]() {
		m_pool->ReturnConnection(std::move(con));
	});

	try {
		std::unique_ptr<sql::PreparedStatement> st(con->prepareStatement("SELECT * from user WHERE uid = ?"));
		st->setInt(1, nUid);
		//Execute sql
		std::unique_ptr<sql::ResultSet> res(st->executeQuery());
		std::shared_ptr<UserInfo> pUserInfo = nullptr;
		while (res->next())
		{
			pUserInfo.reset(new UserInfo);
			pUserInfo->pwd = res->getString("pwd");
			pUserInfo->email = res->getString("email");
			pUserInfo->name = res->getString("name");
			pUserInfo->nick = res->getString("nick");
			pUserInfo->desc = res->getString("desc");
			pUserInfo->gender = res->getInt("gender");
			pUserInfo->icon = res->getString("icon");
			pUserInfo->uid = nUid;
			break;
		}

		return pUserInfo;
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return nullptr;
	}
}

std::shared_ptr<UserInfo> MysqlDao::GetUserInfo(const std::string& name)
{
	auto con = m_pool->GetConnection();
	if (!con)
		return nullptr;

	Defer defer([this, &con]() {
		m_pool->ReturnConnection(std::move(con));
	});

	try {
		std::unique_ptr<sql::PreparedStatement> st(con->prepareStatement("SELECT * from user WHERE name = ?"));
		st->setString(1, name);
		//Execute sql
		std::unique_ptr<sql::ResultSet> res(st->executeQuery());
		std::shared_ptr<UserInfo> pUserInfo = nullptr;
		while (res->next())
		{
			pUserInfo.reset(new UserInfo);
			pUserInfo->pwd = res->getString("pwd");
			pUserInfo->email = res->getString("email");
			pUserInfo->name = name;
			pUserInfo->nick = res->getString("nick");
			pUserInfo->desc = res->getString("desc");
			pUserInfo->gender = res->getInt("gender");
			pUserInfo->icon = res->getString("icon");
			pUserInfo->uid = res->getInt("uid");
			break;
		}

		return pUserInfo;
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return nullptr;
	}
}

bool MysqlDao::AddFriendApply(const int& from, const int& to)
{
	auto con = m_pool->GetConnection();
	if (!con)
		return false;

	Defer defer([this, &con]() {
		m_pool->ReturnConnection(std::move(con));
	});

	try 
	{
		std::unique_ptr<sql::PreparedStatement> st(con->prepareStatement("INSERT INTO friend_apply(from_uid, to_uid) VALUES(?, ?) ON DUPLICATE KEY UPDATE from_uid = from_uid, to_uid = to_uid"));
		st->setInt(1, from);
		st->setInt(2, to);
		if (st->executeUpdate() < 0)
			return false;
		
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

bool MysqlDao::GetApplyList(int toUId, std::vector<std::shared_ptr<ApplyInfo>>& vecApplyInfo, int offset, int limit)
{
	auto con = m_pool->GetConnection();
	if (!con)
		return false;

	Defer defer([this, &con]() {
		m_pool->ReturnConnection(std::move(con));
	});

	try
	{
		std::unique_ptr<sql::PreparedStatement> st(con->prepareStatement("SELECT apply.from_uid, apply.status, user.name, "
		"user.nick, user.gender from friend_apply as apply join user on apply.from_uid = user.uid where apply.to_uid = ? "
		"and apply.id > ? order by apply.id ASC LIMIT ?"));
		st->setInt(1, toUId);
		st->setInt(2, offset);
		st->setInt(3, limit);
		std::unique_ptr<sql::ResultSet> res(st->executeQuery());
		while (res->next())
		{
			auto name = res->getString("name");
			auto uid = res->getInt("from_uid");
			auto status = res->getInt("status");
			auto nick = res->getString("nick");
			auto gender = res->getInt("gender");

			auto pResult = std::make_shared<ApplyInfo>(uid, gender, name, "", "", nick, status);
			vecApplyInfo.push_back(pResult);
		}
			
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

bool MysqlDao::GetFriendList(int uid, std::vector<std::shared_ptr<UserInfo>>& vUserInfo)
{
	auto con = m_pool->GetConnection();
	if (!con)
		return false;

	Defer defer([this, &con]() {
		m_pool->ReturnConnection(std::move(con));
	});

	try
	{
		std::unique_ptr<sql::PreparedStatement> st(con->prepareStatement("select * from friend where self_id = ? "));
		st->setInt(1, uid);
		std::unique_ptr<sql::ResultSet> res(st->executeQuery());
		while (res->next())
		{
			auto friendId = res->getInt("friend_id");
			auto back = res->getString("back");
			auto pUserInfo = GetUserInfo(friendId);
			if (pUserInfo == nullptr)
				continue;

			pUserInfo->back = pUserInfo->name;
			vUserInfo.push_back(pUserInfo);
		}

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

bool MysqlDao::AuthFriendApply(const int& from_uid, const int& to_uid)
{
	auto con = m_pool->GetConnection();
	if (!con)
		return false;

	Defer defer([this, &con]() {
		m_pool->ReturnConnection(std::move(con));
	});

	try
	{
		std::unique_ptr<sql::PreparedStatement> st(con->prepareStatement("UPDATE friend_apply SET status = 1 WHERE from_uid = ? AND to_uid = ?"));
		st->setInt(1, to_uid);
		st->setInt(2, from_uid);
		int nRow = st->executeUpdate();
		if (nRow < 0)
			return false;

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

bool MysqlDao::AddFriend(const int& from_uid, const int& to_uid, const std::string& strBackName)
{
	auto con = m_pool->GetConnection();
	if (!con)
		return false;

	Defer defer([this, &con]() {
		m_pool->ReturnConnection(std::move(con));
	});

	try
	{
		con->setAutoCommit(false);
		std::unique_ptr<sql::PreparedStatement> st(con->prepareStatement("INSERT IGNORE INTO friend(self_id, friend_id, back) VALUES(?, ?, ?)"));
		st->setInt(1, from_uid);
		st->setInt(2, to_uid);
		st->setString(3, strBackName);
		int nRow = st->executeUpdate();
		if (nRow < 0)
		{
			con->rollback();
			return false;
		}
			
		std::unique_ptr<sql::PreparedStatement> st1(con->prepareStatement("INSERT IGNORE INTO friend(self_id, friend_id, back) VALUES(?, ?, ?)"));
		st1->setInt(1, to_uid);
		st1->setInt(2, from_uid);
		st1->setString(3, "");
		nRow = st->executeUpdate();
		if (nRow < 0)
		{
			con->rollback();
			return false;
		}

		con->commit();
		return true;
	}
	catch (sql::SQLException& e)
	{
		if (con)
			con->rollback();

		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
}