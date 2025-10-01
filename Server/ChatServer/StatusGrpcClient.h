#pragma once
#include "Singleton.h"
#include <queue>
#include "message.grpc.pb.h"
#include <grpcpp/grpcpp.h>

using message::StatusService;
using message::GetChatServerReq;
using message::GetChatServerRsp;

using message::LoginReq;
using message::LoginRsp;

using grpc::Channel;

class StatusPool;
class StatusGrpcClient : public Singleton<StatusGrpcClient>
{
	friend class Singleton<StatusGrpcClient>;

private:
	StatusGrpcClient();

public:
	GetChatServerRsp GetChatServer(int uid);
	LoginRsp Login(int uid, std::string token);

private:
	std::unique_ptr<StatusPool> m_statusPool;
};

class StatusPool
{
public:
	StatusPool(std::string& host, std::string& port, size_t nSize = std::thread::hardware_concurrency())
		: m_bStop(false)
	{
		for (size_t i = 0; i < nSize; ++i)
		{
			m_connections.emplace(StatusService::NewStub(grpc::CreateChannel(host + ":" + port, grpc::InsecureChannelCredentials())));
		}
	}

	void Stop()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_bStop = true;
		m_cond.notify_all();
		while (!m_connections.empty())
			m_connections.pop();
	}

	std::unique_ptr<StatusService::Stub> GetConnection()
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

	void ReturnConnection(std::unique_ptr<StatusService::Stub> con)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		if (m_bStop)
			return;

		m_connections.push(std::move(con));
		m_cond.notify_one();
	}

	~StatusPool()
	{
		Stop();
	}

private:
	std::queue<std::unique_ptr<StatusService::Stub>> m_connections;
	std::mutex m_mutex;
	std::condition_variable m_cond;
	std::atomic<bool> m_bStop;
};

