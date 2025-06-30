#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "Singleton.h"
#include <queue>

using message::VerifyService;
using message::GetVerifyReq;
using message::GetVerifyRsp;

class RPCPool;

class CVefifyGrpcClient : public Singleton<CVefifyGrpcClient>
{
	friend class Singleton<CVefifyGrpcClient>;

private:
	CVefifyGrpcClient();

public:
	GetVerifyRsp GetVerifyCode(std::string email);

private:
	std::unique_ptr<RPCPool> m_rpcPool;
};

class RPCPool
{
public:
	RPCPool(std::string host, std::string port, size_t nSize = std::thread::hardware_concurrency())
		:m_bStop(false)
	{
		for (size_t i = 0; i < nSize; ++i)
		{
			m_connections.emplace(VerifyService::NewStub(grpc::CreateChannel(host + ":" + port, grpc::InsecureChannelCredentials())));
		}
	}

	void Stop()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_bStop = true;
		m_cond.notify_all();
		while (m_connections.empty())
			m_connections.pop();
	}

	~RPCPool()
	{
		Stop();
	}

	std::unique_ptr<VerifyService::Stub> GetConnection()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cond.wait(lock, [this]() {
			if (m_bStop)
				return true;

			return !m_connections.empty();
			});

		if (m_bStop)
			return nullptr;

		auto con = std::move(m_connections.front());
		m_connections.pop();
		return std::move(con);
	}

	void ReturnConnection(std::unique_ptr<VerifyService::Stub> con)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		if (m_bStop)
			return;

		m_connections.push(std::move(con));
		m_cond.notify_one();
	}

private:
	std::queue<std::unique_ptr<VerifyService::Stub>> m_connections;
	std::mutex m_mutex;
	std::condition_variable m_cond;
	std::atomic<bool> m_bStop;
};

