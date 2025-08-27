#include "StatusServiceImp.h"
#include <boost/uuid.hpp>
#include "ConfigMgr.h"
#include "Const.h"
#include "RedisMgr.h"

std::string GenerateUUID()
{
	boost::uuids::uuid uuid = boost::uuids::random_generator()();
	return to_string(uuid);
}

StatusServiceImp::StatusServiceImp()
{
	auto& configMgr = ConfigMgr::GetInstance();
	auto serverList = configMgr["ServerList"]["Servers"];
	std::vector<std::string> vecName;
	std::stringstream ss(serverList);
	std::string strName;
	while (std::getline(ss, strName, ','))
	{
		vecName.push_back(strName);
	}

	for (auto& name : vecName)
	{
		ChatServer chatServer;
		chatServer.host = configMgr[name]["host"];
		chatServer.port = configMgr[name]["port"];
		chatServer.name = name;

		m_Servers[name] = chatServer;
	}
}

Status StatusServiceImp::GetChatServer(ServerContext* context, const GetChatServerReq* request, GetChatServerRsp* response)
{
	const auto& chatServer = GetChatServer();
	response->set_host(chatServer.host);
	response->set_port(chatServer.port);
	response->set_error(ErrorCodes::Success);
	response->set_token(GenerateUUID());
	InsertToken(request->uid(), response->token());
	return Status::OK;
}

void StatusServiceImp::InsertToken(int uid, std::string token)
{
	std::string strUid = std::to_string(uid);
	std::string token_key = UTOKENPREFIX;
	token_key += strUid;
	CRedisMgr::GetInstance()->Set(token_key, token);
}

Status StatusServiceImp::Login(ServerContext* context, const LoginReq* request, LoginRsp* response)
{
	auto uid = request->uid();
	auto token = request->token();

	std::string strUId = std::to_string(uid);
	std::string token_key = UTOKENPREFIX;
	token_key += strUId;
	std::string token_val = "";
	bool ret = CRedisMgr::GetInstance()->Get(token_key, token_val);
	if (!ret)
	{
		response->set_error(static_cast<int>(ErrorCodes::Uid_Invalid));
		return Status::OK;
	}

	if (token != token_val)
	{
		response->set_error(static_cast<int>(ErrorCodes::Token_Invalid));
		return Status::OK;
	}

	response->set_error(static_cast<int>(ErrorCodes::Success));
	response->set_uid(uid);
	response->set_token(token);
	return Status::OK;
}

ChatServer StatusServiceImp::GetChatServer()
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	auto minServer = m_Servers.begin()->second;
	std::string strCount = "";
	bool bRet = CRedisMgr::GetInstance()->HGet(LOGIN_NUMBER, minServer.name, strCount);
	if (!bRet)
		minServer.conNums = INT_MAX;
	else
		minServer.conNums = std::stoi(strCount);

	for (auto& server : m_Servers)
	{
		if (server.second.name == minServer.name)
			continue;

		bRet = CRedisMgr::GetInstance()->HGet(LOGIN_NUMBER, server.second.name, strCount);
		if (!bRet)
			server.second.conNums = INT_MAX;
		else
			server.second.conNums = std::stoi(strCount);

		if (server.second.conNums < minServer.conNums)
			minServer = server.second;
	}
	
	return minServer;
}
