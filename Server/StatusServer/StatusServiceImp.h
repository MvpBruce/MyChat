#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"

using grpc::Status;
using grpc::ServerContext;

using message::StatusService;
using message::GetChatServerReq;
using message::GetChatServerRsp;
using message::LoginReq;
using message::LoginRsp;

struct ChatServer
{
	std::string host;
	std::string port;
};

class StatusServiceImp final : public StatusService::Service
{
public:
	StatusServiceImp();
	Status GetChatServer(ServerContext* context, const GetChatServerReq* request, GetChatServerRsp* response) override;
	Status Login(ServerContext* context, const LoginReq* request, LoginRsp* response) override;

private:
	void InsertToken(int uid, std::string token);
	std::vector<ChatServer> m_Servers;
	int m_nServerIndex;
};

