#include <iostream>

//enable_if_t

using IndifyToken = const char*;

template <IndifyToken token>
struct EventInstace
{
    // template <typename RetType, typename CallerClassType, typename ...Args>
    // void Subscribe(RetType(CallerClassType::*)(Args...) callback, std::shared_ptr<CallerClassType> caller)
    template <typename CallbackType, typename CallerClassType>
    void Subscribe(CallbackType callback, std::shared_ptr<CallerClassType> caller)
    {
        std::cout << __FILE__ << ':' << __LINE__ << ':' << __FUNCTION__ << std::endl;
    }

    template <typename ...Args>
    void BroadCast(Args ...args)
    {
        std::cout << __FILE__ << ':' << __LINE__ << ':' << __FUNCTION__ << std::endl;
    }

    template <typename ...Args, typename CallerClassType>
    void BroadCast(Args ...args)
    {
        std::cout << __FILE__ << ':' << __LINE__ << ':' << __FUNCTION__ << std::endl;
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
            std::cout << __FILE__ << ':' << __LINE__ << ':' << __FUNCTION__ << std::endl;

    EventInstace<Event1> eventInstace1;
    eventInstace1.Subscribe(&TestClass::memberFunction, std::make_shared<TestClass>());
    eventInstace1.BroadCast(1, 2.0);
            std::cout << __FILE__ << ':' << __LINE__ << ':' << __FUNCTION__ << std::endl;

    return 0;
}