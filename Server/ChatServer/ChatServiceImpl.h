#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "message.pb.h"

using grpc::ServerContext;
using grpc::Status;

using message::ChatService;
using message::AddFriendReq;
using message::AddFriendRsp;

class ChatServiceImpl final : public ChatService::Service
{
public:
	ChatServiceImpl();
	Status NotifyAddFriend(ServerContext* context, const AddFriendReq* req, AddFriendRsp* rsp) override;

};

