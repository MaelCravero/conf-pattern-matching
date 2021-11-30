#include <iostream>
#include <memory>
#include <variant>

struct Tree
{
    virtual void traverse() = 0;
};

struct Int : public Tree
{
    Int(int val)
        : val(val)
    {}

    virtual void traverse()
    {
        std::cout << val;
    }

    int val;
};

using sTree = std::shared_ptr<Tree>;

template <typename T>
struct Mem : public Tree
{
    using exp_t = std::shared_ptr<T>;

    Mem(exp_t exp)
        : exp(exp)
    {}

    virtual void traverse() override
    {
        std::cout << "Mem(";
        exp->traverse();
        std::cout << ")";
    }

    exp_t exp;
};

using sInt = std::shared_ptr<Int>;

template <typename T>
using sMem = std::shared_ptr<Mem<T>>;

template <typename T>
using vTree = std::variant<sMem<T>, sInt>;

struct Matcher
{
    void operator()(const auto& t)
    {
        std::cout << "auto! ";
        t->traverse();
        std::cout << std::endl;
    }

    template <typename T>
    void operator()(const sMem<Mem<T>>& m)
    {
        std::cout << "sMem with a sMem child! ";
        m->traverse();
        std::cout << std::endl;
    }

    template <typename T>
    void operator()(const sMem<T>& m)
    {
        std::cout << "sMem! ";
        m->traverse();
        std::cout << std::endl;
    }
};

template <typename T>
static sMem<T> make_mem(const std::shared_ptr<T>& exp)
{
    return sMem<T>(new Mem(exp));
}

template <typename T>
static vTree<T> variant_mem(sMem<T> mem)
{
    return mem;
}

int main(void)
{
    sInt i1(new Int(42));
    sInt i2(new Int(21));

    auto mem1 = make_mem(i1);
    auto mem2 = make_mem(mem1);
    auto t1 = variant_mem(mem1);
    auto t2 = variant_mem(mem2);

    std::visit(Matcher(), t1);
    std::visit(Matcher(), t2);

    return 0;
}
