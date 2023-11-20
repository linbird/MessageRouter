#include <iostream>

//enable_if_t

using IndifyToken = const char*;

#include <cxxabi.h>

std::string demangle(const char *mangled) {
    int status = 0;
    char *demangled = nullptr;
// #if HAS_CXA_DEMANGLE
    demangled = abi::__cxa_demangle(mangled, nullptr, nullptr, &status);
// #endif
    std::string out;
    if (status == 0 && demangled) { // Demangling succeeeded.
        out.append(demangled);
        free(demangled);
    } else {
        out.append(mangled);
    }
    return out;
}

struct Base {
    virtual ~Base() {}
};

template <typename T>
struct CallableBase : Base
{
    using Type = T;
    T _function;

    CallableBase(T function)
    {
        _function = function;
    }

    virtual void runner() {}
};

template <typename RetType, typename CallerClassType, typename ...Args>
struct Callable : CallableBase<RetType(CallerClassType::*)(Args...)>
{
    template <typename T>
    Callable(T function)
    : CallableBase<RetType(CallerClassType::*)(Args...)>(function)
    {

    }
};

//todo: 由于虚函数引入了运行时多态，改为CRTP实现

template <IndifyToken token>
struct EventInstace
{
    std::shared_ptr<Base> function;

    // template <typename RetType, typename CallerClassType, typename ...Args>
    // void Subscribe(RetType(CallerClassType::*)(Args...) callback, std::shared_ptr<CallerClassType> caller)
    template <typename CallbackType, typename CallerClassType>
    void Subscribe(CallbackType callback, std::shared_ptr<CallerClassType> caller)
    {
        std::shared_ptr<CallerClassType> _caller = caller;
        // decltype<callback> _callback = callback;
        function = std::make_shared<CallableBase<CallbackType>>(callback);
        std::cout << demangle(typeid(*function).name()) << std::endl;
    }

    template <typename ...Args>
    void BroadCast(Args ...args)
    {
        std::cout << demangle(typeid(*function).name()) << std::endl;
        std::cout << demangle(typeid(decltype(*function)).name()) << '\n';
        // std::static_cast<decltype(*function)::Type>(nullptr);
        // std::cout << demangle(typeid(**function).name()) << std::endl;
        // (*function)(std::forward<Args>(args)...);
    }

    template <typename ...Args, typename CallerClassType>
    void BroadCast(Args ...args)
    {
    }
};

class TestClass
{
public:
    void memberFunction(int, float)
    {
        std::cout << __FILE__ << ':' << __LINE__ << ':' << __FUNCTION__ << std::endl;
    }
};

constexpr char Event1[] = "event1";

int main()
{
            // std::cout << __FILE__ << ':' << __LINE__ << ':' << __FUNCTION__ << std::endl;

    EventInstace<Event1> eventInstace1;
    eventInstace1.Subscribe(&TestClass::memberFunction, std::make_shared<TestClass>());
    eventInstace1.BroadCast(1, 2.0);
            // std::cout << __FILE__ << ':' << __LINE__ << ':' << __FUNCTION__ << std::endl;

    return 0;
}