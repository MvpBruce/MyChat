#include "RedisMgr.h"

CRedisMgr::~CRedisMgr()
{

}

bool CRedisMgr::Connect(const std::string& url, int port)
{
	m_context = redisConnect(url.c_str(), port);
	if (!m_context || (m_context && m_context->err))
	{
		std::cout << "Failed to connect " << url << ":" << port << std::endl;
		Close();
		return false;
	}

	return true;
}

bool CRedisMgr::Auth(const std::string& password)
{
	m_reply = (redisReply*)redisCommand(m_context, "AUTH %s", password.c_str());
	if (m_reply->type == REDIS_REPLY_ERROR)
	{
		std::cout << "Failed to authenticate " << std::endl;
		freeReplyObject(m_reply);
		return false;
	}
	else
	{
		std::cout << "Success to authenticate " << std::endl;
		freeReplyObject(m_reply);
		return true;
	}
}

bool CRedisMgr::Set(const std::string& key, const std::string& value)
{
	m_reply = (redisReply*)redisCommand(m_context, "SET %s %s", key.c_str(), value.c_str());
	if (!m_reply || m_reply->type != REDIS_REPLY_STATUS || (strcmp(m_reply->str, "ok") != 0 && strcmp(m_reply->str, "OK") != 0))
	{
		std::cout << "Failed to execute [SET " << key << " " << value << "]" << std::endl;
		freeReplyObject(m_reply);
		return false;
	}

	std::cout << "Success to execute [SET " << key << " " << value << "]" << std::endl;
	freeReplyObject(m_reply);
	return true;	
}

bool CRedisMgr::Get(const std::string& key, std::string& value)
{
	m_reply = (redisReply*)redisCommand(m_context, "GET %s", key.c_str());
	if (!m_reply || m_reply->type != REDIS_REPLY_STRING)
	{
		std::cout << "Failed to execute [GET " << key << "]" << std::endl;
		freeReplyObject(m_reply);
		return false;
	}

	std::cout << "Success to execute [GET " << key << "]" << std::endl;
	value = m_reply->str;
	freeReplyObject(m_reply);
	return true;
}

bool CRedisMgr::LPush(const std::string& key, const std::string& value)
{
	m_reply = (redisReply*)redisCommand(m_context, "LPUSH %s %s", key.c_str(), value.c_str());
	if (!m_reply || m_reply->type != REDIS_REPLY_INTEGER || m_reply->integer <= 0)
	{
		std::cout << "Failed to execute [LPUSH " << key << " " << value << "]" << std::endl;
		freeReplyObject(m_reply);
		return false;
	}

	std::cout << "Success to execute [LPUSH " << key << " " << value << "]" << std::endl;
	freeReplyObject(m_reply);
	return true;
}

bool CRedisMgr::LPop(const std::string& key, std::string& value)
{
	m_reply = (redisReply*)redisCommand(m_context, "LPOP %s", key.c_str());
	if (!m_reply || m_reply->type == REDIS_REPLY_NIL)
	{
		std::cout << "Failed to execute [LPOP " << key << "]" << std::endl;
		freeReplyObject(m_reply);
		return false;
	}

	std::cout << "Success to execute [LPOP " << key << "]" << std::endl;
	value = m_reply->str;
	freeReplyObject(m_reply);
	return true;
}

bool CRedisMgr::RPush(const std::string& key, const std::string& value)
{
	m_reply = (redisReply*)redisCommand(m_context, "RPUSH %s %s", key.c_str(), value.c_str());
	if(!m_reply || m_reply->type != REDIS_REPLY_INTEGER || m_reply->integer <= 0)
	{
		std::cout << "Failed to execute [RPUSH " << key << " " << value << "]" << std::endl;
		freeReplyObject(m_reply);
		return false;
	}

	std::cout << "Success to execute [RPUSH " << key << " " << value << "]" << std::endl;
	freeReplyObject(m_reply);
	return true;
}

bool CRedisMgr::RPop(const std::string& key, std::string& value)
{
	m_reply = (redisReply*)redisCommand(m_context, "RPOP %s %s", key.c_str(), value.c_str());
	if (!m_reply || m_reply->type == REDIS_REPLY_NIL)
	{
		std::cout << "Failed to execute [RPOP " << key << "]" << std::endl;
		freeReplyObject(m_reply);
		return false;
	}

	
	std::cout << "Success to execute [RPOP " << key << "]" << std::endl;
	value = m_reply->str;
	freeReplyObject(m_reply);
	return true;
}

bool CRedisMgr::HSet(const std::string& table, const std::string& key, const std::string& value)
{
	m_reply = (redisReply*)redisCommand(m_context, "HSET %s %s %s", table.c_str(), key.c_str(), value.c_str());
	if (!m_reply || m_reply->type != REDIS_REPLY_INTEGER)
	{
		std::cout << "Failed to execute [HSET " << table << " " << key << " " << value << "]" << std::endl;
		freeReplyObject(m_reply);
		return false;
	}

	std::cout << "Success to execute [HSET " << table << " " << key << " " << value << "]" << std::endl;
	freeReplyObject(m_reply);
	return true;
}

bool CRedisMgr::HGet(const std::string& table, std::string key, std::string& value)
{
	m_reply = (redisReply*)redisCommand(m_context, "HGET %s %s", table.c_str(), key.c_str());
	if (!m_reply || m_reply->type == REDIS_REPLY_NIL)
	{
		std::cout << "Failed to execute [HGET " << table << " " << key << " " << value << "]" << std::endl;
		freeReplyObject(m_reply);
		return false;
	}

	value = m_reply->str;
	std::cout << "Success to execute [HGET " << table << " " << key << " " << "]" << std::endl;
	freeReplyObject(m_reply);
	return true;

}

bool CRedisMgr::Delete(const std::string& key)
{
	m_reply = (redisReply*)redisCommand(m_context, "DEL %s", key.c_str());
	if (!m_reply || m_reply->type != REDIS_REPLY_INTEGER)
	{
		std::cout << "Failed to execute [DEL " << key << "]" << std::endl;
		freeReplyObject(m_reply);
		return false;
	}

	std::cout << "Success to execute [DEL " << key << "]" << std::endl;
	freeReplyObject(m_reply);
	return true;
}

bool CRedisMgr::ExistsKey(const std::string& key)
{
	m_reply = (redisReply*)redisCommand(m_context, "EXISTS %s", key.c_str());
	if (!m_reply || m_reply->type != REDIS_REPLY_INTEGER || m_reply->integer == 0)
	{
		std::cout << "Not Found [key " << key << "]" << std::endl;
		freeReplyObject(m_reply);
		return false;
	}

	std::cout << "Found [key " << key << "]" << std::endl;
	freeReplyObject(m_reply);
	return true;
}

void CRedisMgr::Close()
{
	redisFree(m_context);
}

CRedisMgr::CRedisMgr():
	m_context(nullptr), m_reply(nullptr)
{
}
