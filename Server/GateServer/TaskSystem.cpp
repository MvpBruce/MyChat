#include "TaskSystem.h"
#include "HttpConnection.h"

CTaskSystem::CTaskSystem()
{
	RegisterEvent("/get_test", [](std::shared_ptr<CHttpConnection> connection){
		beast::ostream(connection->m_response.body()) << "Received test!" << std::endl;
		for (auto& item : connection->m_parameters)
		{
			beast::ostream(connection->m_response.body()) << "Param: " << item.first << " , Value: " << item.second << std::endl;;
			//beast::ostream(connection->m_response.body()) << "Param: " << item.first << std::endl;
		}
	});
}

CTaskSystem::~CTaskSystem()
{
	
}

void CTaskSystem::RegisterEvent(std::string strName, HttpHandler handler)
{
	m_handlers.emplace(strName, handler);
}

bool CTaskSystem::ExecuteEvent(std::string strName, std::shared_ptr<CHttpConnection> connection)
{
	auto it = m_handlers.find(strName);
	if (it != m_handlers.end())
	{
		it->second(connection);
		return true;
	}
		
	return false;
}
