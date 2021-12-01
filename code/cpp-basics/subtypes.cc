// C is a class
struct C
{};

// Sub_C is a subclass of C
struct Sub_C : public C
{};

void f(C)
{}

void g(Sub_C)
{}

int main()
{
    C c;
    Sub_C sub;
    f(c);
    f(sub);
    g(c); // does not compile: the superclass cannot be used instead of a
          // subclass
    g(sub);

    return 0;
}
