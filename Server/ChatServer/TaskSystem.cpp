#include "TaskSystem.h"
#include "Session.h"
#include "StatusGrpcClient.h"
#include "MsgData.h"
#include "UserMgr.h"
#include "RedisMgr.h"
#include "ConfigMgr.h"
#include "MySqlMgr.h"

TaskSystem::TaskSystem(): m_bStop(false)
{
	RegisterEvent();
	m_WorkThread = std::thread(&TaskSystem::ProcessMsg, this);
}

TaskSystem::~TaskSystem()
{
	m_bStop = true;
	m_Con.notify_all();
	m_WorkThread.join();
}

void TaskSystem::RegisterEvent()
{
	m_Handler[MSG_IDS::MSG_CHAT_LOGIN] = std::bind(&TaskSystem::LoginHandler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	m_Handler[MSG_IDS::SEARCH_USER_REQ] = std::bind(&TaskSystem::SearchInfoHandler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	m_Handler[MSG_IDS::ADD_FRIEND_REQ] = std::bind(&TaskSystem::AddFriendApply, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	m_Handler[MSG_IDS::AUTH_FRIEND_REQ] = std::bind(&TaskSystem::AuthFriendApply, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

}

void TaskSystem::LoginHandler(std::shared_ptr<Session> session, const short& msgId, const std::string& msgData)
{
	Json::Reader reader;
	Json::Value root;
	reader.parse(msgData, root);
	auto uid = root["uid"].asInt();
	auto token = root["token"].asString();
	std::cout << "User login uid: " << uid  << ", token: " << token << std::endl;

	//Verify token from status server
	auto rsp = StatusGrpcClient::GetInstance()->Login(uid, token);

	Json::Value retValue;
	Defer defer([this, session, &retValue]() {
		session->Send(retValue.toStyledString(), static_cast<short>(MSG_IDS::MSG_CHAT_LOGIN_RSP));
	});

	//Get token from redis
	std::string strUid = std::to_string(uid);
	std::string strTokenKey = UTOKENPREFIX;
	strTokenKey += strUid;
	std::string strValue = "";
	bool bRet = CRedisMgr::GetInstance()->Get(strTokenKey, strValue);
	if (!bRet)
	{
		retValue["error"] = ErrorCodes::Uid_Invalid;
		return;
	}

	if (strValue != token)
	{
		retValue["error"] = ErrorCodes::Token_Invalid;
		return;
	}

	retValue["error"] = ErrorCodes::Success;

	auto pUserInfo = std::make_shared<UserInfo>();
	std::string strBaseInfo = USERBASEINFO;
	strBaseInfo += strUid;
	bRet = GetBaseInfo(strBaseInfo, uid, pUserInfo);
	if (!bRet)
	{
		retValue["error"] = ErrorCodes::Uid_Invalid;
		return;
	}

	retValue["uid"] = uid;
	retValue["pwd"] = pUserInfo->pwd;
	retValue["name"] = pUserInfo->name;
	retValue["email"] = pUserInfo->email;
	retValue["nick"] = pUserInfo->nick;
	retValue["desc"] = pUserInfo->desc;
	retValue["gender"] = pUserInfo->gender;
	retValue["icon"] = pUserInfo->icon;
	retValue["token"] = token;

	//get apply list from mysql
	std::vector<std::shared_ptr<ApplyInfo>> vecApplyInfo;
	bRet = MySqlMgr::GetInstance()->GetApplyList(uid, vecApplyInfo, 0, 10);
	if (bRet)
	{
		for (auto& apply : vecApplyInfo)
		{
			Json::Value applyObj;
			applyObj["name"] = apply->name;
			applyObj["uid"] = apply->uid;
			applyObj["icon"] = apply->icon;
			applyObj["nick"] = apply->nick;
			applyObj["gender"] = apply->gender;
			applyObj["desc"] = apply->desc;
			applyObj["status"] = apply->status;
			retValue["apply_list"].append(applyObj);
		}
	}

	//get contact list
	std::vector<std::shared_ptr<UserInfo>> vecFriendInfo;
	bRet = MySqlMgr::GetInstance()->GetFriendList(uid, vecFriendInfo);
	if (bRet)
	{
		for (auto& friendInfo : vecFriendInfo) {
			Json::Value obj;
			obj["name"] = friendInfo->name;
			obj["uid"] = friendInfo->uid;
			obj["icon"] = friendInfo->icon;
			obj["nick"] = friendInfo->nick;
			obj["sex"] = friendInfo->gender;
			obj["desc"] = friendInfo->desc;
			//obj["back"] = friendInfo->back;
			retValue["friend_list"].append(obj);
		}
	}

	auto serverName = ConfigMgr::GetInstance()["CurrentServer"]["name"];
	bRet = CRedisMgr::GetInstance()->HGet(LOGIN_NUMBER, serverName, strValue);
	int nLoginNumber = 0;
	if (bRet && !strValue.empty())
		nLoginNumber = std::stoi(strValue);

	nLoginNumber++;
	std::string strNumber = std::to_string(nLoginNumber);
	CRedisMgr::GetInstance()->HSet(LOGIN_NUMBER, serverName, strNumber);
	session->SetUserId(uid);
	//bind uid and server name
	std::string strIpKey = USERIPPREFIX;
	strIpKey += strUid;
	CRedisMgr::GetInstance()->Set(strIpKey, serverName);
	UserMgr::GetInstance()->SetSession(uid, session);
}

void TaskSystem::PostMsg(std::shared_ptr<TaskNode> taskNode)
{
	std::unique_lock<std::mutex> lock(m_Mutex);
	m_MsgQueue.push(taskNode);
	if (m_MsgQueue.size() == 1)
	{
		lock.unlock();
		m_Con.notify_one();
	}
}

void TaskSystem::ProcessMsg()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_Con.wait(lock, [this]() {
			if (m_bStop)
				return true;

			return !m_MsgQueue.empty();
		});

		//Process all tasks when getting stop
		if (m_bStop)
		{
			while (!m_MsgQueue.empty())
			{
				auto& msgNode = m_MsgQueue.front();
				auto it = m_Handler.find(msgNode->m_RecvData->m_msgId);
				if (it == m_Handler.end())
				{
					m_MsgQueue.pop();
					continue;
				}
				//todo? copy recvdata to string? need curlen?
				it->second(msgNode->m_Session, msgNode->m_RecvData->m_msgId, msgNode->m_RecvData->m_pData);
				m_MsgQueue.pop();
				break;
			}
		}

		//Normal way
		auto& msgNode = m_MsgQueue.front();
		auto it = m_Handler.find(msgNode->m_RecvData->m_msgId);
		if (it == m_Handler.end())
		{
			std::cout << "Can't find msg id: " << msgNode->m_RecvData->m_msgId << " in TaskSystem." << std::endl;
			m_MsgQueue.pop();
			continue;
		}
		//todo? copy recvdata to string? need curlen?
		it->second(msgNode->m_Session, msgNode->m_RecvData->m_msgId, msgNode->m_RecvData->m_pData);
		m_MsgQueue.pop();
	}
}

void TaskSystem::SearchInfoHandler(std::shared_ptr<Session> session, const short& msgId, const std::string& msgData)
{
	Json::Reader reader;
	Json::Value root;
	reader.parse(msgData, root);
	auto strUid = root["uid"].asString();
	std::cout << "User search uid: " << strUid << std::endl;

	Json::Value retValue;
	Defer defer([this, &retValue, session]() {
		session->Send(retValue.toStyledString(), MSG_IDS::SEARCH_USER_RSP);
	});

	bool bDigit = std::all_of(strUid.begin(), strUid.end(), [](unsigned char ch) {return std::isdigit(ch); });
	if (bDigit)
		GetUserInfoByID(strUid, retValue);
	else
		GetUserInfoByName(strUid, retValue);
}

void TaskSystem::AddFriendApply(std::shared_ptr<Session> session, const short& msgId, const std::string& msgData)
{
	Json::Reader reader;
	Json::Value root;
	reader.parse(msgData, root);
	auto uid = root["uid"].asInt();
	auto applyname = root["applyname"].asString();
	auto bakname = root["bakname"].asString();
	auto touid = root["touid"].asInt();
	std::cout << "uid: " << uid << " applyname: "
		<< applyname << " bakname: " << bakname << " touid: " << touid << std::endl;

	Json::Value retValue;
	retValue["error"] = ErrorCodes::Success;
	Defer defer([this, &retValue, session]() {
		session->Send(retValue.toStyledString(), MSG_IDS::ADD_FRIEND_RSP);
	});

	//Add to mysql
	MySqlMgr::GetInstance()->AddFriendApply(uid, touid);

	//Get to_uid's server from redis
	std::string toIpKey = USERIPPREFIX;
	toIpKey += std::to_string(touid);
	std::string toServerName = "";
	bool bRet = CRedisMgr::GetInstance()->Get(toIpKey, toServerName);
	if (!bRet)
		return;

	auto cfg = ConfigMgr::GetInstance();
	auto selfName = cfg["CurrentServer"]["name"];
	std::string strBaseKey = USERBASEINFO;
	strBaseKey += std::to_string(uid);
	auto pInfo = std::make_shared<UserInfo>();
	bRet = GetBaseInfo(strBaseKey, uid, pInfo);
	if (selfName == toServerName)
	{
		auto session = UserMgr::GetInstance()->GetSession(touid);
		if (session)
		{
			Json::Value notifyValue;
			notifyValue["error"] = ErrorCodes::Success;
			notifyValue["applyuid"] = uid;
			notifyValue["name"] = applyname;
			notifyValue["desc"] = "";
			if (bRet)
			{
				notifyValue["icon"] = pInfo->icon;
				notifyValue["gender"] = pInfo->gender;
				notifyValue["nick"] = pInfo->nick;
			}

			//Send to target user's client
			session->Send(notifyValue.toStyledString(), MSG_IDS::ADD_FRIEND_NOTIFY);
		}

		return;
	}

	//Users are in different chat server, need to use grpc
	
}

void TaskSystem::AuthFriendApply(std::shared_ptr<Session> session, const short& msgId, const std::string& msgData)
{
	Json::Reader reader;
	Json::Value root;
	reader.parse(msgData, root);

	auto uid = root["fromuid"].asInt();
	auto touid = root["touid"].asInt();
	auto back_name = root["back"].asString();
	std::cout << "from " << uid << " auth friend to " << touid << std::endl;

	Json::Value retValue;
	retValue["error"] = ErrorCodes::Success;
	auto pUserInfo = std::make_shared<UserInfo>();
	
	std::string strKey = USERBASEINFO;
	strKey += std::to_string(touid);
	bool bRet = GetBaseInfo(strKey, touid, pUserInfo);
	if (bRet)
	{
		retValue["uid"] = touid;
		retValue["name"] = pUserInfo->name;
		retValue["nick"] = pUserInfo->nick;
		retValue["icon"] = pUserInfo->name;
		retValue["gender"] = pUserInfo->gender;
	}
	else
	{
		retValue["error"] = ErrorCodes::Uid_Invalid;
	}

	Defer defer([this, &retValue, session]() {
		session->Send(retValue.toStyledString(), MSG_IDS::AUTH_FRIEND_RSP);
	});

	//change apply friend status
	MySqlMgr::GetInstance()->AuthFriendApply(uid, touid);

	//todo, add friend to sql.
}

bool TaskSystem::GetBaseInfo(std::string strKey, int uid, std::shared_ptr<UserInfo>& pUserInfo)
{
	std::string strInfo = "";
	bool bRet = CRedisMgr::GetInstance()->Get(strKey, strInfo);
	if (bRet)
	{
		Json::Reader reader;
		Json::Value root;
		reader.parse(strInfo, root);
		pUserInfo->uid = root["uid"].asInt();
		pUserInfo->name = root["name"].asString();
		pUserInfo->pwd = root["pwd"].asString();
		pUserInfo->email = root["email"].asString();
		pUserInfo->nick = root["nick"].asString();
		pUserInfo->desc = root["desc"].asString();
		pUserInfo->gender = root["gender"].asInt();
		pUserInfo->icon = root["icon"].asString();
	}
	else
	{
		pUserInfo = MySqlMgr::GetInstance()->GetUserInfo(uid);
		if (pUserInfo == nullptr)
			return false;

		Json::Value redisRoot;
		redisRoot["uid"] = uid;
		redisRoot["name"] = pUserInfo->name;
		redisRoot["pwd"] = pUserInfo->pwd;
		redisRoot["email"] = pUserInfo->email;
		redisRoot["nick"] = pUserInfo->nick;
		redisRoot["desc"] = pUserInfo->desc;
		redisRoot["gender"] = pUserInfo->gender;
		redisRoot["icon"] = pUserInfo->icon;

		CRedisMgr::GetInstance()->Set(strKey, redisRoot.toStyledString());
	}

	return true;
}

void TaskSystem::GetUserInfoByID(const std::string& strUid, Json::Value& retValue)
{
	retValue["error"] = ErrorCodes::Success;
	std::string baseInfo = USERBASEINFO;
	baseInfo += strUid;
	std::string strValue = "";
	Json::Reader reader;
	Json::Value root;
	bool bRet = CRedisMgr::GetInstance()->Get(baseInfo, strValue);
	if (bRet)
	{
		reader.parse(strValue, root);
		retValue["uid"] = root["uid"];
		retValue["name"] = root["name"];
		retValue["pwd"] = root["pwd"];
		retValue["email"] = root["email"];
		retValue["nick"] = root["nick"];
		retValue["desc"] = root["desc"];
		retValue["gender"] = root["gender"];
		retValue["icon"] = root["icon"];
		return;
	}

	//Get user info from mysql
	auto uid = atoi(strUid.c_str());
	std::shared_ptr<UserInfo> pUserInfo = MySqlMgr::GetInstance()->GetUserInfo(uid);
	if (pUserInfo == nullptr)
	{
		retValue["error"] = ErrorCodes::Uid_Invalid;
		return;
	}
	
	//Save to redis
	root["uid"] = pUserInfo->uid;
	root["name"] = pUserInfo->name;
	root["pwd"] = pUserInfo->pwd;
	root["email"] = pUserInfo->email;
	root["nick"] = pUserInfo->nick;
	root["desc"] = pUserInfo->desc;
	root["gender"] = pUserInfo->gender;
	root["icon"] = pUserInfo->icon;

	CRedisMgr::GetInstance()->Set(baseInfo, root.toStyledString());

	//Save to return value
	retValue["uid"] = pUserInfo->uid;
	retValue["name"] = pUserInfo->name;
	retValue["pwd"] = pUserInfo->pwd;
	retValue["email"] = pUserInfo->email;
	retValue["nick"] = pUserInfo->nick;
	retValue["desc"] = pUserInfo->desc;
	retValue["gender"] = pUserInfo->gender;
	retValue["icon"] = pUserInfo->icon;
}

void TaskSystem::GetUserInfoByName(const std::string& name, Json::Value& retValue)
{
	retValue["error"] = ErrorCodes::Success;
	std::string nameInfo = NAMEINFO;
	nameInfo += name;
	std::string strValue = "";
	Json::Reader reader;
	Json::Value root;
	bool bRet = CRedisMgr::GetInstance()->Get(nameInfo, strValue);
	if (bRet)
	{
		reader.parse(strValue, root);
		retValue["uid"] = root["uid"];
		retValue["name"] = root["name"];
		retValue["pwd"] = root["pwd"];
		retValue["email"] = root["email"];
		retValue["nick"] = root["nick"];
		retValue["desc"] = root["desc"];
		retValue["gender"] = root["gender"];
		retValue["icon"] = root["icon"];
		return;
	}

	//Get user info from mysql
	std::shared_ptr<UserInfo> pUserInfo = MySqlMgr::GetInstance()->GetUserInfo(name);
	if (pUserInfo == nullptr)
	{
		retValue["error"] = ErrorCodes::Uid_Invalid;
		return;
	}

	//Save to redis
	root["uid"] = pUserInfo->uid;
	root["name"] = pUserInfo->name;
	root["pwd"] = pUserInfo->pwd;
	root["email"] = pUserInfo->email;
	root["nick"] = pUserInfo->nick;
	root["desc"] = pUserInfo->desc;
	root["gender"] = pUserInfo->gender;
	root["icon"] = pUserInfo->icon;

	CRedisMgr::GetInstance()->Set(nameInfo, root.toStyledString());

	//Save to return value
	retValue["uid"] = pUserInfo->uid;
	retValue["name"] = pUserInfo->name;
	retValue["pwd"] = pUserInfo->pwd;
	retValue["email"] = pUserInfo->email;
	retValue["nick"] = pUserInfo->nick;
	retValue["desc"] = pUserInfo->desc;
	retValue["gender"] = pUserInfo->gender;
	retValue["icon"] = pUserInfo->icon;
}
