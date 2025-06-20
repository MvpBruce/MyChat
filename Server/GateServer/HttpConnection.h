#pragma once
#include "Const.h"


class CHttpConnection : public std::enable_shared_from_this<CHttpConnection>
{
public:
	CHttpConnection(tcp::socket socket);

	void Start();

public:
	http::response<http::dynamic_body> m_response;
	std::unordered_map<std::string, std::string> m_parameters;
	http::request_parser<http::dynamic_body> m_parser;

private:
	void HandleRequest();
	void WriteResopnse();
	void CheckTimeOut();
	void ParseParams(std::string strTarget);

private:
	tcp::socket m_socket;
	beast::flat_buffer m_buffer;
	net::steady_timer m_timer;
	std::string m_url;
};

