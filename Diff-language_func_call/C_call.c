#include <stdio.h>

extern int MyAdd(int a, int b);

int main()
{
    int a = 3, b = -2;
    printf("in value: a = %d, b = %d\n\n", a, b);

    int c = MyAdd(a, b);

    printf("out value: a+b = %d\n", c);

    return 0;
}
