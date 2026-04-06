#include <stdio.h>

extern int MyPrint(const char* const format);

#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

int main()
{
    printf("Test number 1.\n"
           "Test name: " BLUE "only format string\n" RESET
           "\n"
           "Test results:\n"
           "MyPrint:    \n"
           "            string: " GREEN);

    int test = MyPrint("Mikle - the best engineer in the world");

    printf(RESET "\n      return value: " GREEN "%d\n"
                 "\n" RESET, test);
    printf("printf:     \n"
           "            string: " RED);

    int ans = printf("Mikle - the best engineer in the world");

    printf(RESET "\n      return value: " RED "%d\n" RESET, ans);


    return 0;
}
