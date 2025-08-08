#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"

using message::StatusService;
using message::GetChatServerReq;
using message::GetChatServerRsp;

using grpc::Status;
using grpc::ServerContext;

struct ChatServer
{
	std::string host;
	std::string port;
};

class StatusServiceImpl final : public StatusService::Service
{
public:
	StatusServiceImpl();

	Status GetChatServer(ServerContext* context, const GetChatServerReq* request, GetChatServerRsp* response) override;

	std::vector<ChatServer> m_vChatServer;
	int m_nServerIndex;
};

