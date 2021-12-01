// Elaboration on match-tree.cc using templates on Mem

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

    virtual void traverse() override
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

int main(void)
{
    sInt i1(new Int(42));
    sInt i2(new Int(21));
    sMem<Int> mem1(new Mem<Int>(i1));
    sMem<Mem<Int>> mem2(new Mem<Mem<Int>>(mem1));
    vTree<Int> t1 = mem1;
    vTree<Mem<Int>> t2 = mem2;

    std::visit(Matcher(), t1);
    std::visit(Matcher(), t2);
    return 0;
}
