#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "Singleton.h"

class CVefifyGrpcClient : public Singleton<CVefifyGrpcClient>
{
	friend class Singleton<CVefifyGrpcClient>;
public:

};

