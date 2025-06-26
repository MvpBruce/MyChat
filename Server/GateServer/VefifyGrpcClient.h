#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "Singleton.h"

using message::VerifyService;
using message::GetVerifyReq;
using message::GetVerifyRsp;

class CVefifyGrpcClient : public Singleton<CVefifyGrpcClient>
{
	friend class Singleton<CVefifyGrpcClient>;

private:
	CVefifyGrpcClient();

public:
	GetVerifyRsp GetVerifyCode(std::string email);

private:
	std::shared_ptr<VerifyService::Stub> m_stub;
};

