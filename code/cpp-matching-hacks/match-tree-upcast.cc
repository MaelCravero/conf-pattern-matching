// Based on match-tree.cc using upcasts of shared_ptr instead of auto functions

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

struct Mem : public Tree
{
    Mem(sTree exp)
        : exp(exp)
    {}

    virtual void traverse() override
    {
        std::cout << "Mem(";
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

    virtual void traverse() override
    {
        std::cout << "Move(";
        dst->traverse();
        std::cout << ",";
        src->traverse();
        std::cout << ")";
    }

    sTree dst;
    sTree src;
};

using sInt = std::shared_ptr<Int>;
using sMem = std::shared_ptr<Mem>;
using sMove = std::shared_ptr<Move>;

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
};

struct MatcherAuto : public Matcher
{
    using Matcher::operator();

    void operator()(auto t)
    {
        std::cout << "auto :( ";
        t->traverse();
        std::cout << std::endl;
    }
};

int main(void)
{
    sInt i1(new Int(42));
    sInt i2(new Int(21));
    sMem mem(new Mem(i1));
    sMove move(new Move(i2, mem));
    vTree t1 = move;
    vTree t2 = mem;

    sTree t(i1);

    std::visit(Matcher(), t1);
    std::visit(MatcherAuto(), t1);
    std::visit(Matcher(), t2);
    std::visit(MatcherAuto(), t2);
    return 0;
}
