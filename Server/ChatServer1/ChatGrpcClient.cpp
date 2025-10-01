#include "ChatGrpcClient.h"
#include "ConfigMgr.h"
#include <sstream>
#include "Const.h"

AddFriendRsp ChatGrpcClient::NotifyAddFriend(std::string strSeverAddress, const AddFriendReq& req)
{
	AddFriendRsp rsp;
	rsp.set_error(ErrorCodes::Success);
	Defer defer([&rsp, &req]() {
		rsp.set_applyuid(req.applyuid());
		rsp.set_touid(req.touid());
	});

	auto it = m_chatPool.find(strSeverAddress);
	if (it == m_chatPool.end())
		return rsp;

	auto& pool = it->second;
	ClientContext context;
	auto stub = pool->GetConnection();
	Status status = stub->NotifyAddFriend(&context, req, &rsp);
	Defer conDefer([&stub, this, &pool]() {
		pool->ReturnConnection(std::move(stub));
	});

	if (!status.ok())
		rsp.set_error(ErrorCodes::Error_RPC);

	return rsp;
}

AuthFriendRsp ChatGrpcClient::NotifyAuthFriend(std::string strSeverAddress, const AuthFriendReq& req)
{
	AuthFriendRsp rsp;
	rsp.set_error(ErrorCodes::Success);
	Defer defer([&rsp, &req]() {
		rsp.set_fromuid(req.fromuid());
		rsp.set_touid(req.touid());
	});

	auto it = m_chatPool.find(strSeverAddress);
	if (it == m_chatPool.end())
		return rsp;

	auto& pool = it->second;
	ClientContext context;
	auto stub = pool->GetConnection();
	Status status = stub->NotifyAuthFriend(&context, req, &rsp);
	Defer conDefer([&stub, this, &pool]() {
		pool->ReturnConnection(std::move(stub));
	});

	if (!status.ok())
	{
		rsp.set_error(ErrorCodes::Error_RPC);
		return rsp;
	}

	return rsp;
}

ChatTextMsgRsp ChatGrpcClient::NotifyChatTextMsg(std::string strSeverAddress, const ChatTextMsgReq& req)
{
	ChatTextMsgRsp rsp;
	rsp.set_error(ErrorCodes::Success);
	Defer defer([&rsp, &req]() {
		rsp.set_fromuid(req.fromuid());
		rsp.set_touid(req.touid());
		for (const auto& textData : req.textmsgs())
		{
			auto pChatData = rsp.add_textmsgs();
			pChatData->set_msgid(textData.msgid());
			pChatData->set_msgcontent(textData.msgcontent());
		}
	});

	auto it = m_chatPool.find(strSeverAddress);
	if (it == m_chatPool.end())
		return rsp;

	auto& pool = it->second;
	ClientContext context;
	auto stub = pool->GetConnection();
	Status status = stub->NotifyChatTextMsg(&context, req, &rsp);
	Defer conDefer([&stub, this, &pool]() {
		pool->ReturnConnection(std::move(stub));
	});

	if (!status.ok())
	{
		rsp.set_error(ErrorCodes::Error_RPC);
		return rsp;
	}

	return rsp;
}

ChatGrpcClient::ChatGrpcClient()
{
	auto& config = ConfigMgr::GetInstance();
	auto serverList = config["PeerServer"]["Servers"];

	std::stringstream ss(serverList);
	std::vector<std::string> serverNames;
	std::string strName;
	while (std::getline(ss, strName, ','))
		serverNames.push_back(strName);

	for (auto& name : serverNames)
	{
		m_chatPool.emplace(config[name]["name"], std::make_unique<ChatPool>(config[name]["host"], config[name]["port"]));
	}
}
