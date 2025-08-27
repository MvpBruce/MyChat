#pragma once
#include "Const.h"

class Session;
class CServer:public std::enable_shared_from_this<CServer>
{
public:
	CServer(net::io_context& ioc, unsigned short nPort);
	~CServer();
	void Start();
	void ClearSession(std::string uuid);

private:
	net::io_context& m_ioc;
	tcp::acceptor m_acceptor;
	std::map<std::string, std::shared_ptr<Session>> m_Sessions;
	std::mutex m_Mutex;
};

