#include <iostream>
#include "ConfigMgr.h"
#include "AsioServicePool.h"
#include "Server.h"

int main()
{
	try
	{
		auto& cfg = ConfigMgr::GetInstance();
		boost::asio::io_context ioc;
		boost::asio::signal_set signals(ioc, SIGTERM, SIGINT);
		signals.async_wait([&ioc](auto, auto) {
			ioc.stop();
		});

		auto port = cfg["ChatServer1"]["port"];
		std::shared_ptr<CServer> pServer = std::make_shared<CServer>(ioc, atoi(port.c_str()));
		pServer->Start();
		ioc.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}

