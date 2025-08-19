#include "Session.h"
#include <iostream>
#include "TaskSystem.h"
#include "Utils.h"
#include "Server.h"
#include "MsgData.h"

Session::Session(net::io_context& ioc, CServer* pServer):
m_socket(ioc), m_timer(ioc, std::chrono::seconds(60)), m_pServer(pServer), m_headData(std::make_shared<MsgData>(HEAD_MAX_LEN)), m_recvData(nullptr)
{
	boost::uuids::uuid uid = boost::uuids::random_generator()();
	m_SessionID = to_string(uid);
}

void Session::Start()
{
	AsyncReadHead(HEAD_MAX_LEN);
}

void Session::Close()
{
	m_socket.close();
}

tcp::socket& Session::GetSocket()
{
	return m_socket;
}

std::string& Session::GetSessionId()
{
	return m_SessionID;
}

void Session::AsyncReadHead(int nTotalLen)
{
	auto self = shared_from_this();
	AsyncReadFull(HEAD_MAX_LEN, [self, this](const boost::system::error_code& ec, std::size_t bytes_transfered) {
		try
		{
			if (ec)
			{
				std::cout << "Read failed: " << ec.what() << std::endl;
				Close();
				m_pServer->ClearSession(m_SessionID);
				return;
			}

			if (bytes_transfered < HEAD_MAX_LEN)
			{
				std::cout << "Full data not match, read: " << bytes_transfered << ", total: " << HEAD_DATA_LEN << std::endl;
				Close();
				m_pServer->ClearSession(m_SessionID);
				return;
			}

			//Read full head data
			m_headData->Clear();
			memcpy(m_headData->m_pData, m_data, bytes_transfered);

			short nMsgId = 0;
			memcpy(&nMsgId, m_headData->m_pData, HEAD_ID_LEN);
			nMsgId = boost::asio::detail::socket_ops::network_to_host_short(nMsgId);
			std::cout << "Msg id: " << nMsgId << std::endl;
			//illegal id
			if (nMsgId > MAX_LEN)
			{
				std::cout << "Invalid msg id: " << nMsgId << std::endl;
				m_pServer->ClearSession(m_SessionID);
				return;
			}

			short nMsgLen = 0;
			memcpy(&nMsgLen, m_headData->m_pData + HEAD_ID_LEN, HEAD_DATA_LEN);
			nMsgLen = boost::asio::detail::socket_ops::network_to_host_short(nMsgLen);
			std::cout << "Msg Length: " << nMsgLen << std::endl;
			if (nMsgLen > MAX_LEN)
			{
				std::cout << "Invalid msg length: " << nMsgLen << std::endl;
				m_pServer->ClearSession(m_SessionID);
				return;
			}

			//Construct for data body
			m_recvData = std::make_shared<RecvData>(nMsgLen, nMsgId);
			AsyncReadBody(nMsgLen);
		}
		catch (std::exception& e)
		{
			std::cout << "Exception occured in AsyncReadHead: " << e.what() << std::endl;
		}
	});
}

void Session::AsyncReadFull(std::size_t nMaxLen, std::function<void(const boost::system::error_code&, std::size_t)> handler)
{
	memset(m_data, 0, sizeof(char) * MAX_LEN);
	AsyncReadLen(0, nMaxLen, handler);
}

void Session::AsyncReadLen(std::size_t read_len, std::size_t total_len, std::function<void(const boost::system::error_code&, std::size_t)> handler)
{
	auto self = shared_from_this();
	m_socket.async_read_some(boost::asio::buffer(m_data + read_len, total_len - read_len), [self, read_len, total_len, handler](const boost::system::error_code& ec, std::size_t bytes_transferd) {
		if (ec)
		{
			//error happened, call callback function
			handler(ec, read_len + bytes_transferd);
			return;
		}

		if (read_len + bytes_transferd >= total_len)
		{
			//Read data enough
			handler(ec, read_len + bytes_transferd);
			return;
		}

		//Not enough lengh, continue to read
		self->AsyncReadLen(read_len + bytes_transferd, total_len, handler);
	});
}

void Session::AsyncReadBody(std::size_t totalLen)
{
	auto self = shared_from_this();
	AsyncReadFull(totalLen, [self, this, totalLen](const boost::system::error_code& ec, std::size_t bytes_transfered) {
		try
		{
			if (ec)
			{
				std::cout << "Read body failed: " << ec.what() << std::endl;
				Close();
				m_pServer->ClearSession(m_SessionID);
				return;
			}

			if (bytes_transfered < totalLen)
			{
				std::cout << "Read length not match, read: " << bytes_transfered << ", total: " << totalLen << std::endl;
				Close();
				m_pServer->ClearSession(m_SessionID);
				return;
			}

			memcpy(m_recvData->m_pData, m_data, bytes_transfered);
			m_recvData->m_cur += bytes_transfered;
			m_recvData->m_pData[m_recvData->m_totalLen] = '\0';
			std::cout << "Received data: " << m_recvData->m_pData << std::endl;
			//Post msg to logic queue, todo
			TaskSystem::GetInstance()->PostMsg(std::make_shared<TaskNode>(self, m_recvData));
			
			//Continue to read next head
			AsyncReadHead(HEAD_MAX_LEN);
		}
		catch (std::exception& e)
		{
			std::cout << "Exception occured in AsyncReadBody: " << e.what() << std::endl;
		}
	});
}

void Session::Send(std::string msg, short msgId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	int nSize = m_sendQueue.size();
	if (nSize > MAX_SEND_QUEUE)
	{
		std::cout << "Session id: " << m_SessionID << ", Msg queue is full, size: " << nSize << std::endl;
		return;
	}

	m_sendQueue.emplace(std::make_shared<SendData>(msg.c_str(), static_cast<short>(msg.length()), msgId));
	if (nSize > 0)
		return;

	auto& msgNode = m_sendQueue.front();
	boost::asio::async_write(m_socket, boost::asio::buffer(msgNode->m_pData, msgNode->m_totalLen), 
		std::bind(&Session::ProcessWrite, this, std::placeholders::_1, std::placeholders::_2, shared_from_this()));
}

void Session::ProcessWrite(boost::system::error_code ec, size_t nSize, std::shared_ptr<Session> session)
{
	try
	{
		if (ec)
		{
			std::cout << "Process write failed: " << ec.what() << std::endl;
			m_pServer->ClearSession(m_SessionID);
		}
		else
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			//Remove current node that has been processed
			m_sendQueue.pop();
			//Not empty, continue to write
			if (!m_sendQueue.empty())
			{
				auto& msgNode = m_sendQueue.front();
				boost::asio::async_write(m_socket, boost::asio::buffer(msgNode->m_pData, msgNode->m_totalLen),
					std::bind(&Session::ProcessWrite, this, std::placeholders::_1, std::placeholders::_2, session));
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in ProcessWrite: " << e.what() << std::endl;
	}
}


/*****  Class TaskNode  ******/
TaskNode::TaskNode(std::shared_ptr<Session> session, std::shared_ptr<RecvData> recvData)
	: m_Session(session), m_RecvData(recvData)
{

}

