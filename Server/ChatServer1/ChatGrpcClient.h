#pragma once
#include "message.grpc.pb.h"
#include "message.pb.h"
#include <grpcpp/grpcpp.h>
#include <queue>
#include "Singleton.h"

using grpc::ClientContext;
using grpc::Status;

using message::ChatService;
using message::AddFriendReq;
using message::AddFriendRsp;
using message::AuthFriendReq;
using message::AuthFriendRsp;

using grpc::Channel;

class ChatPool
{
public:
	ChatPool(std::string host, std::string port, size_t nSize = std::thread::hardware_concurrency())
		: m_bStop(false)
	{
		for (size_t i = 0; i < nSize; i++)
		{
			std::shared_ptr<Channel> pChannel = grpc::CreateChannel(host + ":" + port, grpc::InsecureChannelCredentials());
			m_connections.emplace(ChatService::NewStub(pChannel));
		}
	}

	~ChatPool()
	{
		Stop();
	}

	void Stop()
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		m_bStop = true;
		m_Con.notify_all();
		while (!m_connections.empty())
			m_connections.pop();
	}

	std::unique_ptr<ChatService::Stub> GetConnection()
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_Con.wait(lock, [this]() {
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

	void ReturnConnection(std::unique_ptr<ChatService::Stub> con)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		if (m_bStop)
			return;

		m_connections.push(std::move(con));
		m_Con.notify_one();
	}

private:
	std::atomic<bool> m_bStop;
	std::queue<std::unique_ptr<ChatService::Stub>> m_connections;
	std::mutex m_Mutex;
	std::condition_variable m_Con;
};

class ChatGrpcClient : public Singleton<ChatGrpcClient>
{
	friend class Singleton<ChatGrpcClient>;
public:
	AddFriendRsp NotifyAddFriend(std::string strSeverAddress, const AddFriendReq& req);
	AuthFriendRsp NotifyAuthFriend(std::string strSeverAddress, const AuthFriendReq& req);

private:
	ChatGrpcClient();

private:
	std::unordered_map<std::string, std::unique_ptr<ChatPool>> m_chatPool;
};

