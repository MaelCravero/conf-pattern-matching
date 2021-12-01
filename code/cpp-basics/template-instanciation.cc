// f is a templated function
// A different specialized function will be generated from it for every type
// possible in the template at compilation.
template <typename T>
void f()
{}

int main()
{
    f<int>(); // instantiate an int-specialized version of f
    f<float>(); // instantiate a float-specialized version of f

    // The binary will have two function definitions although we only wrote one.

    return 0;
}
