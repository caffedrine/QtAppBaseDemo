#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <QtGlobal>
#include <string>

template <typename T, typename D = T>
class Singleton
{
    friend D;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    static_assert(std::is_base_of_v<T, D>, "T should be a base type for D");
#endif

public:
    static T& instance()
    {
        static D inst;
        return inst;
    }

private:
    Singleton() = default;;
    ~Singleton() = default;
    Singleton( const Singleton& ) = delete;
    Singleton& operator=( const Singleton& ) = delete;
protected:
    virtual void OnContructorCalled() {};
};

#endif // _SINGLETON_H_
