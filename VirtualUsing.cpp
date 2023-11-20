#include <memory>
#include <iostream>
#include <algorithm>
#include <cxxabi.h>
#include <string>
#include <type_traits>

using namespace std;

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

struct Base{
    using Type = int;
    virtual void f()
    {
        cout << demangle(typeid(this).name()) << endl;
    }
};

struct Derived1 : public Base{
    using Type = unsigned int;
    void f()
    {
        cout << demangle(typeid(this).name()) << endl;
    }
};

struct Derived2 : public Base{
    using Type = float;
    void f()
    {
        cout << demangle(typeid(this).name()) << endl;
    }
};

int main()
{
    // shared_ptr<Base> pa(new Base);
    // shared_ptr<Base> pb(new Derived1);
    // shared_ptr<Base> pc(new Derived2);

    shared_ptr<Base> pa = make_shared<Base>();
    shared_ptr<Base> pb = make_shared<Derived1>();
    shared_ptr<Base> pc = make_shared<Derived2>();

        // cout << demangle(typeid((*pa)).name()) << endl;
        // cout << demangle(typeid((*pb)).name()) << endl;
        // cout << demangle(typeid((*pc)).name()) << endl;

    vector<shared_ptr<Base>> p {pa, pb, pc};
    // vector<shared_ptr<Base>> p1 {new Base, new Derived1, new Derived2};
    for_each(p.cbegin(), p.cend(), [](shared_ptr<Base> _p){
        // cout << demangle(typeid(typename _p::Type).name() << endl;
        _p->f();
        cout << demangle(typeid((*_p)).name()) << endl;
        cout << demangle(typeid(decltype(_p)).name()) << endl;
        cout << demangle(typeid(decltype(_p.get())).name()) << endl;
        // decltype(*_p);
        // using TT = typename std::remove_pointer<decltype(*_p)>::type;
        // cout << demangle(typeid(TT).name()) << endl << endl;
    });
    
    return 0;
}