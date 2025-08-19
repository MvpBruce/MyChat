#include "Server.h"
#include <iostream>
#include "Session.h"
#include "AsioServicePool.h"

CServer::CServer(net::io_context& ioc, unsigned short nPort):
	m_ioc(ioc), m_acceptor(ioc, tcp::endpoint(tcp::v4(), nPort))
{
	std::cout << "Running server on port: " << nPort << std::endl;
}

void CServer::Start()
{
	auto self = shared_from_this();
	auto& ioc = AsioServicePool::GetInstance()->GetService();
	std::shared_ptr<Session> session = std::make_shared<Session>(ioc, this);
	m_acceptor.async_accept(session->GetSocket(), [self, session](boost::system::error_code ec) {
		try
		{
			if (!ec)
			{
				session->Start();
				std::lock_guard<std::mutex> lock(self->m_Mutex);
				self->m_Sessions.insert(std::make_pair(session->GetSessionId(), session));
			}

			self->Start();
		}
		catch (std::exception& ec)
		{
			std::cout << "CServer::Start() exception is: " << ec.what() << std::endl;
			self->Start();
		}
	});
}

CServer::~CServer()
{
	std::cout << "Server destructed" << std::endl;
}

void CServer::ClearSession(std::string uuid)
{
	if (m_Sessions.find(uuid) != m_Sessions.end())
	{
		//remove from usermgr, todo
	}

	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		m_Sessions.erase(uuid);
	}
}