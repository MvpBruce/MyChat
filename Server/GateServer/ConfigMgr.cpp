#include "ConfigMgr.h"
#include <boost/filesystem.hpp>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp> 

ConfigMgr::ConfigMgr()
{
	boost::filesystem::path curPath = boost::filesystem::current_path();
	boost::filesystem::path iniPath = curPath/"config.ini";
	std::cout << "Config path: " << iniPath << std::endl;

	boost::property_tree::ptree pt;
	boost::property_tree::read_ini(iniPath.string(), pt);
	//Get all sections
	for (const auto& it: pt)
	{
		const std::string& sectionName = it.first;
		const boost::property_tree::ptree& section_data = it.second;
		std::map<std::string, std::string> section_map;
		for (const auto& sectionPair : section_data)
		{
			const std::string& key = sectionPair.first;
			const std::string& val = sectionPair.second.get_value<std::string>();
			section_map[key] = val;
		}

		Section sc;
		sc.m_sectionData = section_map;
		m_mapConfig[sectionName] = sc;
	}
}

ConfigMgr::~ConfigMgr()
{
	m_mapConfig.clear();
}

ConfigMgr::ConfigMgr(const ConfigMgr& src)
{
	this->m_mapConfig = src.m_mapConfig;
}

Section ConfigMgr::operator[] (std::string secName)
{
	if (m_mapConfig.find(secName) == m_mapConfig.end())
		return Section();

	return m_mapConfig[secName];
}

ConfigMgr& ConfigMgr::operator= (const ConfigMgr& src)
{
	if (&src == this)
		return *this;

	this->m_mapConfig = src.m_mapConfig;

	return *this;
}