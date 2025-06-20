#pragma once
#include <iostream>
#include <memory>

template<typename T>
class Singleton
{
protected:
	Singleton() = default;
	Singleton(const Singleton<T>&) = delete;
	Singleton& operator = (const Singleton<T>&) = delete;

	static std::shared_ptr<T> s_instance;

public:
	static std::shared_ptr<T> GetInstance()
	{
		static std::once_flag flag;
		std::call_once(flag, []() {
			s_instance = std::shared_ptr<T>(new T);
		});

		return s_instance;
	}

	~Singleton()
	{
		std::cout << "Destruct Singleton" << std::endl;
	}
};

template<typename T>
std::shared_ptr<T> Singleton<T>::s_instance = nullptr;

