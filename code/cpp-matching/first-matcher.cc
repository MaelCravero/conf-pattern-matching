#include <variant>

struct A
{};
struct B
{};

struct Is_A
{
    bool operator()(A)
    {
        return true;
    }
    bool operator()(B)
    {
        return false;
    }
};

int main()
{
    std::variant<A, B> v = A();
    return std::visit(Is_A(), v);
}
