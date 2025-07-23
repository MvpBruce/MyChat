#include "AsioServicePool.h"

AsioServicePool::AsioServicePool(size_t number)
	: m_ioServices(number), m_nextIOService(0)
{
	for (size_t i = 0; i < number; ++i)
	{
		m_works.emplace_back(net::make_work_guard(m_ioServices[i]));
	}

	for (size_t i = 0; i < number; ++i)
	{
		m_threads.emplace_back([this, i]() {
			m_ioServices[i].run();
		});
	}
}

AsioServicePool::~AsioServicePool()
{
	for (auto& work : m_works)
	{
		work.get_executor().context().stop();
		work.reset();
	}

	for (auto& thread : m_threads)
	{
		thread.join();
	}
}

net::io_context& AsioServicePool::GetService()
{
	auto& service = m_ioServices[m_nextIOService++];
	if (m_nextIOService == m_ioServices.size())
		m_nextIOService = 0;

	return service;
}

