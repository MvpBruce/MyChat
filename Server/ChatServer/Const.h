#pragma once
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/uuid.hpp>
#include "Singleton.h"
#include <functional>
#include <unordered_map>
#include <vector>
#include <queue>
#include <memory>

//json
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>

namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
using tcp = net::ip::tcp;


enum class ErrorCodes {
	Success = 0,
	Error_Json = 1001,
	Error_RPC = 1002,
	Verify_Expired = 1003,
	Verify_Error = 1004,
	User_Existed = 1005,
	Email_Not_Matched = 1006,
	Password_Update_Failed = 1007,
	Password_Invalid = 1008
};

class Defer
{
public:
	Defer(std::function<void(void)> func) : m_func(func) {}
	~Defer() { m_func(); }

private:
	std::function<void(void)> m_func;
};

enum class MSG_IDS
{
	MSG_CHAT_LOGIN = 1005,
	MSG_CHAT_LOGIN_RSP = 1006
};

#define MAX_LEN 2048
#define HEAD_MAX_LEN 4
#define HEAD_ID_LEN 2
#define HEAD_DATA_LEN 2
#define MAX_SEND_QUEUE 1000

#define LOGIN_NUMBER "loginnumber"