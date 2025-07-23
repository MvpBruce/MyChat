#include "Const.h"
#include "VefifyGrpcClient.h"


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

CVefifyGrpcClient::CVefifyGrpcClient()
{
	m_rpcPool.reset(new RPCPool("127.0.0.1", "50052", 5));
}

GetVerifyRsp CVefifyGrpcClient::GetVerifyCode(std::string email)
{
	ClientContext client;
	GetVerifyReq req;
	GetVerifyRsp resp;

	req.set_email(email.c_str());
	auto con = m_rpcPool->GetConnection();
	Status s = con->GetVerifyCode(&client, req, &resp);
	if (!s.ok())
		resp.set_error(static_cast<int>(ErrorCodes::Error_RPC));

	m_rpcPool->ReturnConnection(std::move(con));
	return resp;
}
