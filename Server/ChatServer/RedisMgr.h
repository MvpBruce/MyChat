#pragma once
#include "Singleton.h"
#include "hiredis.h"

class RedisPool;
class CRedisMgr : public Singleton<CRedisMgr>, public std::enable_shared_from_this<CRedisMgr>
{
	friend class Singleton<CRedisMgr>;
public:
	~CRedisMgr();

	//bool Connect(const std::string& url, int port);
	//bool Auth(const std::string& password);
	bool Set(const std::string& key, const std::string& value);
	bool Get(const std::string& key, std::string& value);
	bool LPush(const std::string& key, const std::string& value);
	bool LPop(const std::string& key, std::string& value);
	bool RPush(const std::string& key, const std::string& value);
	bool RPop(const std::string& key, std::string& value);
	bool HSet(const std::string& table, const std::string& key, const std::string& value);
	bool HGet(const std::string& table, std::string key, std::string& value);
	bool Delete(const std::string& key);
	bool HDel(const std::string& table, const std::string& key);
	bool ExistsKey(const std::string& key);
	void Close();

private:
	CRedisMgr();

private:
	redisReply* m_reply;
	std::unique_ptr<RedisPool> m_redisPool;
};

#include <queue>
#include <mutex>
class RedisPool
{
public:
	RedisPool(std::string& url, unsigned int port, std::string& password, size_t nSize)
		: m_bStop(false)
	{
		for (size_t i = 0; i < nSize; i++)
		{
			auto con = redisConnect(url.c_str(), port);
			if (!con || (con && con->err))
			{
				std::cout << "Failed to connect " << url << ":" << port << std::endl;
				redisFree(con);
				continue;
			}

			redisReply* pReply = (redisReply*)redisCommand(con, "AUTH %s", password.c_str());
			if (pReply->type == REDIS_REPLY_ERROR)
			{
				std::cout << "Failed to authenticate " << std::endl;
				freeReplyObject(pReply);
				continue;
			}

			std::cout << "Success to authenticate" << std::endl;
			freeReplyObject(pReply);

			m_contexts.emplace(con);
		}
	}

	~RedisPool()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_bStop = true;
		m_cond.notify_all();
		while (!m_contexts.empty())
		{
			auto con = m_contexts.front();
			redisFree(con);
			m_contexts.pop();
		}
	}

	redisContext* GetConnection()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cond.wait(lock, [this]() {
			if (m_bStop)
				return true;

			return !m_contexts.empty();
		});

		if (m_bStop)
			return nullptr;

		auto con = m_contexts.front();
		m_contexts.pop();
		return con;
	}

	void RetrunConnection(redisContext* con)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		if (m_bStop)
			return;

		m_contexts.push(con);
		m_cond.notify_one();
	}

private:
	std::queue<redisContext*> m_contexts;
	std::mutex m_mutex;
	std::condition_variable m_cond;
	std::atomic<bool> m_bStop;
};