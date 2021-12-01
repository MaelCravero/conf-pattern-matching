// An attempt at wrapping elements with type tags.
// Obviously won't work since template arguments must be constexpr, which is not
// the case for results of virtual methods.

#include <iostream>
#include <memory>
#include <variant>

struct Tree
{
    enum class Type
    {
        NONE,
        INT,
        MOVE,
        MEM,
    };

    virtual void traverse() = 0;
    virtual operator Type() = 0;

    Type type()
    {
        return *this;
    }
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

    virtual operator Type() override
    {
        return Type::INT;
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

    virtual operator Type() override
    {
        return Type::MEM;
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

    virtual operator Type() override
    {
        return Type::MOVE;
    }

    sTree dst;
    sTree src;
};

using sInt = std::shared_ptr<Int>;
using sMem = std::shared_ptr<Mem>;
using sMove = std::shared_ptr<Move>;

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

struct wInt
{
    sInt elt;
};

template <Tree::Type T>
struct wMem
{
    sMem elt;
};

template <Tree::Type D, Tree::Type S>
struct wMove
{
    sMove elt;
};

template <Tree::Type T1, Tree::Type T2>
using vTree = std::variant<wMem<T1>, wMove<T1, T2>, wInt>;

static vTree<Tree::Type::NONE, Tree::Type::NONE> make_variant(sInt i)
{
    return wInt{i};
}

template <Tree::Type T>
static vTree<T, Tree::Type::NONE> make_variant(sMem mem)
{
    return wMem<T>{mem};
}

template <Tree::Type D, Tree::Type S>
static vTree<D, S> make_variant(sMove move)
{
    return wMove<D, S>(move);
}

int main(void)
{
    sInt i1(new Int(42));
    sInt i2(new Int(21));
    sMem mem(new Mem(i1));
    sMove move(new Move(i2, mem));
    Tree::Type exp_type = *mem->exp;
    auto t1 = make_variant<exp_type>(mem); // not constexpr, will not compile

    return 0;
}
