#include "ChatGrpcClient.h"
#include "ConfigMgr.h"
#include <sstream>

ChatGrpcClient::ChatGrpcClient()
{
	auto& config = ConfigMgr::GetInstance();
	auto serverList = config["ServerList"]["Servers"];

	std::stringstream ss(serverList);
	std::vector<std::string> serverNames;
	std::string strName;
	while (std::getline(ss, strName, ','))
		serverNames.push_back(strName);

	for (auto& name : serverNames)
	{
		m_chatPool.emplace(config[name]["host"], std::make_unique<ChatPool>(config[name]["host"], config[name]["port"]));
	}
}
