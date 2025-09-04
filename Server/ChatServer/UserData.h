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
