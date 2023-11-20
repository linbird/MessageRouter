//
//  main.cpp
//  MessageRouter
//
//  Created by Jay Zhang on 2023/10/18.
//

#include <iostream>
#include "MessageRouter.hpp"
#include "MessageRouter2.hpp"
#include "API.hpp"

/*
// 定义一个函数类型
typedef int (*FuncType)(int, double);



int main1(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    MessageRouter router;
    
    
//    router.Add<int>("no_input", [=]()->int{std::cout << "no input\n"; return 1; });
//
//    bool find = router.Find<int>("no_input");
//
//    std::cout << (find ? "find topic" : "cannot find topic") << " no_input\n";
//
//    router.Send<int>("no_input");
//
//    router.Remove<int>("no_input");
//
//    find = router.Find<int>("no_input");
//
//    std::cout << (find ? "find topic" : "cannot find topic") << " no_input\n";
    
    
    
//    auto f2 = [=](int input)->void{std::cout << "input:" << input << "\n"; };
//
//    router.Add<void, int>("has_input", f2);
//
//    find = router.Find<void, int>("has_input");
//
//    std::cout << (find ? "find topic" : "cannot find topic") << " has_input\n";
//
//    router.Send<void, int>("has_input", 1);
//
//    router.Remove<void, int>("has_input");
//
//    find = router.Find<void, int>("has_input");
//
//    std::cout << (find ? "find topic" : "cannot find topic") << " has_input\n";
    
    using MyFun = std::function<int()>;

    auto f = [=]()->int{std::cout << "no input\n"; return 1; };
    
//    router.Add<API_OnVoid>(KEY_OnVoid, f);
    WS_API_ADD(router, OnVoid, f);

//    bool find = router.Find<API_OnVoid>(KEY_OnVoid);
    bool find = WS_API_FIND(router, OnVoid);

    std::cout << (find ? "find topic" : "cannot find topic") << " no_input\n";

//    router.Send<API_OnVoid>(KEY_OnVoid, {});
    WS_API_SEND(router, OnVoid, WS_PARAS());

//    router.Remove<API_OnVoid>(KEY_OnVoid);
    WS_API_REMOVE(router, OnVoid);

//    find = router.Find<API_OnVoid>(KEY_OnVoid);
    find = WS_API_FIND(router, OnVoid);

    std::cout << (find ? "find topic" : "cannot find topic") << " no_input\n";
    
    
//    using MyFun2 = std::function<int(int input1, int input2)>;

    auto f2 = [=](const int& input1, const float& input2, bool b)->int{std::cout << "input: " << input1 << "\n" ; return 1; };
    
//    router.Add<API_OnLoginFinished>(KEY_OnLoginFinished, f2);
    WS_API_ADD(router, OnLoginFinished, f2);


//    find = router.Find<API_OnLoginFinished>(KEY_OnLoginFinished);
    find = WS_API_FIND(router, OnLoginFinished);


    std::cout << (find ? "find topic" : "cannot find topic") << " input\n";

    int i = 1;
//    router.Send<API_OnLoginFinished>(KEY_OnLoginFinished, {i, 2.0, true});
//    std::tuple<int, float, bool> input(1, 2.0, true);
//    WS_API_SEND(router, OnLoginFinished, input);
    WS_API_SEND(router, OnLoginFinished, WS_PARAS(i, 2.0, true));


//    router.Remove<API_OnLoginFinished>(KEY_OnLoginFinished);
    WS_API_REMOVE(router, OnLoginFinished);


//    find = router.Find<API_OnLoginFinished>(KEY_OnLoginFinished);
    find = WS_API_FIND(router, OnLoginFinished);

    std::cout << (find ? "find topic" : "cannot find topic") << " input\n";
    
    
    
    int x = 10;
    int* ptr = &x;
    std::string address_str = std::to_string(reinterpret_cast<uintptr_t>(ptr));
    std::cout << "Address of x: " << address_str << std::endl;

    
    return 0;
    
}
*/

// namespace TESTROUTER2
// {
int normalFunctionSubscrible1(int)
{
    std::cout << __FILE__ << ':' << __LINE__ << ':' << __FUNCTION__ << std::endl;
    return 0;
}

int normalFunctionSubscrible2(int)
{
    std::cout << __FILE__ << ':' << __LINE__ << ':' << __FUNCTION__ << std::endl;
    return 0;
}

