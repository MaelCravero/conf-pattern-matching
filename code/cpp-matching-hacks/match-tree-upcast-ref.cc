// match-tree-upcast.cc with TC's misc::ref wrapper on shared instead of
// std::shared_ptr. This causes issues when using the unpatched version due to
// poor template instantiations. C++20 concepts can be used to fix this through
// template restrictions.

#include <iostream>
#include <variant>

#ifdef FIXED
#    include "lib/ref-fixed.hh"
#else
#    include "lib/ref.hh"
#endif

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

using sTree = misc::ref<Tree>;

struct Mem : public Tree
{
    Mem(sTree exp)
        : exp(exp)
    {}

    virtual void traverse() override
    {
        std::cout << "Move(";
        exp->traverse();
        std::cout << ")";
    }

    sTree exp;
};

struct Move : public Tree
{
    Move(sTree dst, sTree src)
        : dst(dst)
        , src(src)
    {}

    virtual void traverse()
    {
        std::cout << "Mem(";
        dst->traverse();
        std::cout << ",";
        src->traverse();
        std::cout << ")";
    }

    sTree dst;
    sTree src;
};

using sInt = misc::ref<Int>;
using sMem = misc::ref<Mem>;
using sMove = misc::ref<Move>;

using vTree = std::variant<sMem, sMove, sInt>;

struct Matcher
{
    void operator()(sTree t)
    {
        std::cout << "sTree! ";
        t->traverse();
        std::cout << std::endl;
    }

    void operator()(sMem m)
    {
        std::cout << "sMem! ";
        m->traverse();
        std::cout << std::endl;
    }

    void operator()(misc::ref<int>)
    {}

    // if using "lib/ref.hh", does not compile without this
    // void operator()(auto t)
    //{
    //    std::cout << "auto :( ";
    //    t->traverse();
    //    std::cout << std::endl;
    //}
};

int main(void)
{
    sInt i1(new Int(42));
    sInt i2(new Int(21));
    sMem mem(new Mem(i1));
    sMove move(new Move(i2, mem));
    vTree t1 = move;
    vTree t2 = mem;

    Matcher()(mem);
    Matcher()(move);

#ifdef FIXED
    std::visit(Matcher(), t1);
    std::visit(Matcher(), t2);
#endif

    return 0;
}
