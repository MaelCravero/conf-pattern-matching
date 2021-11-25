#include <iostream>
#include <variant>

struct Lambda
{
    // void operator()(auto x)
    // {
    //     std::cout << x << '\n';
    // }

    template <typename T>
    void operator()(T x)
    {
        std::cout << x << '\n';
    }
};

int main()
{
    std::variant<int, std::string> v = 42;
    std::visit(Lambda(), v);
}
