#include <iostream>
#include <variant>

struct Matcher
{
    void operator()(int a, auto b)
    {
        std::cout << "a is an int, b is whatever" << std::endl;
    }

    void operator()(auto a, auto b)
    {
        std::cout << "a and b are whatever" << std::endl;
    }
};

using my_variant = std::variant<int, float, bool>;

int main()
{
    my_variant a = 42;
    my_variant b = true;

    std::visit(Matcher(), a, b);

    return 0;
}