int normalFunction(int, int)
{
    std::cout << __FILE__ << ':' << __LINE__ << ':' << __FUNCTION__ << std::endl;
    return 0;
}

class TestClass
{
public:
    void memberFunction(int, float)
    {
        std::cout << __FILE__ << ':' << __LINE__ << ':' << __FUNCTION__ << std::endl;
    }
    
    // void memberFunction(int, float) const
    // {
    //     std::cout << __FILE__ << ':' << __LINE__ << ':' << __FUNCTION__ << std::endl;
    // }
    
    static void staticFunction(int, float)
    {
        std::cout << __FILE__ << ':' << __LINE__ << ':' << __FUNCTION__ << std::endl;
    }
};

auto lambda = [](int) {
    std::cout << __FILE__ << ':' << __LINE__ << ':' << __FUNCTION__ << std::endl;
};

class functor
{
public:
    bool operator()()
    {
        std::cout << __FILE__ << ':' << __LINE__ << ':' << __FUNCTION__ << std::endl;
        return true;
    }
};
// };

char EVENT1[] = "1";


int main()
{
    TestClass x();
    {
        using MemberFuntionType = void (TestClass::*)(int, float);
        MemberFuntionType x = &TestClass::memberFunction;
        // decltype<TestClass::memberFunction> y = &TestClass::memberFunction;
        MessageRouter2::GetInstance()->Subscribe("normalFunction", normalFunctionSubscrible1);
        // MessageRouter2::GetInstance()->Subscribe("normalFunction", normalFunctionSubscrible2);
        MessageRouter2::GetInstance()->Subscribe("memberFunction", &TestClass::memberFunction, std::shared_ptr<TestClass>(new TestClass));
        // MessageRouter2::GetInstance()->Subscribe("lambda", lambda);
        // MessageRouter2::GetInstance()->Subscribe("functor", functor());

        MessageRouter2::GetInstance()->Boardcast("normalFunction", 1);
        MessageRouter2::GetInstance()->Boardcast("memberFunction", 2);
        // MessageRouter2::GetInstance()->Boardcast("lambda", 3);
        // MessageRouter2::GetInstance()->Boardcast("functor");
    }

    {
        // EventCenter<EVENT1>().Subscribe(normalFunctionSubscrible1);
        // // MessageRouter2::GetInstance()->Subscribe("normalFunction", normalFunctionSubscrible2);
        // EventCenter<EVENT1>().Subscribe(&TestClass::memberFunction, std::shared_ptr<TestClass>(new TestClass));

        // MessageRouter2::GetInstance()->Subscribe("memberFunction", &TestClass::memberFunction, std::shared_ptr<TestClass>(new TestClass));
        // MessageRouter2::GetInstance()->Subscribe("lambda", lambda);
        // MessageRouter2::GetInstance()->Subscribe("functor", functor());

        // MessageRouter2::GetInstance()->Boardcast("normalFunction", 1);
        // MessageRouter2::GetInstance()->Boardcast("memberFunction", 2);
        // MessageRouter2::GetInstance()->Boardcast("lambda", 3);
        // MessageRouter2::GetInstance()->Boardcast("functor");

    }
    // {
    //     MessageRouter2::GetInstance()->UnSubscribe("normalFunction", normalFunctionSubscrible1);
    
    //     MessageRouter2::GetInstance()->Boardcast("normalFunction", 1);
    //     MessageRouter2::GetInstance()->Subscribe("memberFunction", 2);
    //     MessageRouter2::GetInstance()->Subscribe("lambda", 3);
    //     MessageRouter2::GetInstance()->Subscribe("functor");
    // }

    // {
    //     MessageRouter2::GetInstance()->UnSubscribe("normalFunction", normalFunctionSubscrible2);
        
    //     MessageRouter2::GetInstance()->Boardcast("normalFunction", 1);
    //     MessageRouter2::GetInstance()->Subscribe("memberFunction", 2);
    //     MessageRouter2::GetInstance()->Subscribe("lambda", 3);
    //     MessageRouter2::GetInstance()->Subscribe("functor");
    // }

    return 0;
}

// int method2()
// {
//     MessageRouter2::GetInstance()->AddExector(std::enable_shared_from_this());
//     MessageRouter2::GetInstance()->Subscribe(key, $TestClass::memberFunction);
//     MessageRouter2::GetInstance()->RemoveExector(std::enable_shared_from_this());
//     MessageRouter2::GetInstance()->UnSubscribe(key, $TestClass::memberFunction);


// }