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
	bool AddFriendApply(const int& from, const int& to);
	bool GetApplyList(int toUId, std::vector<std::shared_ptr<ApplyInfo>>& vecApplyInfo, int offset, int limit);
	bool GetFriendList(int uid, std::vector<std::shared_ptr<UserInfo>>& vUserInfo);

private:
	std::unique_ptr<MySqlPool> m_pool;
};

