#pragma once
#include "Const.h"

class CServer;
class MsgData;
class RecvData;
class SendData;
class TaskSystem;
class Session : public std::enable_shared_from_this<Session>
{
public:
	Session(net::io_context& ioc, CServer* pServer);
	void Start();
	tcp::socket& GetSocket();
	std::string& GetSessionId();
	void Close();
	void Send(std::string msg, short msgId);

private:
	//read
	void AsyncReadHead(int nTotalLen);
	void AsyncReadFull(std::size_t nMaxLen, std::function<void(const boost::system::error_code&, std::size_t)> handler);
	void AsyncReadLen(std::size_t read_len, std::size_t total_len, std::function<void(const boost::system::error_code&, std::size_t)> handler);
	void AsyncReadBody(std::size_t totalLen);

	//write
	void ProcessWrite(boost::system::error_code ec, size_t nSize, std::shared_ptr<Session> session);

private:
	tcp::socket m_socket;
	net::steady_timer m_timer;
	//new add
	std::string m_SessionID;
	char m_data[MAX_LEN];
	CServer* m_pServer;
	//Head data
	std::shared_ptr<MsgData> m_headData;
	//Message data
	std::shared_ptr<RecvData> m_recvData;
	std::queue<std::shared_ptr<SendData>> m_sendQueue;
	std::mutex m_mutex;
};

class TaskNode {
	friend class TaskSystem;
public:
	TaskNode(std::shared_ptr<Session> session, std::shared_ptr<RecvData> recvData);

private:
	std::shared_ptr<Session> m_Session;
	std::shared_ptr<RecvData> m_RecvData;
};
