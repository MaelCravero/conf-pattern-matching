// Basic exemple of matching on trees.

#include <iostream>
#include <memory>
#include <variant>

//------------------------------------------------------------------//
//                     Tree classes definitions                     //
//------------------------------------------------------------------//

/// Tree is the abstract class all nodes inherit from.
struct Tree
{
    virtual void traverse() = 0;
};

using sTree = std::shared_ptr<Tree>;

/// Int is a leaf, it represents an immediate value.
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

/// Mem is a node with one child, it represents a memory access.
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

/// Move is a node with two children, it represents an assembly move.
struct Move : public Tree
{
    Move(sTree dst, sTree src)
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

    sTree dst;
    sTree src;
};

//------------------------------------------------------------------//
//                  Smart pointer types defintions                  //
//------------------------------------------------------------------//

using sInt = std::shared_ptr<Int>;
using sMem = std::shared_ptr<Mem>;
using sMove = std::shared_ptr<Move>;

//------------------------------------------------------------------//
//                        Variant definition                        //
//------------------------------------------------------------------//

using vTree = std::variant<sMem, sMove, sInt>;

//------------------------------------------------------------------//
//                        Matcher definition                        //
//------------------------------------------------------------------//

struct Matcher
{
    void operator()(const auto& t)
    {
        std::cout << "auto! ";
        t->traverse();
        std::cout << std::endl;
    }

    void operator()(const sMem& m)
    {
        std::cout << "sMem! ";
        m->traverse();
        std::cout << std::endl;
    }
};

//------------------------------------------------------------------//
//                          Main function                           //
//------------------------------------------------------------------//

int main(void)
{
    sInt i1(new Int(42));
    sInt i2(new Int(21));

    sMem mem(new Mem(i1));

    sMove move(new Move(i2, mem));

    vTree t1 = move;
    vTree t2 = mem;

    std::visit(Matcher(), t1); // auto!
    std::visit(Matcher(), t2); // sMem!
    return 0;
}
