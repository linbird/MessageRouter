#include <iostream>
#include <memory>
#include <string>
//enable_if_t
#ifdef __linux__
#include <format>
#else
#include <sstream>
#endif

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

class TestClass1
{
public:
    void memberFunction1(int a, float b)
    {

        #ifdef __linux__
        std::cout<< std::format("[{}:{}]::{}({} {}, {} {})",__FILE__ , __LINE__, __FUNCTION__, demangle(typeid(a).name()), a, demangle(typeid(b).name()), b) << std::endl;
        #else
        std::stringstream oss;
        oss << '[' << __FILE__ << ':' << __LINE__ << ']' << "::" << __FUNCTION__ << '(' << demangle(typeid(a).name()) << ' ' << a << "," << demangle(typeid(b).name()) << ' ' << b << ")\n";
        std::cout << oss.str();
        // char buffer[100];
        // std::snprintf(buffer, sizeof(buffer), "[%s:%d]::%s(%s %d, %s %f)\n", __FILE__ , __LINE__, __FUNCTION__, demangle(typeid(a).name()).c_str(), a, demangle(typeid(b).name()).c_str(), b);
        // std::cout << buffer;
        // std::cout << __FILE__ << ':' << __FUNCTION__ << '(' << ')'<< std::endl;
        #endif
    }
};
class TestClass2
{
public:
    int memberFunction2(float a, int b)
    {

        #ifdef __linux__
        std::cout<< std::format("[{}:{}]::{}({} {}, {} {})",__FILE__ , __LINE__, __FUNCTION__, demangle(typeid(a).name()), a, demangle(typeid(b).name()), b) << std::endl;
        #else
        std::stringstream oss;
        oss << '[' << __FILE__ << ':' << __LINE__ << ']' << "::" << __FUNCTION__ << '(' << demangle(typeid(a).name()) << ' ' << a << "," << demangle(typeid(b).name()) << ' ' << b << ")\n";
        std::cout << oss.str();
        // char buffer[100];
        // std::snprintf(buffer, sizeof(buffer), "[%s:%d]::%s(%s %d, %s %f)\n", __FILE__ , __LINE__, __FUNCTION__, demangle(typeid(a).name()).c_str(), a, demangle(typeid(b).name()).c_str(), b);
        // std::cout << buffer;
        // std::cout << __FILE__ << ':' << __FUNCTION__ << '(' << ')'<< std::endl;
        #endif
        return 0;
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

    CallBackInfo<TestClass1, decltype(&TestClass1::memberFunction1)> call;
#define MR_Store(ClassName, FunctionName)
#ifndef MR_SUB
#define MR_SUB(EventName, ClassName, FunctionName) MR_Store(ClassName, FunctionName)
#endif
#undef MR_SUB
    // CallBackInfo<TestClass1, decltype(&TestClass1::memberFunction2)> call2;
    //todo: macro expand

    template <typename CallbackType, typename CallerClassType>
    void Subscribe(CallbackType callback, std::shared_ptr<CallerClassType> caller)
    {
        call.caller = caller;
        call.callback = callback;
        //todo: macro expand
#define MR_Reg(ClassName, FunctionName) 
#ifndef MR_SUB
#define MR_SUB(EventName, ClassName, FunctionName) MR_Reg(ClassName, FunctionName)
#endif
#undef MR_SUB
    }

    template <typename ...Args>
    void BroadCast(Args ...args)
    {
        call.run(std::forward<Args>(args)...);
        //todo: macro expand
#define MR_Call(ClassName, FunctionName)
#ifndef MR_SUB
#define MR_SUB(EventName, ClassName, FunctionName) MR_Call(ClassName, FunctionName)
#endif
#undef MR_SUB
    }
};


constexpr char Event1[] = "event1";
constexpr char Event2[] = "event2";


#define EVENT1 "k_EVENT1"


int main()
{

    MR_SUB(EVENT1, "TestClass1", "memberFunction1");
    MR_SUB(EVENT1, "TestClass2", "memberFunction2");
    // EventInstace<Event1> eventInstace1;
    // EventInstace<Event2> eventInstace2;
    // eventInstace1.Subscribe(&TestClass1::memberFunction1, std::make_shared<TestClass1>());
    // // eventInstace1.Subscribe(&TestClass2::memberFunction2, std::make_shared<TestClass2>());
    // eventInstace1.BroadCast(1, 2.0);
    // eventInstace1.BroadCast(2.0, 1);

    return 0;
}
