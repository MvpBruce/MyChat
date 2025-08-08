#include "StatusGrpcClient.h"
#include "ConfigMgr.h"
#include "Const.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

StatusGrpcClient::StatusGrpcClient()
{
	auto& configMgr = ConfigMgr::GetInstance();
	auto url = configMgr["StatusServer"]["host"];
	auto port = configMgr["StatusServer"]["port"];
	m_statusPool.reset(new StatusPool(url, port, 5));
}

GetChatServerRsp StatusGrpcClient::GetChatServer(int uid)
{
	ClientContext client;
	GetChatServerReq req;
	GetChatServerRsp resp;
	req.set_uid(uid);
	auto con = m_statusPool->GetConnection();
	Defer defer([this, &con] {
		m_statusPool->ReturnConnection(std::move(con));
	});

	Status s = con->GetChatServer(&client, req, &resp);
	if (!s.ok())
		resp.set_error(static_cast<int>(ErrorCodes::Error_RPC));

	return resp;
}
