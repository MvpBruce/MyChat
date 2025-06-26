#include "TaskSystem.h"
#include "HttpConnection.h"
#include "VefifyGrpcClient.h"

CTaskSystem::CTaskSystem()
{
	RegisterEvent("/get_verifycode", [](std::shared_ptr<CHttpConnection> connection){
		std::string strJson = beast::buffers_to_string(connection->m_parser.get().body().data());
		std::cout << "Received json: " << strJson << std::endl;
		connection->m_response.set(http::field::content_type, "text/json");
		//Parse json string to json object
		Json::Value root;
		Json::Reader reader;
		Json::Value src_Json;
		bool bRet = reader.parse(strJson, src_Json);
		if (!bRet)
		{
			std::cout << "Parsed Json error" << std::endl;
			root["error"] = static_cast<int>(ErrorCodes::Error_Json);
			std::string strRes = root.toStyledString();
			beast::ostream(connection->m_response.body()) << strRes;
			return;
		}

		auto strEmail = src_Json["email"].asString();
		GetVerifyRsp rsp = CVefifyGrpcClient::GetInstance()->GetVerifyCode(strEmail);
		std::cout << "Email is: " << strEmail << std::endl;
		root["email"] = strEmail;
		root["error"] = rsp.code().c_str();
		beast::ostream(connection->m_response.body()) << root.toStyledString();
	}, http::verb::post);
}

CTaskSystem::~CTaskSystem()
{
	
}

void CTaskSystem::RegisterEvent(std::string strName, HttpHandler handler, http::verb method)
{
	if (method == http::verb::get)
		m_getHandlers.emplace(strName, handler);
	else if (method == http::verb::post)
		m_postHandlers.emplace(strName, handler);
}

bool CTaskSystem::ExecuteEvent(std::string strName, std::shared_ptr<CHttpConnection> connection, http::verb method)
{
	if (method == http::verb::get)
	{
		auto it = m_getHandlers.find(strName);
		if (it != m_getHandlers.end())
		{
			it->second(connection);
			return true;
		}
	}
	else if (method == http::verb::post)
	{
		auto it = m_postHandlers.find(strName);
		if (it != m_postHandlers.end())
		{
			it->second(connection);
			return true;
		}
	}
	
	return false;
}
