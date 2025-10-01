#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "message.pb.h"
#include "UserData.h"

using grpc::ServerContext;
using grpc::Status;

using message::ChatService;
using message::AddFriendReq;
using message::AddFriendRsp;
using message::AuthFriendReq;
using message::AuthFriendRsp;
using message::ChatTextMsgReq;
using message::ChatTextMsgRsp;

class ChatServiceImpl final : public ChatService::Service
{
public:
	ChatServiceImpl();
	Status NotifyAddFriend(ServerContext* context, const AddFriendReq* req, AddFriendRsp* rsp) override;
	Status NotifyAuthFriend(ServerContext* context, const AuthFriendReq* req, AuthFriendRsp* rsp) override;
	Status NotifyChatTextMsg(ServerContext* context, const ChatTextMsgReq* req, ChatTextMsgRsp* rsp) override;

private:
	bool GetBaseInfo(std::string strKey, int uid, std::shared_ptr<UserInfo>& pUserInfo);
};

