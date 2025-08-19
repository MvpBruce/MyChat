#pragma once
#include "Const.h"

class AsioServicePool : public Singleton<AsioServicePool>
{
	friend class Singleton<AsioServicePool>;
public:
	AsioServicePool(const AsioServicePool&) = delete;
	AsioServicePool& operator = (const AsioServicePool&) = delete;
	
	net::io_context& GetService();
	~AsioServicePool();

private:
	AsioServicePool(size_t number = std::thread::hardware_concurrency());
	
private:
	std::vector<std::thread> m_threads;
	std::vector<net::io_context> m_ioServices;
	std::vector<net::executor_work_guard<net::io_context::executor_type>> m_works;
	size_t m_nextIOService;
};

