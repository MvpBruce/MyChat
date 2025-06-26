#include "Const.h"
#include "VefifyGrpcClient.h"


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

CVefifyGrpcClient::CVefifyGrpcClient()
{
	std::shared_ptr<Channel> channel = grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials());
	m_stub = VerifyService::NewStub(channel);
}

GetVerifyRsp CVefifyGrpcClient::GetVerifyCode(std::string email)
{
	ClientContext client;
	GetVerifyReq req;
	GetVerifyRsp resp;

	req.set_email(email.c_str());
	Status s = m_stub->GetVerifyCode(&client, req, &resp);
	if (!s.ok())
		resp.set_error(static_cast<int>(ErrorCodes::Error_RPC));

	return resp;
}
