#include <iostream>

// C is a class with both a classic and a virtual method. Only the virtual
// method may access the dynamic type of the object.
struct C
{
    void static_f()
    {
        std::cout << "static C" << std::endl;
    }

    virtual void dynamic_f()
    {
        std::cout << "dynamic C" << std::endl;
    }
};

// Sub_C is a subclass of C whic overrides its virtual method and shadows the
// static method.
struct Sub_C : public C
{
    void static_f()
    {
        std::cout << "not static C" << std::endl;
    }

    virtual void dynamic_f() override
    {
        std::cout << "not dynamic C" << std::endl;
    }
};

// Dynamic dispatch in C++ can only happen on pointers, passing C by copy like
// such means that there will only be static dispatch here.
void test(C c)
{
    c.dynamic_f();
    c.static_f();
}

// Here we pass a pointer, so the call to the virtual method will use dynamic
// dispatch and end up in the correct virtual method.
void test_ptr(C* c)
{
    c->dynamic_f();
    c->static_f();
}

int main()
{
    C true_c;
    Sub_C false_c;

    test(true_c); // static C, dynamic C
    test(false_c); // static C, dynamic C

    test_ptr(&true_c); // static C, dynamic C
    test_ptr(&false_c); // static C, not dynamic C

    return 0;
}
