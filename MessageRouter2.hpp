//
//  MessageRouter2.hpp
//  MessageRouter
//
//  Created by Jovy Zhang on 2023/11/17.
//

#ifndef MessageRouter2_hpp
#define MessageRouter2_hpp

#include <memory>
#include <unordered_map>
#include <string>
#include <any>
#include <map>
#include <functional>
#include <cassert>
#include <cxxabi.h>

#define ContextCapture \
std::cout << __FUNCTION__ << ':' << __LINE__  << ": "

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

class TestClass;

struct FunctionTraitBase {
    virtual void parentFunction() {
        ContextCapture << std::endl;
    }
};

template <typename Type>
struct FunctionTrait;

template <typename Ret, typename ...Args>
struct FunctionTrait<Ret(Args...)> : FunctionTraitBase
{
    using ReturnType = Ret;
    using ClassType = nullptr_t;
    using FunctionType = Ret(Args...);

    template<size_t index>
    struct Arg
    {
        static_assert(index < sizeof...(Args));
        using Type = std::tuple_element_t<index, std::tuple<Args...>>;
    };

void parentFunction() {
        ContextCapture << std::endl;
    }
};

template <typename Ret, typename ...Args>
struct FunctionTrait<Ret(*)(Args...)> : FunctionTrait<Ret(Args...)> {void parentFunction() {
        ContextCapture << std::endl;
    }};

template <typename Ret, typename ...Args>
struct  FunctionTrait<std::function<Ret(Args...)>> : FunctionTrait<Ret(Args...)> {void parentFunction() {
        ContextCapture << std::endl;
    }};

template <typename Callable>
struct FunctionTrait : FunctionTrait<decltype(&Callable::operator())> {void parentFunction() {
        ContextCapture << std::endl;
    }};

template <typename Ret, typename _ClassType, typename ...Args>
struct FunctionTrait<Ret(_ClassType::*)(Args...)> : FunctionTrait<Ret(Args...)> {
    using ClassType = _ClassType;
    using FunctionType = Ret(_ClassType::*)(Args...);
};

template <typename Ret, typename _ClassType, typename ...Args>
struct FunctionTrait<Ret(_ClassType::*)(Args...) const> : FunctionTrait<Ret(Args...)> {
    using ClassType = _ClassType;
    using FunctionType = Ret(_ClassType::*)(Args...) const;

    void parentFunction() {
        ContextCapture << std::endl;
    }
};

//template <typename Ret, typename ClassType, typename ...Args>
//struct FunctionTrait<Ret(ClassType::*)(Args...) volatile> : FunctionTrait<Ret(Args...)> {};
//
//template <typename Ret, typename ClassType, typename ...Args>
//struct FunctionTrait<Ret(ClassType::*)(Args...) const volatile> : FunctionTrait<Ret(Args...)> {};

//template <typename Type>
//class MessageRouter2;

//template <typename Type>
//static std::map<std::string, std::shared_ptr<MessageRouter2<Type>>> routers;

class MessageRouter2;
std::shared_ptr<MessageRouter2> shareRouter(nullptr);

//同一个webService回调可以call到不同的Service，即使Service有相同/不同的函数签名，需要考虑同一个类里面有相同函数类型的多个函数，
/*
 required:
    1.同一个webService回调可以同时call到不同的Service
        solution: 每个webService用唯一的标识（decltype+functionName），标识需要提供消息来源（class.method）
 optional:
    1.需要支持不同的call（仿函数，function，类成员函数，函数指针）
        solution：函数模板 + 便特化
    2.支持不同的被call方获得不同的参数
        solution：回掉方发送参数包，被回掉方全拿，选择性使用 <Args...>
*/

//template <typename Type>
class MessageRouter2
{
private:
    using Key = std::string;
    using Caller = std::any;
    using Callee = std::any;
    using CallablePair = std::pair<Caller, Callee>;

public:
    MessageRouter2(const MessageRouter2&) = delete;
    MessageRouter2(MessageRouter2&&) = delete;
    MessageRouter2 operator=(const MessageRouter2&) = delete;
    MessageRouter2 operator=(MessageRouter2&&) = delete;
    
    static std::shared_ptr<MessageRouter2> GetInstance()
    {
        if (!shareRouter)
//            shareRouter = std::make_shared<MessageRouter2>();
            shareRouter = std::shared_ptr<MessageRouter2>(new MessageRouter2);
        return shareRouter;
    }
    
    template <typename ...Args>
    int Boardcast(const Key& key, Args... args)
    {
        int result = -1;
        auto callbackIters = m_map.equal_range(key);
        
        for (auto callableIter = callbackIters.first; callableIter != callbackIters.second; ++callableIter)
        {
            auto callable = callableIter->second;
            _InvokeFunction(callable.first, callable.second, std::forward<Args>(args)...);
            
            ++result;
        }
        
        return result;
    }
        
