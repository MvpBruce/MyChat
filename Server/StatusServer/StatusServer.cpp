
#include <iostream>
#include "ConfigMgr.h"
#include "StatusServiceImp.h"
#include <boost/asio.hpp>

void Run()
{
	auto& configMgr = ConfigMgr::GetInstance();
	auto serverAddress = configMgr["StatusServer"]["host"] + ":" + configMgr["StatusServer"]["port"];
	StatusServiceImp service;
	grpc::ServerBuilder builder;
	builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
	builder.RegisterService(&service);

	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	std::cout << "Runing  server on " << serverAddress << std::endl;

	boost::asio::io_context ioc;
	boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
	signals.async_wait([&server, &ioc] (const boost::system::error_code& error, int signal_number) {
		if (!error)
		{
			std::cout << "Closing server..." << std::endl;
			server->Shutdown();
			ioc.stop();
		}
	});

	std::thread([&ioc]() {ioc.run();}).detach();
	server->Wait();
}

int main()
{
	try
	{
		Run();
	}
	catch (const std::exception&)
	{
		std::cerr << "Closing Status server..." << std::endl;
		return EXIT_FAILURE;
	}

	return 0;
}

