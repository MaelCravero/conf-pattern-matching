#include <string>
#include <tuple>
#include <variant>

int main()
{
    std::tuple<int, std::string> p = {0, "zero"};
    std::variant<int, std::string> s_int = 42;
    std::variant<int, std::string> s_string = "forty-two";
    return std::get<int>(s_int);
}
