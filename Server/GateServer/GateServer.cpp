// GateServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "Server.h"
#include "hiredis.h"

int main()
{
    try
    {
        //test redis


        //end test
        boost::asio::io_context ioc;
        boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc](boost::system::error_code ec, int signals) {
            if (ec)
                return;

            ioc.stop();
        });
     
        unsigned short nPort = static_cast<unsigned short>(8080);
        std::shared_ptr<CServer> pServer = std::make_shared<CServer>(ioc, nPort);
        pServer->Start();
        ioc.run();
    }
    catch (std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
