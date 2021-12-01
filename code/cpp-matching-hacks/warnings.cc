// Unused arguments showcase for generated functions.

#include <iostream>
#include <variant>

int main()
{
    std::variant<int, float, double, std::string> v1 = 0;
    std::variant<int, float, double, std::string> v2 = "zero";
    std::visit([](auto a, auto b) { std::cout << "a and b are whatever\n"; },
               v1, v2);
    return 0;
}
