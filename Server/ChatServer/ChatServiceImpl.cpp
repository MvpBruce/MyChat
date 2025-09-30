#include "ChatServiceImpl.h"
#include "UserMgr.h"
#include "Const.h"
#include "Session.h"

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

	return Status::OK;
}
