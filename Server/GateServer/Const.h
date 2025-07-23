#pragma once
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "Singleton.h"
#include <functional>
#include <unordered_map>
#include <vector>
#include <queue>

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
	Password_Update_Failed = 1007
};