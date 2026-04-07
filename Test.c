#include <stdio.h>
#include <assert.h>

extern int MyPrint(const char* const format);

#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

void TestFunc(const char* const TestType, const char* const Format, const int TestNum);

const struct
{
    const char* const TestType;
    const char* const Format;
} Tests[] =
{
    {"clear format string", "Mike"},
    {"special situation","12345678"},
    {"clear format string and check drop buffer", "Mike- the best engineer in the world"}
};

const size_t NumOfTests = sizeof(Tests)/sizeof(Tests[0]);

int main()
{
    for (int i = 0; i < NumOfTests; i++)
        TestFunc(Tests[i].TestType, Tests[i].Format, i);
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
           "            string:\n" GREEN, TestNum, TestType);

    int test = MyPrint(Format);

    printf(RESET "\n      return value: " GREEN "%d\n"
                 "\n" RESET, test);
    printf("printf:     \n"
           "            string:\n" RED);

    int ans = printf(Format);

    printf(RESET "\n      return value: " RED "%d\n" RESET
                 "--------------------------------------\n\n", ans);

    return;
}
