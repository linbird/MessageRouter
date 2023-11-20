#include <iostream>
#include <memory>
#include <typeinfo>

class Base {
public:
    virtual ~Base() {} // 虚析构函数，确保正确的析构顺序
};

class Derived : public Base {
    // 派生类的成员
};

int main() {
    std::shared_ptr<Base> basePtr = std::make_shared<Derived>(); // 使用智能指针指向派生类对象

    // 使用typeid获取派生类的类型信息
    const std::type_info& typeInfo = typeid(*basePtr);

    // 输出类型信息的名称
    std::cout << "Derived class type: " << typeInfo.name() << std::endl;

    // 智能指针会自动释放内存，无需手动delete

    return 0;
}
