// Elaboration on match-tree-step2.cc with Move added back

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

template <typename D, typename S>
struct Move : public Tree
{
    using dst_t = std::shared_ptr<D>;
    using src_t = std::shared_ptr<S>;

    Move(dst_t dst, src_t src)
        : dst(dst)
        , src(src)
    {}

    virtual void traverse()
    {
        std::cout << "Move(";
        dst->traverse();
        std::cout << ",";
        src->traverse();
        std::cout << ")";
    }

    dst_t dst;
    src_t src;
};

using sInt = std::shared_ptr<Int>;

template <typename T>
using sMem = std::shared_ptr<Mem<T>>;

template <typename D, typename S>
using sMove = std::shared_ptr<Move<D, S>>;

template <typename T1, typename T2>
using vTree = std::variant<sMem<T1>, sMove<T1, T2>, sInt>;

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

    template <typename T>
    void operator()(const sMove<T, T>& m)
    {
        std::cout << "sMove with same type dst and src! ";
        m->traverse();
        std::cout << std::endl;
    }

    template <typename T1, typename T2>
    void operator()(const sMove<T1, T2>& m)
    {
        std::cout << "sMove with different type dst and src! ";
        m->traverse();
        std::cout << std::endl;
    }
};

template <typename T>
static sMem<T> make_mem(const std::shared_ptr<T>& exp)
{
    return sMem<T>(new Mem(exp));
}

template <typename D, typename S>
static sMove<D, S> make_move(const std::shared_ptr<D>& dst,
                             const std::shared_ptr<S>& src)
{
    return sMove<D, S>(new Move(dst, src));
}

static vTree<Tree, Tree> make_variant(sInt i)
{
    return i;
}

template <typename T>
static vTree<T, Tree> make_variant(sMem<T> mem)
{
    return mem;
}

template <typename D, typename S>
static vTree<D, S> make_variant(sMove<D, S> move)
{
    return move;
}

int main(void)
{
    sInt i1(new Int(42));
    sInt i2(new Int(21));

    auto mem1 = make_mem(i1);
    auto mem2 = make_mem(mem1);
    auto move1 = make_move(i2, mem2);
    auto move2 = make_move(i2, i1);

    auto t1 = make_variant(mem1);
    auto t2 = make_variant(mem2);
    auto t3 = make_variant(move1);
    auto t4 = make_variant(move2);
    auto t5 = make_variant(i1);

    std::visit(Matcher(), t1);
    std::visit(Matcher(), t2);
    std::visit(Matcher(), t3);
    std::visit(Matcher(), t4);
    std::visit(Matcher(), t5);

    return 0;
}