    template <typename Calllback, typename CallerType>
    size_t Subscribe(Key key, Calllback function, std::shared_ptr<CallerType> caller)
    {
        // ContextCapture << key << ':' << caller << "> " << std::is_member_function_pointer<Calllback>::value << std::endl;
        // // is_member_function_pointer<Calllback>::value
        // if(std::is_member_function_pointer<Calllback>::value)
        // {
            typename FunctionTrait<decltype(function)>::ClassType a;
            typename FunctionTrait<decltype(function)>::FunctionType b;
            // ContextCapture << std::is_same_v<typename FunctionTrait<decltype(function)>::ClassType, TestClass> << std::endl;
            // ContextCapture << std::is_same_v<typename FunctionTrait<decltype(function)>::ClassType, nullptr_t> << std::endl;
            // ContextCapture << std::is_same_v<typename FunctionTrait<decltype(function)>::ClassType, void> << std::endl;
            ContextCapture << demangle(typeid(typename FunctionTrait<decltype(function)>::FunctionType).name()) << std::endl;
            ContextCapture << demangle(typeid(Calllback).name()) << std::endl;
        // }
        m_typeInfo[typeid(Calllback).hash_code()] = std::shared_ptr<FunctionTraitBase>(new FunctionTrait<Calllback>);

        return _Register(key, CallablePair(caller, function));
    }

    template <typename Calllback>
    size_t Subscribe(Key key, Calllback function)
    {
        using FunctionType = typename FunctionTrait<decltype(function)>::FunctionType;
        ContextCapture << demangle(typeid(FunctionType).name()) << std::endl;
        ContextCapture << demangle(typeid(Calllback).name()) << std::endl;

        // m_typeInfo[typeid(FunctionType).hash_code()] = std::shared_ptr<FunctionTraitBase>(new FunctionTrait<decltype(function)>);
        m_typeInfo[typeid(Calllback).hash_code()] = std::shared_ptr<FunctionTraitBase>(new FunctionTrait<Calllback>);
        return _Register(key, CallablePair(nullptr, function));
    }
    
    // template <typename Ret = void, typename CallerType, typename ...Args>
    // size_t Subscribe(Key key, typename FunctionTrait<Ret(CallerType::*)(Args...)>::FunctionType function, std::shared_ptr<CallerType> caller )
    // {
    //     std::cout << __FILE__ << __LINE__ << std::endl;
    //     static_assert(caller);
    //     static_assert(!std::is_same_v<void, CallerType>);
    //     static_assert(!std::is_same_v<nullptr_t, CallerType>);
    //     return _Register(key, CallablePair(caller, function));
    // }

    template <typename Function>
    void UnSubscribe(Key key, Function& function)
    {
        auto iters = m_map.equal_range(key);
        m_map.erase(std::find(iters.first, iters.second, function));
    }
    
    void UnSubscribe(Key key)
    {
        m_map.erase(key);
    }

    ~MessageRouter2()
    {
//        m_map.clear();
    }
    
private:
    MessageRouter2() = default;

    int _Register(Key key, CallablePair&& callable)
    {
        m_map.emplace(key, callable);
        // std::cout << m_map.size() << ' ';
        return m_map.count(key);
    }

    // template <typename Calllback, typename ...Args>
    // template <typename ...Args, typename CallerType = void>
    // void _InvokeFunction(std::shared_ptr<CallerType> caller, std::any function, Args ...args)

