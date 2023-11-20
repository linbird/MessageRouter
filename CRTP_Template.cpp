#include <iostream>
#include <memory>
#include <string>
#include <format>
//enable_if_t
// #include <stream>

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

class TestClass
{
public:
    void memberFunction(int a, float b)
    {
        // std::stringstream oss;
        // std::cout << __FILE__ << ':' << __FUNCTION__ << '(' << ')'<< std::endl;
        std::cout<< std::format("[{}:{}]::{}({} {}, {} {})", __FILE__, __LINE__, __FUNCTION__, demangle(typeid(a).name()), a, demangle(typeid(b).name()), b) << std::endl;
    }
};




// struct Base {
//     virtual ~Base() {}
// };

// template <typename T>
// struct CallableBase : Base
// {
//     using Type = T;
//     T _function;

//     CallableBase(T function)
//     {
//         _function = function;
//     }

//     virtual void runner() {}
// };

// template <typename RetType, typename CallerClassType, typename ...Args>
// struct Callable : CallableBase<RetType(CallerClassType::*)(Args...)>
// {
//     template <typename T>
//     Callable(T function)
//     : CallableBase<RetType(CallerClassType::*)(Args...)>(function)
//     {

//     }
// };

// //todo: 由于虚函数引入了运行时多态，改为CRTP实现
//  todo: 宏+模板减少手写代码量

// template <typename T>
// struct VirtualBase
// {
//     template <typename ...Args>
//     void run(Args ...arg)
//     {
//         static_cast<T*>(this)->run();
//     }
// };

// template<typename CallbackType>
// struct Callback : VirtualBase<Callback<CallbackType>>
// {
//     CallbackType function;

//     template <typename ...Args>
//     void run(Args ...args)
//     {
//         function(std::forward<Args>(args)...);
//     }
// };

// template <typename T>
// struct CallBase
// {
//     template <typename ...Args>
//     void run(Args ...args)
//     {
//         static_cast<T*>(this)->run(std::forward<Args>(args)...);
//     }
// };

template <typename CallerType, typename CallbackType>
struct CallBackInfo /*: CallBase<CallBackInfo<CallerType, CallbackType>>*/
{
    std::shared_ptr<CallerType> caller;
    // Callback<CallbackType> callback;
    CallbackType callback;

    CallBackInfo()
    {

    }

    template <typename ...Args>
    void run(Args ...args)
    {
        ((*caller).*callback)(std::forward<Args>(args)...);
    }
};
// #define Instance<()>

template <IndifyToken token>
struct EventInstace
{
    // std::shared_ptr<VirtualBase> _function;

    CallBackInfo<TestClass, decltype(&TestClass::memberFunction)> call;

    // template <typename RetType, typename CallerClassType, typename ...Args>
    // void Subscribe(RetType(CallerClassType::*)(Args...) callback, std::shared_ptr<CallerClassType> caller)
    template <typename CallbackType, typename CallerClassType>
    void Subscribe(CallbackType callback, std::shared_ptr<CallerClassType> caller)
    {
        call.caller = caller;
        call.callback =callback;
    }

    template <typename ...Args>
    void BroadCast(Args ...args)
    {
        call.run(std::forward<Args>(args)...);
    }

};


constexpr char Event1[] = "event1";

int main()
{

    EventInstace<Event1> eventInstace1;
    eventInstace1.Subscribe(&TestClass::memberFunction, std::make_shared<TestClass>());
    eventInstace1.BroadCast(1, 2.0);

    return 0;
}
