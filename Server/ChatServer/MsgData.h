#pragma once
class TaskSystem;
class MsgData
{
public:
	MsgData(short maxLen);
	~MsgData();

	void Clear();

	short m_cur;
	short m_totalLen;
	char* m_pData;
};

class RecvData : public MsgData
{
	friend class TaskSystem;
public:
	RecvData(short maxLen, short msgId);
private:
	short m_msgId;
};

class SendData : public MsgData
{
	friend class TaskSystem;
public:
	SendData(const char* data, short maxLen, short msgId);
private:
	short m_msgId;
};
