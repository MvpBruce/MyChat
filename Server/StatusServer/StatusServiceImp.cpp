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
	: m_nServerIndex(0)
{
	auto& configMgr = ConfigMgr::GetInstance();
	ChatServer server;
	server.host = configMgr["ChatServer1"]["host"];
	server.port = configMgr["ChatServer1"]["port"];
	m_Servers.push_back(server);

	server.host = configMgr["ChatServer2"]["host"];
	server.port = configMgr["ChatServer2"]["port"];
	m_Servers.push_back(server);
}

Status StatusServiceImp::GetChatServer(ServerContext* context, const GetChatServerReq* request, GetChatServerRsp* response)
{
	//m_nServerIndex = (m_nServerIndex++) % (m_Servers.size());
	m_nServerIndex = 0;
	auto& server = m_Servers[m_nServerIndex];
	response->set_host(server.host);
	response->set_port(server.port);
	response->set_error(static_cast<int>(ErrorCodes::Success));
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
