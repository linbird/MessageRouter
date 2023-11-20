#include <iostream>

// 声明通用的模板
template <typename T>
struct MyClassWrapper;

template <typename Ret, typename Class, typename... Args>
struct MyClassWrapper<Ret(Class::*)(Args...)> {
    using ReturnType = Ret;
    using ClassType = Class;
    using FuncType = Ret(Class::*)(Args...);
};

// // 模板特化，匹配 MyClass 类的成员函数
// template <typename Ret, typename... Args>
// struct MyClassWrapper<Ret(MyClass::*)(Args...)> {
//     using ReturnType = Ret;
//     using ClassType = MyClass;
// };

// // 模板特化，匹配 AnotherClass 类的成员函数
// template <typename Ret, typename... Args>
// struct MyClassWrapper<Ret(AnotherClass::*)(Args...)> {
//     using ReturnType = Ret;
//     using ClassType = AnotherClass;
// };

// 使用模板
template <typename FuncPtr>
void processFunction(FuncPtr func) {
    // 使用 MyClassWrapper 进行特化匹配
    using Wrapper = MyClassWrapper<FuncPtr>;

    // 使用 Wrapper::ClassType 和 Wrapper::ReturnType
    std::cout << "ClassType: " << typeid(typename Wrapper::ClassType).name() << std::endl;
    std::cout << "ReturnType: " << typeid(typename Wrapper::ReturnType).name() << std::endl;
    std::cout << "FuncType: " << typeid(typename Wrapper::FuncType).name() << std::endl;
}

class MyClass {
public:
    void myMemberFunction(int) {}
};

class AnotherClass {
public:
    double anotherMemberFunction(char) { return 0.0; }
};

class AC {
public:
    double anotherMemberFunction(char) { return 0.0; }
};

int main() {
    // 使用模板处理 MyClass 的成员函数
    processFunction(&MyClass::myMemberFunction);

    // 使用模板处理 AnotherClass 的成员函数
    processFunction(&AnotherClass::anotherMemberFunction);

    processFunction(&AC::anotherMemberFunction);


}
