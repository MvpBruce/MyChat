#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"

using grpc::Status;
using grpc::ServerContext;

using message::StatusService;
using message::GetChatServerReq;
using message::GetChatServerRsp;

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

private:
	std::vector<ChatServer> m_Servers;
	int m_nServerIndex;
};

