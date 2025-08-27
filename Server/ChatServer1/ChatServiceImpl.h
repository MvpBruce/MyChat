#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"

using message::ChatService;

class ChatServiceImpl final : public ChatService::Service
{
public:
	ChatServiceImpl();
};

