#pragma once
#include<string>

struct UserInfo
{
	int uid;
	int gender;
	std::string name;
	std::string pwd;
	std::string email;
	std::string nick;
	std::string desc;
	std::string icon;

	UserInfo()
		: uid(0), gender(0), name(""), pwd(""), email(""), nick(""), desc(""), icon("")
	{
	}
};


struct ApplyInfo
{
	int uid;
	int gender;
	std::string name;
	std::string desc;
	std::string icon;
	std::string nick;
	int status;

	ApplyInfo(int uid, int gender, const std::string& name, const std::string& desc, const std::string& icon, const std::string& nick, int status)
		: uid(uid), gender(gender), name(name), desc(desc), icon(icon), nick(nick), status(status)
	{
	}
};