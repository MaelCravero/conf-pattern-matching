#include <variant>
// 1 type:  33 lines
// 2 types: 15078 lines
// 3 types: 387073 lines (201324007 characters)
// 4 types: ... (1440157310 characters)
// 5 types: could not get it to run (:
using variant = std::variant<int, float, char, double>;
int main()
{
    variant v = 0;
    std::visit(
        [](auto a, auto b, auto c, auto d, auto e, auto f, auto g, auto h) {},
        v, v, v, v, v, v, v, v);
}
