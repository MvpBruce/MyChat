#include "HttpConnection.h"
#include <iostream>
#include "TaskSystem.h"
#include "Utils.h"

CHttpConnection::CHttpConnection(net::io_context& ioc):
m_socket(ioc), m_timer(ioc, std::chrono::seconds(60))
{
}

void CHttpConnection::Start()
{
	auto self = shared_from_this();
	http::async_read(m_socket, m_buffer, m_parser, [self](boost::system::error_code ec, std::size_t bytes_transferred) {
		try
		{
			if (ec)
			{
				std::cout << "Http read error is : " << ec.what() << std::endl;
				return;
			}

			self->HandleRequest();
			self->CheckTimeOut();
		}
		catch (std::exception e)
		{
			std::cout << "CHttpConnection::Start() exception is: " << e.what() << std::endl;
		}
	});
}

tcp::socket& CHttpConnection::GetSocket()
{
	return m_socket;
}

void CHttpConnection::HandleRequest()
{
	auto req = m_parser.get();
	//Create a response
	m_response.version(req.version());
	m_response.keep_alive(false);
	if (req.method() == http::verb::get)
	{
		ParseParams(req.target());
		bool ret = CTaskSystem::GetInstance()->ExecuteEvent(m_url , shared_from_this(), req.method());
		//error
		if (!ret)
		{
			m_response.result(http::status::not_found);
			m_response.set(http::field::content_type, "text/plain");
			beast::ostream(m_response.body()) << "Url not found" << std::endl;
			WriteResopnse();
			return;
		}
		
		//correct
		m_response.result(http::status::ok);
		m_response.set(http::field::server, "GateServer");
		WriteResopnse();
	}
	else if (req.method() == http::verb::post)
	{
		ParseParams(req.target());
		bool ret = CTaskSystem::GetInstance()->ExecuteEvent(m_url, shared_from_this(), req.method());
		if (!ret)
		{
			m_response.result(http::status::not_found);
			m_response.set(http::field::content_type, "text/plain");
			beast::ostream(m_response.body()) << "Url not found" << std::endl;
			WriteResopnse();
			return;
		}

		m_response.result(http::status::ok);
		m_response.set(http::field::server, "GateServer");
		WriteResopnse();
	}
}

void CHttpConnection::WriteResopnse()
{
	m_response.prepare_payload();
	auto self = shared_from_this();
	//Write response
	http::async_write(m_socket, m_response, [self](boost::system::error_code ec, std::size_t bytes_transferred) {
		if (ec)
		{
			std::cerr << "Write error: " << ec.what() << std::endl;
		}

		boost::system::error_code ignored_ec;
		self->m_socket.shutdown(tcp::socket::shutdown_send, ignored_ec);
		self->m_timer.cancel();
	});
}

void CHttpConnection::CheckTimeOut()
{
	auto self = shared_from_this();
	m_timer.async_wait([self](boost::system::error_code ec) {
		if (!ec)
			self->m_socket.close();
	});
}

void CHttpConnection::ParseParams(std::string strTarget)
{
	size_t nPos = strTarget.find("?");
	if (nPos == std::string::npos)
	{
		m_url = strTarget;
		return;
	}

	m_url = strTarget.substr(0, nPos);
	std::string strParams = strTarget.substr(nPos + 1);
	std::string strKey;
	std::string strValue;
	size_t nSplitedPos = strParams.find("&");
	while (nSplitedPos != std::string::npos)
	{
		std::string strParam = strParams.substr(0, nSplitedPos);
		size_t nPos_eql = strParam.find("=");
		if (nPos_eql != std::string::npos)
		{
			strKey = url_encode(strParam.substr(0, nPos_eql));
			strValue = url_encode(strParam.substr(nPos_eql + 1));
			m_parameters[strKey] = strValue;
		}

		strParams.erase(0, nSplitedPos + 1);
		nSplitedPos = strParams.find("&");
	}

	//Process last one
	if (!strParams.empty())
	{
		size_t nPos_eql = strParams.find("=");
		if (nPos_eql != std::string::npos)
		{
			strKey = url_encode(strParams.substr(0, nPos_eql));
			strValue = url_encode(strParams.substr(nPos_eql + 1));
			m_parameters[strKey] = strValue;
		}
	}
}
