Hacking C++ (badly) to emulate OCaml (poorly)
=============================================

### Down the rabbit hole

The previous example was neat, but it had no destructuring.

\pause
![Instruction selection in ML](img/munchStm.png){height=40%}

\pause
The whole instruction selection process is based on this...

### Back to MonoBURG

Since 2005, instruction selection was done using
[MonoBURG](https://gitlab.lrde.epita.fr/tiger/monoburg) which generates nested
C++ switches from input files.

```cpp
binop: Binop(left : exp, right : Const)
{
    rBinop binop = tree.cast<Binop> ();
    rConst cst = right.cast<Const> ();

    // ...
}

binop: Binop(left : exp, right : exp)
{
    // ...
}
```

### From nested switches...

```cpp
switch(tree->type) {
case Tree::BINOP:
    auto binop = tree.cast<Binop>();
    switch(binop->right->type) {
    case Tree::CONST:
        auto cst = right.cast<Const>();
        // ...
    }
// ...
}
```

### ...to nested visits

```cpp
void GasMatcher::operator()(tree::rBinop binop)
{
    auto left = binop->left_get()->variant(binop->left_get());
    auto right = binop->right_get()->variant(binop->right_get());

    tree::ExpVariant::visit(
        misc::LambdaVisitor{
            [&](auto lhs, tree::rConst cst) { /* ... */ },
            [&](auto lhs, auto rhs) { /* ... */ },
        },
        left, right);
}
```
\pause
Still no destructuring though...

### Not so trivial

- Our "pattern-matching" is a visitor dispatching on specific types
- Dispatch is only based on the **static type** stored in the variant
- How do we dispatch based on the type of a node's children if we can only
  dispatch on the node's static type?

In other words, how can the static type of a node depend on the type of its
children?

### A hint of an answer

- What's the type of...
\pause
    - `int`
\pause
    - `std::string`
\pause
    - `std::vector<int>`
\pause
    - `std::vector<std::string>`
\pause
    - `std::tuple<int, std::string>`
\pause
    - `std::tuple<std::string>`
\pause
\vfill

- Which of these denote the same type? \pause Obviously, *none*.
\vfill

\pause
**Monomorphization** applies to types too!

Templated types denote **multiple types** that can be **generated** at run-time
through template instantiation.

### TODO

- If we template nodes based on the children's type, we create specified types!
- We could then do specific matching functions for specific types and subtypes!

Seems simple! Here's what we have to do:

1. Template the nodes with children
\pause
2. Template the smart pointer wrapping the node
\pause
3. Template the variant wrapping the smart pointer
\pause
4. Specify the templates type at definition
\pause
5. Cry over the sheer number of templates everywhere

### A quick look

```cpp
template <typename T> struct Mem : public Tree { ... };

template <typename T>
using sMem = std::shared_ptr<Mem<T>>;

template <typename T>
using vTree = std::variant<sMem<T>, sInt>;
```

### A quick look

```cpp
struct Matcher
{
    void operator()(const auto& t) { ... }

    template <typename T>
    void operator()(const sMem<Mem<T>>& m) { ... }

    template <typename T>
    void operator()(const sMem<T>& m) { ... }
};
```

### A quick look

```cpp
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
```
### We're in too deep

You should be asking yourselves some questions:

- Are we doing things right?
\pause
- Do we really have to template everything?
\pause
- Do we really have to explicitly state all types?
\pause
- Can't we just go back to OCaml?
\pause
- Why are we here just to suffer?

### One word: Class Template Argument Deduction

- Did you notice the `auto` keyword I use everywhere?
- This is used for type inference \pause (like in OCaml but less good)
\pause
- What if we were able to use `auto` as a template parameter?

\pause
Well, unfortunately we can't. Instead we have **template argument deduction**.

\pause
Since C++17, if we **omit** the template arguments in certain contexts (like
definitions) they can be **inferred**.

\pause
But this can't be used everywhere...

### Factory functions for type deduction
```cpp
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
```

### Factory functions for type deduction
```cpp
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
```
### Using the dynamic type in variants

What if a node's static type differs from it's dynamic type?

```cpp
using vTree = std::variant<sInt, sMem>;
sTree t = make_mem(sInt(new Int(42)));
vTree v = t; // Will not compile: sTree is not in vTree
```

When declaring the variant, only the value's static type is taken into account,
but we want to access the dynamic type.

\pause
To access the dynamic type, we have to use virtual methods.

### More templates

```cpp
// [...] (forward declarations)

template <typename T1, typename T2>
using vTree = std::variant<sMem<T1>, sMove<T1, T2>, sInt>;

template <typename T1, typename T2>
struct Tree
{
    virtual void traverse() = 0;

    virtual vTree<T1, T2> variant() = 0;
};
```

### More templates

```cpp
// Dummy class
struct None : public Tree<None, None> { ... };

struct Int : public Tree<None, None> { ... };

template <typename T>
struct Mem : public Tree<T, None> { ... };

template <typename D, typename S>
struct Move : public Tree<D, S> { ... };
```

### More templates

```cpp
int main(void)
{
    sInt i1(new Int(42));

    auto mem1 = make_mem(i1);
    auto t1 = mem1->variant();
    std::visit(Matcher(), t1);

    sTree<Int, None> tree = mem1;
    auto t2 = tree->variant();
    std::visit(Matcher(), t2); // Same result

    return 0;
}
```

### Memory issues

- The previous example outputs the right result
\pause
- But we have a memory error

```
free(): double free detected in tcache 2
Aborted (core dumped)
```

\pause
This is due to our use of `std::shared_ptr`, the C++ *smart pointers*.

### Smarter pointers

- C++ is not garbage collected, so we must free memory
- Shared pointers can be used for memory management
- They are automatically freed using **reference counting**

Each shared pointer points to a dynamically allocated resource and contains a
counter to the number of shared pointers referencing it

\vfill
\pause
```cpp
A* raw_a = new A;
std::shared_ptr<A> ptr1(raw_A); // use_count = 1
std::shared_ptr<A> ptr2(ptr1);  // use_count = 2
```

\vfill
\pause
- When `ptr1` or `ptr2` goes out of scope, `use_count` is decremented.
- If it goes to 0, the resource is deallocated.

### Not so smart

The use count is actually propagated only through copy construction.

\vfill
\pause
What happens here?

```cpp
A* raw_a = new A;
std::shared_ptr<A> ptr1(raw_A); // ptr1's use_count = 1
std::shared_ptr<A> ptr2(raw_A); // ptr2's use_count = 1
```

\vfill
\pause
- `ptr1` and `ptr2`'s use count are distinct here...
- If one goes out of scope the resource will be deallocated, and when the second
  one attempts to free it we get an error.

### C++ black magic

- Thanks to `std::enable_shared_from_this` we can fix this issue.
- Using the provided `shared_from_this` method returns a smart pointer with the
  correct reference count.
- It can then be propagated through copy construction, as usual.

### Before...

```cpp
struct Int : public Tree<None, None>
{
    // ...

    virtual vTree<None, None> variant() override
    {
        sInt res(this);
        return res;
    }

    int val;
};
```

### ...after

```cpp
struct Int
    : public Tree<None, None>
    , std::enable_shared_from_this<Int>
{
    // ...

    virtual vTree<None, None> variant() override
    {
        sInt res(this->shared_from_this());
        return res;
    }

    int val;
};
```

### Note

- This does fix memory issues.
- ...but this does not work in TC, probably due to our wrapper class on
  `std::shared_ptr`.
- Instead, we pass the shared pointer as argument to the virtual method to
  propagate its use count...

```cpp
auto left = binop->left_get()->variant(binop->left_get());
```

### Monomorphization does not suppress warnings...

Let's imagine we have some code like that:

```cpp
int main()
{
    std::variant<int, float, double, std::string> v1 = 0;
    std::variant<int, float, double, std::string> v2 = "zero";
    std::visit([](auto a, auto b) {
                  std::cout << "a and b are whatever\n"; },
               v1, v2);
    return 0;
}
```

\pause
We have two unused arguments, who cares right?

\pause
This **11 lines** code generates **360 lines** of unused arguments warnings.

### ...or errors

- Monomorphization will generate a function for **every possible combination** of
  argument types for the lambda
- 2 arguments and 4 possible types so $2^4$ generated functions $\times$ 2
  unused arguments $=$ 32 warnings
\pause
- Each type being a long generated type also adds to the message's length

\pause
Here this is only warnings, but imagine if we had template errors inside of the
lambda...

### An exercise in absurdity

```cpp
#include <variant>
using variant = std::variant<int, float, char, bool>;
int main()
{
    variant v = 0;
    std::visit(
        [](auto a, auto b, auto c, auto d,
           auto e, auto f, auto g, auto h) {},
        v, v, v, v, v, v, v, v);
}
```

### Subtyping gets rid of templates!

- We can upcast shared pointers on `Tree` subclasses to shared pointers to
  `Tree`
- This means we can remove the `auto` for default cases!
- Without `auto`, no more monomorphization and way shorter error messages

### Example

```cpp
struct Matcher
{
    void operator()(sTree t) { ... }

    void operator()(sMem m) { ... }
};

int main(void)
{
    sInt i1(new Int(42));
    vTree t1 = i1;
    std::visit(Matcher(), t1);
    return 0;
}
```

### ...but not in TC
- This does not work in TC due to our wrapper class on `std::shared_ptr`.
\pause
- This is due to poor template instantiation which can be fixed using C++20
concepts (a mechanism for adding constraints to templates).

### Aren't we missing something?

If you paid attention, the last example did not feature any destructuring.

\pause
- Implementing it features templates heavily, which does complicates everything.
\pause
- And our `Tree` implementation in TC is not templated...

\pause
This is bad news.

### Let's rephrase

- Templates are fully static, so compile-time only
- Dynamic types are, well, dynamic, so run-time only

How can we generate at compile-time types which are only known at run-time?

\vfill

Three ideas:

\pause

1. Scrap it and template the whole tree class

\pause

2. Statically handle every possible run-time case

\pause

3. Get type tags from virtual methods and use them as template parameter

### The template hell

- Templates add a lot of complexity to the code you write
- The tree structure is an generated from an AST anyway, so the problem is the
  same
- How can we even handle every possible tree structure at compilation anyway?

\pause
Imagine this templated type (and associated tree):

```
Mem<Mem<Mem<Mem<Mem<Mem<Move<Int, Move<Int, Mem<Mem<...>>>>>>>>>>
```

The **static** type would have to contain the content of the whole **dynamic**
tree... This makes no sense.

\pause
#### Note
Using only a small depth might work but it still would involve a lot of
complexity, both at generation and at use.

### There can't be that many cases, right?

Handling statically every possible dynamic case might be possible.

- Go through each node
- Go though each of its children
- Depending on all types, instantiate a template statically
\pause
- So we just end up with nested visits to generate templated types... \pause So
  that we can visit them again?
\pause
- Why not just perform nested visits then?

\pause
This is a working solution, but adds too much unnecessary complexity.

### Dynamic and static don't match

What's wrong with this?

```cpp
template <int N> struct S {};
int main(int argc, char *argv[])
{
    S<1000> s1;
    S<argc> s2;
}
```
\pause
Templates are **compile-time** only. What's the value of `argc` at compilation?

\pause
- This will not compile because every template parameter must be known at
  compilation.
\pause
- But virtual method calls are run-time only...
\pause
- So we can't use virtual methods results for templating.

### Wasted

What we try to do here cannot work.

\pause

- We can't mix dynamic and static polymorphism at will, it's not
  the way they work.
\pause
- We can't generate templated types from types unknown at compilation, the basis
  basis of our pattern-matching destructuration.
\pause

We'll have to stick to more basic matching, but that's okay: C++ is just not the
language for the job.
