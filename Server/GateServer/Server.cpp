#include "Server.h"
#include <iostream>
#include "HttpConnection.h"

CServer::CServer(net::io_context& ioc, unsigned short nPort):
	m_ioc(ioc), m_acceptor(ioc, tcp::endpoint(tcp::v4(), nPort)), m_socket(ioc)
{
	std::cout << "Started server: " << net::ip::host_name() << ":" << nPort << std::endl;
}

void CServer::Start()
{
	auto self = shared_from_this();
	m_acceptor.async_accept(m_socket, [self](boost::system::error_code ec) {
		try
		{
			if (ec)
				self->Start();

			//Create a http connection
			std::shared_ptr<CHttpConnection> httpConnection = std::make_shared<CHttpConnection>(std::move(self->m_socket));
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