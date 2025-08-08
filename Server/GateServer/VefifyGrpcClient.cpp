#include "Const.h"
#include "VefifyGrpcClient.h"
#include "ConfigMgr.h"


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

CVefifyGrpcClient::CVefifyGrpcClient()
{
	auto& configMgr = ConfigMgr::GetInstance();
	auto url = configMgr["VerifyServer"]["host"];
	auto port = configMgr["VerifyServer"]["port"];
	m_rpcPool.reset(new RPCPool(url, port, 5));
}

GetVerifyRsp CVefifyGrpcClient::GetVerifyCode(std::string email)
{
	ClientContext client;
	GetVerifyReq req;
	GetVerifyRsp resp;

	req.set_email(email.c_str());
	auto con = m_rpcPool->GetConnection();
	Defer defer([this, &con] {
		m_rpcPool->ReturnConnection(std::move(con));
	});

	Status s = con->GetVerifyCode(&client, req, &resp);
	if (!s.ok())
		resp.set_error(static_cast<int>(ErrorCodes::Error_RPC));

	return resp;
}
