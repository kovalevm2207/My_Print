#include <stdio.h>
#include <assert.h>

extern int MyPrint(const char* const format);

#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

void TestFunc(const char* const TestType, const char* const Format, const int TestNum);

int main()
{
    TestFunc("clear format string",                       "Mike",                                 1);
    TestFunc("clear format string and check drop buffer", "Mike- the best engineer in the world", 2);

    return 0;
}

void TestFunc(const char* const TestType, const char* const Format, const int TestNum)
{
    assert(TestType); assert(Format);

    printf("Test number %d.\n"
           "Test name: " BLUE "%s\n" RESET
           "\n"
           "Test results:\n"
           "MyPrint:    \n"
           "            string: " GREEN, TestNum, TestType);

    int test = MyPrint(Format);

    printf(RESET "\n      return value: " GREEN "%d\n"
                 "\n" RESET, test);
    printf("printf:     \n"
           "            string: " RED);

    int ans = printf(Format);

    printf(RESET "\n      return value: " RED "%d\n" RESET
                 "--------------------------------------\n\n", ans);

    return;
}