    template <typename ...Args>
    void _InvokeFunction(std::any caller, std::any function, Args ...args)
    {
        assert(function.has_value());
        assert(caller.has_value());

        auto &callerTypeInfo = caller.type();
        // ContextCapture << demangle(callerTypeInfo.name()) << std::endl;
        ContextCapture << demangle(function.type().name()) << std::endl;

        size_t typeKey = function.type().hash_code();
        auto _ = m_typeInfo.find(typeKey);
        std::shared_ptr<FunctionTraitBase> typeIter = (_ == m_typeInfo.end()? nullptr : _->second);
        ContextCapture << demangle(typeid(*typeIter).name()) << std::endl;
        using functionTraitType = std::remove_reference<decltype(*typeIter)>::type;
        // typename functionTraitType::FunctionType;

        // typeid(*x).hash_code()
        
        // typename decltype(*x)::FunctionType;

        // std::any_cast<>
        
        // if(!x)
        //     ContextCapture << "nullptr" << std::endl;
        // else
        {
            
            // x->parentFunction();
            // typename x->FunctionType;
        }
        // using Type = typename decltype(x->get())::FuntionType;

        // std::any_cast<Type>(function);

        // typename FunctionTrait<decltype(function)>::FunctionType xx;
        // ContextCapture << demangle(typeid(xx).name()) << std::endl;
        
        // auto x = std::any_cast<typename FunctionTrait<decltype(function)>::FunctionType>>(function);

        // if(callerTypeInfo == typeid(nullptr))
        // {
            
        //     // std::any_cast<function.type()>(function)(std::forward<Args>(args)...);
        //     ContextCapture << std::endl;
        // } else if (callerTypeInfo == typeid(void))
        // {
        //     ContextCapture << std::endl;
        // }
        // else{
        //     ContextCapture << std::endl;
        // }
        // if(caller)
        // {
        //     // assert(caller->has_value());
        //     try{
        //         // std::cout << __FILE__ << __LINE__ << std::endl;
        //         // std::cout << std::endl << function.type().name() << std::endl;
        //         // auto &&callee = std::any_cast<function.type()>(function);
        //         // // auto _caller = std::any_cast<caller->type()>(caller);
        //         // caller->callee(std::forward<Args>(args)...);
        //     }
        //     catch (const std::bad_any_cast& e)
        //     {
        //         // std::cout << __FILE__ << e.what() << '\n';
        //     }
        // }
        // else {
        //     // std::cout << __FILE__ << __LINE__ << std::endl;
        //     // function(std::forward<Args>(args)...);
        // }
    }

private:
    std::unordered_multimap<Key, CallablePair> m_map;
    std::unordered_map<std::size_t, std::shared_ptr<FunctionTraitBase>> m_typeInfo;
};

//todo: 过滤重复注册同一个函数
// template <std::string KEY>
template <const char* KEY>
struct EventCenter {
    //<caller: callback>
    // void* caller;
    // vector<FunctionBase> callbackFamily;
    // vector<std::any>
    std::vector<std::pair<std::any, std::any>> callablePair;

    template <typename Callback, typename ...Args>
    void Subscribe(Callback callback, Args ...args)
    {
        constexpr size_t arity = sizeof...(Args);
        std::cout << arity << std::endl;
        if constexpr(1 == sizeof...(Args))
        {
                            static_assert(std::is_member_function_pointer_v<decltype(callback)>);
                using ClassType = typename FunctionTrait<decltype(callback)>::ClassType;
                auto _args = std::forward_as_tuple(std::forward<Args>(args)...);
                std::shared_ptr<ClassType> caller = std::get<0>(_args);
                ContextCapture << typeid(caller.get()).name() << std::endl;
                ContextCapture << demangle(typeid(callback).name()) << std::endl;
                
                // callablePair.emplace_back(wrapper(caller), wrapper<typename FunctionTrait<decltype(callback)>::FunctionType>(callback));
                callablePair.emplace_back(wrapper(caller), wrapper(callback, 1));

            ContextCapture << std::endl;
        } else {
                            callablePair.emplace_back(wrapper(nullptr), wrapper(callback));
                ContextCapture << typeid(callback).name() << std::endl;

            ContextCapture << std::endl;
        }
        // switch(arity){
        //     case 0:
        //             std::cout << 1 << std::endl;

        //         callablePair.emplace_back(wrapper(nullptr), wrapper(callback));
        //         break;

        //     case 1:
        //             std::cout << 2 << std::endl;

        //         static_assert(std::is_member_function_pointer_v<decltype(callback)>);
        //         // using ClassType = typename FunctionTrait<decltype(callback)>::ClassType;
        //         // auto _args = std::forward_as_tuple(std::forward<Args>(args)...);
        //         // std::shared_ptr<typename FunctionTrait<decltype(callback)>::ClassType> caller = std::get<0>(_args);
        //         // callablePair.emplace_back(wrapper(caller), wrapper(callback));
        //         break;
            
        //     // default:
        //     //     // static_assert(false, "errorInfo");
        //     //     break;
        // }
    }

    template <typename Ret, typename CallerType, typename ...Args>
    std::any wrapper(typename FunctionTrait<Ret(CallerType::*)(Args...)>::FunctionType function, int x)
    {
                    ContextCapture << std::endl;

        return std::any(function);
    }

    template <typename Callable>
    std::any wrapper(Callable callback)
    {
                    ContextCapture << std::endl;

        return std::any(callback);
    }

    template <typename CallerType>
    std::any wrapper(std::shared_ptr<CallerType> caller)
    {
                    ContextCapture << std::endl;

        return std::any(caller);
    }

    std::any wrapper(nullptr_t /**/)
    {
                    ContextCapture << std::endl;

        return std::any();
    }

};

#endif /* MessageRouter2_hpp */
