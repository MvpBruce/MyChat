#pragma once
#include "Const.h"
#include "UserData.h"

class Session;
class TaskNode;
typedef std::function<void(std::shared_ptr<Session>, const short& msgId, const std::string& msgData)> CallBakcFun;
class TaskSystem : public Singleton<TaskSystem>
{
	friend class Singleton<TaskSystem>;

public:
	~TaskSystem();
	void PostMsg(std::shared_ptr<TaskNode> taskNode);

private:
	TaskSystem();
	void LoginHandler(std::shared_ptr<Session> session, const short& msgId, const std::string& msgData);
	void RegisterEvent();
	void ProcessMsg();
	void SearchInfoHandler(std::shared_ptr<Session> session, const short& msgId, const std::string& msgData);
	void AddFriendApply(std::shared_ptr<Session> session, const short& msgId, const std::string& msgData);
	void AuthFriendApply(std::shared_ptr<Session> session, const short& msgId, const std::string& msgData);

	bool GetBaseInfo(std::string strKey, int uid, std::shared_ptr<UserInfo>& pUserInfo);
	void GetUserInfoByID(const std::string& strUid, Json::Value& retValue);
	void GetUserInfoByName(const std::string& name, Json::Value& retValue);
	
private:
	std::unordered_map<short, CallBakcFun> m_Handler;
	std::queue<std::shared_ptr<TaskNode>> m_MsgQueue;
	std::mutex m_Mutex;
	std::condition_variable m_Con;
	std::thread m_WorkThread;
	bool m_bStop;
};


