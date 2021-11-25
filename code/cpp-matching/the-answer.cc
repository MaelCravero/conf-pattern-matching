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
