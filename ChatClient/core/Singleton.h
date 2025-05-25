#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>
#include <iostream>
#include <mutex>

//CRPT
template<typename T>
class Signleton {
protected:
    Signleton() = default;
    Signleton(const Signleton<T>&) = delete;
    Signleton& operator=(const Signleton<T>&) = delete;

    static std::shared_ptr<T> s_instance;

public:
    static std::shared_ptr<T> GetInstance()
    {
        static std::once_flag s_once;
        std::call_once(s_once, [&](){
            s_instance = std::shared_ptr<T>(new T);
        });
    }

    void PrintAddress() {
        std::cout << s_instance.get() << std::endl;
    }

    ~Signleton()
    {
        std::cout << "Signleton::~Signleton()" << std::endl;
    }
};

template<typename T>
std::shared_ptr<T> Signleton<T>::s_instance = nullptr;

#endif // SINGLETON_H
