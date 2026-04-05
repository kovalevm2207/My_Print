#include <stdio.h>

extern int MyPrint(const char* const format);

int main()
{
    printf("(only string)\n\n");
    int test1 = MyPrint("Mikle - the best engineer in the world");
    printf("\ntest1 = %d\n\n", test1);

    return 0;
}
