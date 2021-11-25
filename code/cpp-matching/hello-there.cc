#include <iostream>

int main()
{
    std::string s = "Hello there...";
    switch (s)
    {
    case "Hello there...":
        std::cout << "General Kenobi" << std::endl;
        break;
    default:
        break;
    }

    return 0;
}
