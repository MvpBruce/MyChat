#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>
#include <iostream>
#include <mutex>

//CRPT
template<typename T>
class Singleton {
protected:
    Singleton() = default;
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator=(const Singleton<T>&) = delete;

    static std::shared_ptr<T> s_instance;

public:
    static std::shared_ptr<T> GetInstance()
    {
        static std::once_flag s_once;
        std::call_once(s_once, [&](){
            s_instance = std::shared_ptr<T>(new T);
        });

        return s_instance;
    }

    void PrintAddress() {
        std::cout << s_instance.get() << std::endl;
    }

    ~Singleton()
    {
        std::cout << "Singleton::~Singleton()" << std::endl;
    }
};

template<typename T>
std::shared_ptr<T> Singleton<T>::s_instance = nullptr;

#endif // SINGLETON_H
