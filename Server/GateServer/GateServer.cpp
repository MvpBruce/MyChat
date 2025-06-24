// GateServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "Server.h"
#include "hiredis.h"
#include "RedisMgr.h"
#include <assert.h>

void TestRedisMgr() {
    assert(CRedisMgr::GetInstance()->Connect("127.0.0.1", 6380));
    assert(CRedisMgr::GetInstance()->Auth("123456"));
    assert(CRedisMgr::GetInstance()->Set("blogwebsite", "llfc.club"));
    std::string value = "";
    assert(CRedisMgr::GetInstance()->Get("blogwebsite", value));
    assert(CRedisMgr::GetInstance()->Get("nonekey", value) == false);
    assert(CRedisMgr::GetInstance()->HSet("bloginfo", "blogwebsite", "llfc.club"));
    assert(CRedisMgr::GetInstance()->HGet("bloginfo", "blogwebsite", value));
    assert(CRedisMgr::GetInstance()->ExistsKey("bloginfo"));
    assert(CRedisMgr::GetInstance()->Delete("bloginfo"));
    assert(CRedisMgr::GetInstance()->Delete("bloginfo"));
    assert(CRedisMgr::GetInstance()->ExistsKey("bloginfo") == false);
    assert(CRedisMgr::GetInstance()->LPush("lpushkey1", "lpushvalue1"));
    assert(CRedisMgr::GetInstance()->LPush("lpushkey1", "lpushvalue2"));
    assert(CRedisMgr::GetInstance()->LPush("lpushkey1", "lpushvalue3"));
    assert(CRedisMgr::GetInstance()->RPop("lpushkey1", value));
    assert(CRedisMgr::GetInstance()->RPop("lpushkey1", value));
    assert(CRedisMgr::GetInstance()->LPop("lpushkey1", value));
    assert(CRedisMgr::GetInstance()->LPop("lpushkey2", value) == false);
    CRedisMgr::GetInstance()->Close();
}

void TestGRPC()
{

}

int main()
{
    try
    {
        //test redis
        //TestRedisMgr();

        //test grpc
        TestGRPC();
        return 0;


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
