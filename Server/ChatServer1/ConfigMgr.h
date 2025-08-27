#pragma once
#include <map>
#include <string>

struct Section
{
	Section() {}
	~Section() { m_sectionData.clear(); }

	Section(const Section& src) {
		m_sectionData = src.m_sectionData;
	}

	Section& operator = (const Section& src)
	{
		if (&src == this)
			return *this;

		this->m_sectionData = src.m_sectionData;
		return *this;
	}

	std::string operator[] (const std::string key)
	{
		if (m_sectionData.find(key) == m_sectionData.end())
			return "";

		return m_sectionData[key];
	}

	std::map<std::string, std::string> m_sectionData;
};
class ConfigMgr
{
public:
	
	~ConfigMgr();

	ConfigMgr(const ConfigMgr& src);

	Section operator[](std::string secName);
	ConfigMgr& operator= (const ConfigMgr& src);
	static ConfigMgr& GetInstance();

private:
	ConfigMgr();
	std::map<std::string, Section> m_mapConfig;
};

