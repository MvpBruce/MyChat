#pragma once
#include "Singleton.h"
#include "hiredis.h"

class CRedisMgr : public Singleton<CRedisMgr>, public std::enable_shared_from_this<CRedisMgr>
{
	friend class Singleton<CRedisMgr>;
public:
	~CRedisMgr();

	bool Connect(const std::string& url, int port);
	bool Auth(const std::string& password);
	bool Set(const std::string& key, const std::string& value);
	bool Get(const std::string& key, std::string& value);
	bool LPush(const std::string& key, const std::string& value);
	bool LPop(const std::string& key, std::string& value);
	bool RPush(const std::string& key, const std::string& value);
	bool RPop(const std::string& key, std::string& value);
	bool HSet(const std::string& table, const std::string& key, const std::string& value);
	bool HGet(const std::string& table, std::string key, std::string& value);
	bool Delete(const std::string& key);
	bool Exist(const std::string& key);
	void Close();

private:
	CRedisMgr();

private:
	redisContext* m_context;
	redisReply* m_reply;
};

