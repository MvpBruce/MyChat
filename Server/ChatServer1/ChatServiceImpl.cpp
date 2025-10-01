#include "ChatServiceImpl.h"
#include "UserMgr.h"
#include "Const.h"
#include "Session.h"
#include "MySqlMgr.h"
#include "RedisMgr.h"

ChatServiceImpl::ChatServiceImpl()
{
}

Status ChatServiceImpl::NotifyAddFriend(ServerContext* context, const AddFriendReq* req, AddFriendRsp* rsp)
{
	auto toId = req->touid();
	Defer defer([req, rsp]() {
		rsp->set_error(ErrorCodes::Success);
		rsp->set_applyuid(req->applyuid());
		rsp->set_touid(req->touid());
	});

	//check if user is in this server
	auto session = UserMgr::GetInstance()->GetSession(toId);
	if (session == nullptr)
		return Status::OK;

	Json::Value retValue;
	retValue["error"] = ErrorCodes::Success;
	retValue["applyuid"] = req->applyuid();
	retValue["name"] = req->name();
	retValue["desc"] = req->desc();
	retValue["icon"] = req->icon();
	retValue["gender"] = req->gender();
	retValue["nick"] = req->nick();

	session->Send(retValue.toStyledString(), MSG_IDS::ADD_FRIEND_NOTIFY);

	return Status::OK;
}

Status ChatServiceImpl::NotifyAuthFriend(ServerContext* context, const AuthFriendReq* req, AuthFriendRsp* rsp)
{
	auto touid = req->touid();
	auto fromuid = req->fromuid();
	Defer defer([req, rsp]() {
		rsp->set_error(ErrorCodes::Success);
		rsp->set_fromuid(req->fromuid());
		rsp->set_touid(req->touid());
	});
	auto session = UserMgr::GetInstance()->GetSession(touid);
	if (session == nullptr)
		return Status::OK;

	Json::Value retValue;
	retValue["error"] = ErrorCodes::Success;
	retValue["fromuid"] = fromuid;
	retValue["touid"] = touid;

	std::string strKey = USERBASEINFO;
	strKey += std::to_string(fromuid);
	auto pInfo = std::make_shared<UserInfo>();
	bool bRet = GetBaseInfo(strKey, fromuid, pInfo);
	if (bRet)
	{
		retValue["name"] = pInfo->name;
		retValue["nick"] = pInfo->nick;
		retValue["icon"] = pInfo->icon;
		retValue["gender"] = pInfo->gender;
	}
	else
	{
		retValue["error"] = ErrorCodes::Uid_Invalid;
	}

	session->Send(retValue.toStyledString(), MSG_IDS::AUTH_FRIEND_NOTIFY);

	return Status::OK;
}

Status ChatServiceImpl::NotifyChatTextMsg(ServerContext* context, const ChatTextMsgReq* req, ChatTextMsgRsp* rsp)
{
	auto touid = req->touid();
	auto fromuid = req->fromuid();
	rsp->set_error(ErrorCodes::Success);
	auto session = UserMgr::GetInstance()->GetSession(touid);
	if (session == nullptr)
		return Status::OK;

	Json::Value retValue;
	retValue["error"] = ErrorCodes::Success;
	retValue["fromuid"] = fromuid;
	retValue["touid"] = touid;

	Json::Value textArray;
	for (auto& msgData : req->textmsgs())
	{
		Json::Value item;
		item["content"] = msgData.msgcontent();
		item["msgid"] = msgData.msgid();
		textArray.append(item);
	}

	retValue["text_array"] = textArray;
	session->Send(retValue.toStyledString(), MSG_IDS::TEXT_CHAT_MSG_NOTIFY);

	return Status::OK;
}

bool ChatServiceImpl::GetBaseInfo(std::string strKey, int uid, std::shared_ptr<UserInfo>& pUserInfo)
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
