#include "StatusServiceImpl.h"
#include <boost/uuid.hpp>
#include "ConfigMgr.h"
#include "Const.h"

std::string GenerateUUID()
{
	boost::uuids::uuid uuid = boost::uuids::random_generator()();
	return to_string(uuid);
}

StatusServiceImpl::StatusServiceImpl()
	: m_nServerIndex(0)
{
	auto& configMgr = ConfigMgr::GetInstance();
	ChatServer server;
	server.host = configMgr["ChatServer1"]["host"];
	server.port = configMgr["ChatServer1"]["port"];
	m_vChatServer.push_back(server);

	server.host = configMgr["ChatServer2"]["host"];
	server.port = configMgr["ChatServer2"]["port"];
	m_vChatServer.push_back(server);
}

Status StatusServiceImpl::GetChatServer(ServerContext* context, const GetChatServerReq* request, GetChatServerRsp* response)
{
	m_nServerIndex = (m_nServerIndex++) % (m_vChatServer.size());
	auto& server = m_vChatServer[m_nServerIndex];
	response->set_host(server.host);
	response->set_port(server.port);
	response->set_error(static_cast<int>(ErrorCodes::Success));
	response->set_token(GenerateUUID());
	return Status();
}
