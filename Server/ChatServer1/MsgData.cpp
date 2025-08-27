#include "MsgData.h"
#include "Const.h"

MsgData::MsgData(short maxLen) :
	m_cur(0), m_totalLen(maxLen)
{
	m_pData = new char[m_totalLen + 1];
	m_pData[m_totalLen] = '\0';
}

MsgData::~MsgData()
{
	delete[] m_pData;
	m_pData = nullptr;
}

void MsgData::Clear()
{
	memset(m_pData, 0, m_totalLen);
	m_cur = 0;
}


/*  class RecvData*/
RecvData::RecvData(short maxLen, short msgId):
	MsgData(maxLen), m_msgId(msgId)
{

}

/*  class SendData*/
SendData::SendData(const char* data, short maxLen, short msgId) :
	MsgData(maxLen + HEAD_MAX_LEN), m_msgId(msgId)
{
	//Convert to network format
	short newWorkMsgId = boost::asio::detail::socket_ops::host_to_network_short(msgId);
	memcpy(m_pData, &newWorkMsgId, HEAD_ID_LEN);

	short newWorkMaxLen = boost::asio::detail::socket_ops::host_to_network_short(maxLen);
	memcpy(m_pData + HEAD_ID_LEN, &newWorkMaxLen, HEAD_DATA_LEN);
	memcpy(m_pData + HEAD_ID_LEN + HEAD_DATA_LEN, data, maxLen);
}


