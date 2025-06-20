#pragma once
#include "Const.h"

class CServer:public std::enable_shared_from_this<CServer>
{
public:
	CServer(net::io_context& ioc, unsigned short nPort);

	void Start();

private:
	net::io_context& m_ioc;
	tcp::socket m_socket;
	tcp::acceptor m_acceptor;
};

