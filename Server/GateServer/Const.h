#pragma once
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "Singleton.h"
#include <functional>
#include <unordered_map>
#include <vector>

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
	Error_RPC = 1002
};