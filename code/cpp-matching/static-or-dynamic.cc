// Demonstration that variants types are not checked statically.

#include <string>
#include <variant>

int main()
{
    std::variant<int, std::string> s = "forty-two";
    return std::get<int>(s);
}
