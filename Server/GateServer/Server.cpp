#include "Server.h"
#include <iostream>
#include "HttpConnection.h"
#include "AsioServicePool.h"

CServer::CServer(net::io_context& ioc, unsigned short nPort):
	m_ioc(ioc), m_acceptor(ioc, tcp::endpoint(tcp::v4(), nPort))
{
	std::cout << "Running server: " << "127.0.0.1" << ":" << nPort << std::endl;
}

void CServer::Start()
{
	auto self = shared_from_this();
	auto& ioc = AsioServicePool::GetInstance()->GetService();
	std::shared_ptr<CHttpConnection> httpConnection = std::make_shared<CHttpConnection>(ioc);
	m_acceptor.async_accept(httpConnection->GetSocket(), [self, httpConnection](boost::system::error_code ec) {
		try
		{
			if (ec)
				self->Start();

			//Create a http connection
			httpConnection->Start();
			self->Start();
		}
		catch (std::exception& ec)
		{
			std::cout << "CServer::Start() exception is: " << ec.what() << std::endl;
			self->Start();
		}
	});
}