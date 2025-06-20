#pragma once
#include "Const.h"

class CHttpConnection;
typedef std::function<void(std::shared_ptr<CHttpConnection>)> HttpHandler;

class CTaskSystem : public Singleton<CTaskSystem>
{
	friend class Singleton<CTaskSystem>;
private:
	CTaskSystem();

public:
	~CTaskSystem();

	void RegisterEvent(std::string strName, HttpHandler handler, http::verb method);

	bool ExecuteEvent(std::string strName, std::shared_ptr<CHttpConnection> connection, http::verb method);

private:
	std::unordered_map<std::string, HttpHandler> m_getHandlers;
	std::unordered_map<std::string, HttpHandler> m_postHandlers;
};


