#include <iostream>

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

void test(C c)
{
    c.dynamic_f();
    c.static_f();
}

void test_ptr(C* c)
{
    c->dynamic_f();
    c->static_f();
}

int main()
{
    C true_c;
    Sub_C false_c;

    test(true_c);
    test(false_c);

    test_ptr(&true_c);
    test_ptr(&false_c);

    return 0;
}
