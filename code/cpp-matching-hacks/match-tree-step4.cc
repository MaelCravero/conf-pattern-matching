#include <iostream>
#include <memory>
#include <variant>

// Forward declarations
template <typename T1, typename T2>
struct Tree;

struct Int;

template <typename T>
struct Mem;

template <typename D, typename S>
struct Move;

// Smart pointers declarations
template <typename T1, typename T2>
using sTree = std::shared_ptr<Tree<T1, T2>>;

using sInt = std::shared_ptr<Int>;

template <typename T>
using sMem = std::shared_ptr<Mem<T>>;

template <typename D, typename S>
using sMove = std::shared_ptr<Move<D, S>>;

// Variant declaration
template <typename T1, typename T2>
using vTree = std::variant<sMem<T1>, sMove<T1, T2>, sInt>;

template <typename T1, typename T2>
struct Tree
{
    virtual void traverse() = 0;

    virtual vTree<T1, T2> variant() = 0;
};

struct Int : public Tree<void, void>
{
    Int(int val)
        : val(val)
    {}

    virtual void traverse() override
    {
        std::cout << val;
    }

    virtual vTree<void, void> variant() override
    {
        sInt res(this);
        return res;
    }

    int val;
};

template <typename T>
struct Mem : public Tree<T, void>
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

    virtual vTree<T, void> variant() override
    {
        sMem<T> res(this);
        return res;
    }

    exp_t exp;
};

template <typename D, typename S>
struct Move : public Tree<D, S>
{
    using dst_t = std::shared_ptr<D>;
    using src_t = std::shared_ptr<S>;

    Move(dst_t dst, src_t src)
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

    virtual vTree<D, S> variant() override
    {
        sMove<D, S> res(this);
        return res;
    }

    dst_t dst;
    src_t src;
};

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

int main(void)
{
    sInt i1(new Int(42));
    sInt i2(new Int(21));

    auto mem1 = make_mem(i1);
    auto mem2 = make_mem(mem1);
    auto move1 = make_move(i2, mem2);
    auto move2 = make_move(i2, i1);

    auto t1 = mem1->variant();
    auto t2 = mem2->variant();
    auto t3 = move1->variant();
    auto t4 = move2->variant();
    auto t5 = i1->variant();

    std::visit(Matcher(), t1);
    std::visit(Matcher(), t2);
    std::visit(Matcher(), t3);
    std::visit(Matcher(), t4);
    std::visit(Matcher(), t5);

    return 0;
}
