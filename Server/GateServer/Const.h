#pragma once
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "Singleton.h"
#include <functional>
#include <map>
#include <unordered_map>

namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
using tcp = net::ip::tcp;