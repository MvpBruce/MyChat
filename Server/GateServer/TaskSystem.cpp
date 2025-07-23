#include "TaskSystem.h"
#include "HttpConnection.h"
#include "VefifyGrpcClient.h"
#include "RedisMgr.h"
#include "MySqlMgr.h"

CTaskSystem::CTaskSystem()
{
	RegisterEvent("/get_verifycode", [](std::shared_ptr<CHttpConnection> connection){
		std::string strJson = beast::buffers_to_string(connection->m_parser.get().body().data());
		std::cout << "Received json from client: " << strJson << std::endl;
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
		root["error"] = rsp.error();
		beast::ostream(connection->m_response.body()) << root.toStyledString();
	}, http::verb::post);

	RegisterEvent("/register", [](std::shared_ptr<CHttpConnection> connection) {
		std::string strJson = beast::buffers_to_string(connection->m_parser.get().body().data());
		std::cout << "Received registration json: " << strJson << std::endl;
		connection->m_response.set(http::field::content_type, "text/json");
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
		std::string strUuid;
		//Get verifycode from redis
		if (!CRedisMgr::GetInstance()->Get(strEmail, strUuid))
		{
			std::cout << "Verification code was expired" << std::endl;
			root["error"] = static_cast<int>(ErrorCodes::Verify_Expired);
			beast::ostream(connection->m_response.body()) << root.toStyledString();
			return;
		}
		
		auto strCode = src_Json["verifycode"].asString();
		//Not matched
		if (strCode != strUuid)
		{
			std::cout << "Verification code was not matched" << std::endl;
			root["error"] = static_cast<int>(ErrorCodes::Verify_Error);
			beast::ostream(connection->m_response.body()) << root.toStyledString();
			return;
		}
		
		auto strUser = src_Json["user"].asString();
		auto strPwd = src_Json["password"].asString();
		//Check user and register
		int nUid = MySqlMgr::GetInstance()->RegisterUser(strUser, strEmail, strPwd);
		if (nUid == 0 || nUid == -1)
		{
			std::cout << "User or email existed" << std::endl;
			root["error"] = static_cast<int>(ErrorCodes::User_Existed);
			beast::ostream(connection->m_response.body()) << root.toStyledString();
			return;
		}

		root["error"] = static_cast<int>(ErrorCodes::Success);;
		root["email"] = strEmail;
		root["password"] = strPwd;
		root["verifycode"] = strCode;
		beast::ostream(connection->m_response.body()) << root.toStyledString();
	}, http::verb::post);

	RegisterEvent("/reset_pwd", [](std::shared_ptr<CHttpConnection> connection) {
		std::string strJson = beast::buffers_to_string(connection->m_parser.get().body().data());
		std::cout << "Received json: " << strJson << std::endl;
		connection->m_response.set(http::field::content_type, "text/json");
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

		auto email = src_Json["email"].toStyledString();
		auto user = src_Json["user"].toStyledString();
		auto password = src_Json["password"].toStyledString();

		std::string strUuid;
		//Get verifycode from redis
		if (!CRedisMgr::GetInstance()->Get(email, strUuid))
		{
			std::cout << "Verification code was expired" << std::endl;
			root["error"] = static_cast<int>(ErrorCodes::Verify_Expired);
			beast::ostream(connection->m_response.body()) << root.toStyledString();
			return;
		}

		//Check verification code 
		auto verifyCode = src_Json["verifycode"].toStyledString();
		if (strUuid != verifyCode)
		{
			std::cout << "Verification code was not matched" << std::endl;
			root["error"] = static_cast<int>(ErrorCodes::Verify_Error);
			beast::ostream(connection->m_response.body()) << root.toStyledString();
			return;
		}

		//Check user and email exist in database
		bRet = MySqlMgr::GetInstance()->CheckEmail(user, email);
		if (!bRet)
		{
			std::cout << "User email not matched" << std::endl;
			root["error"] = static_cast<int>(ErrorCodes::Email_Not_Matched);
			beast::ostream(connection->m_response.body()) << root.toStyledString();
			return;
		}

		//Update password
		bRet = MySqlMgr::GetInstance()->UpdatePassowrd(user, password);
		if (!bRet)
		{
			std::cout << "Failed to update password" << std::endl;
			root["error"] = static_cast<int>(ErrorCodes::Password_Update_Failed);
			beast::ostream(connection->m_response.body()) << root.toStyledString();
			return;
		}

		std::cout << "Succeed to update passowrd" << password << std::endl;
		root["error"] = static_cast<int>(ErrorCodes::Success);
		root["email"] = email;
		root["user"] = user;
		root["password"] = password;
		root["verifycode"] = verifyCode;
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
