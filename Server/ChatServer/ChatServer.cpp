#include <iostream>
#include "ConfigMgr.h"
#include "AsioServicePool.h"
#include "Server.h"
#include "RedisMgr.h"
#include "ChatServiceImpl.h"

int main()
{
	auto& cfg = ConfigMgr::GetInstance();
	auto serverName = cfg["CurrentServer"]["name"];
	try
	{
		//Set login number
		CRedisMgr::GetInstance()->HSet(LOGIN_NUMBER, serverName, "0");
		Defer defer([serverName]() {
			CRedisMgr::GetInstance()->HDel(LOGIN_NUMBER, serverName);
			CRedisMgr::GetInstance()->Close();
		});

		//CServer
		boost::asio::io_context ioc;
		auto port = cfg["CurrentServer"]["port"];
		std::shared_ptr<CServer> pServer = std::make_shared<CServer>(ioc, atoi(port.c_str()));
		pServer->Start();

		//Grpc service
		ChatServiceImpl service;
		grpc::ServerBuilder builder;
		std::string serverAddress(cfg["CurrentServer"]["host"] + ":" + cfg["CurrentServer"]["rpcport"]);
		//Listen and register service
		builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
		builder.RegisterService(&service);

		//Start a gprc server
		std::unique_ptr<grpc::Server> grpcServer = builder.BuildAndStart();
		std::cout << "Start grpc server on " << serverAddress << std::endl;
		std::thread grpcThread([&grpcServer]() {
			grpcServer->Wait();
		});

		boost::asio::signal_set signals(ioc, SIGTERM, SIGINT);
		signals.async_wait([&ioc, &grpcServer](auto, auto) {
			ioc.stop();
			grpcServer->Shutdown();
		});

		ioc.run();		
		grpcThread.join();
	}
	catch (const std::exception& e)
	{
		CRedisMgr::GetInstance()->HDel(LOGIN_NUMBER, serverName);
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}

