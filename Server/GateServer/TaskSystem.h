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

	void RegisterEvent(std::string strName, HttpHandler handler);

	bool ExecuteEvent(std::string strName, std::shared_ptr<CHttpConnection> connection);

private:
	std::map<std::string, HttpHandler> m_handlers;
};


