Back to C++
===========

### Missing: Fonctionalité vraiment cool(tm)

- C++ has no pattern-matching.
- We will have to **emulate** it through other means.

\pause
Remember: static and dynamic types, templates, compile-time
monomorphization.

### Switches save the day!

```cpp
#include <iostream>

int main()
{
    int n = 42;
    switch (n)
    {
    case 42:
        std::cout << "The answer" << std::endl;
        break;
    default:
        std::cout << "Not the answer" << std::endl;
    }
    return 0;
}
```

### Well, only if processing primitive types...

```cpp
#include <iostream>

int main() {
    std::string s = "Hello there...";
    switch (s) {
    case "Hello there...":
        std::cout << "General Kenobi" << std::endl;
    default:
        break;
    }
    return 0;
}
```

\pause
This **does not** compile. `error: switch quantity not an integer`

### Remember algebraic types?

- **C++11** adds product types (`std::tuple`)
- **C++17** adds sum types (`std::variant`)

\pause
\vfill

```cpp
#include <string>
#include <tuple>
#include <variant>

int main()
{
    std::tuple<int, std::string> p = {0, "zero"};
    std::variant<int, std::string> s_int = 42;
    std::variant<int, std::string> s_string = "forty-two";
    return std::get<int>(s_int);
}
```

### Static or dynamic?

What happens there?

```cpp
#include <string>
#include <variant>

int main()
{
    std::variant<int, std::string> s = "forty-two";
    return std::get<int>(s);
}
```

\pause
It does compile.
\pause
But we get a runtime error. `std::get: wrong index for variant`

### Naive variants using unions

Examples inspired from [cppreference/union](https://en.cppreference.com/w/cpp/language/union).

```cpp
struct my_variant
{
    enum {INT, STRING} tag;
    union
    {
        int i;
        std::string s;
    };
};
```

\pause
`std::variant` is quite more complicated (`clang`'s `libcxx` [variant
header](https://github.com/llvm/llvm-project/blob/main/libcxx/include/variant)
is 1775 lines).

### Dispatching on variants

Remember, it's all **run-time**.

```cpp
void print(const my_variant& v)
{
    switch(s.tag)
    {
        case my_variant::INT: std::cout << v.i << '\n'; break;
        case my_variant::STRING: std::cout << v.s << '\n'; break;
    }
}

int main()
{
    my_variant v = {my_variant::INT, 42};
    print(v);
}
```

### The almighty `std::visit`

For `std::variant`, we can use `std::visit`.

```cpp
#include <variant>
#include <iostream>

int main()
{
    std::variant<int, std::string> v = 42;
    std::visit([](auto x){ std::cout << x << '\n';}, v);
}
```

### Where is Waldo?

Did you catch it? A **template** was hiding in the previous slide...

\pause

Let's desugar the lambda.

```cpp
struct Lambda {
    void operator()(auto x) {
        std::cout << x << '\n';
    }
};

int main() {
    std::variant<int, std::string> v = 42;
    std::visit(Lambda(), v);
}
```

### Warning: explicit

It was there all along!

```cpp
struct Lambda {
    template <typename T>
    void operator()(T x) {
        std::cout << x << '\n';
    }
};

int main() {
    std::variant<int, std::string> v = 42;
    std::visit(Lambda(), v);
}
```

### Our first real matcher

```cpp
struct A {};
struct B {};

struct Is_A
{
    bool operator()(A) { return true; }
    bool operator()(B) { return false; }
};

int main()
{
    std::variant<A, B> v = A();
    return std::visit(Is_A(), v); // returns 1
}
```

### Mandatory explicitness

- `std::visit` needs its visitor to handle **every possible type** for the
  variant visited
- This could get quite annoying if we had to explicitly handle every possible
  case...

\pause
\vfill
Here are the variants we're working with:
```cpp
using TreeVariant = misc::variant<rExp, rStm>;
using ExpVariant =
  misc::variant<rBinop, rCall, rConst, rEseq, rMem, rName, rTemp>;
using StmVariant =
  misc::variant<rCjump, rJump, rLabel, rMove, rSeq, rSxp>;
```

### Default cases thanks to templates

- Templates can be used for handling many types implicitly
- if we use a **templated method** in the visitor, it will **generate code** for
  every otherwise unmatched case

\pause
This is our poor man's wildcard. \pause Sort of.

### As easy as that

```cpp
struct A {};
struct B {};

struct Is_A
{
    bool operator()(A) { return true; }
    bool operator()(auto) { return false; }
};

int main()
{
    std::variant<A, B, int, float> v = 42;
    return std::visit(Is_A(), v); // returns 0
}
```

### Basic solution

Time to put it all together!

Let's see a simple recreation of the structure we use for instruction
scheduling.

### Basic solution

```cpp
struct Tree { virtual void traverse() = 0; };

using sTree = std::shared_ptr<Tree>;

struct Int : public Tree { ... };  // Leaf, no child
struct Mem : public Tree { ... };  // One child
struct Move : public Tree { ... }; // Two children

using sInt = std::shared_ptr<Int>;
using sMem = std::shared_ptr<Mem>;
using sMove = std::shared_ptr<Move>;

using vTree = std::variant<sMem, sMove, sInt>;
```

### Basic solution

```cpp
struct Matcher
{
    void operator()(const auto& t)
    {
        std::cout << "auto! ";
        t->traverse();
    }

    void operator()(const sMem& m)
    {
        std::cout << "sMem! ";
        m->traverse();
    }
};
```

### Basic solution

```cpp
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
```
