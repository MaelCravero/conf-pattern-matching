#include <stdio.h>

void f(int n)
{

    switch (n)
    {
    case 42:
        puts("The answer");
        break;
    case 21:
        puts("Half an answer");
        break;
    default:
        puts("Not the answer");
    }

}

int main(void)
{
    int n = 42;
    f(n);
    return 0;
}

