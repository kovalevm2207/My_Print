#include <stdio.h>
#include <assert.h>

extern int MyPrint(const char* const format, ...);

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
    // Базовые тесты с обычными строками
    {"Empty string", ""},
    {"One character", "A"},
    {"Short string less than buffer", "Hello"},
    {"String exactly buffer size", "12345678"},
    {"String longer than buffer (9 chars)", "123456789"},
    {"String much longer than buffer", "Hello, World!"},

    // Тесты с табуляцией \t
    {"Tab at beginning", "\tstart"},
    {"Tab in middle", "Hello\tWorld"},
    {"Tab at end", "Hello\t"},
    {"Multiple tabs", "\t\t\t"},

    // Тесты с новой строкой \n
    {"Newline at beginning", "\nstart"},
    {"Newline in middle", "Hello\nWorld"},
    {"Newline at end", "Hello\n"},
    {"Multiple newlines", "\n\n\n"},

    // Тесты с backspace \b
    {"Backspace at beginning", "\bstart"},
    {"Backspace in middle", "Hello\bWorld"},
    {"Backspace at end", "Hello\b"},

    // Тесты с carriage return \r
    {"Carriage return at beginning", "\rstart"},
    {"Carriage return in middle", "Hello\rWorld"},
    {"Carriage return at end", "Hello\r"},

    // Тесты с form feed \f
    {"Form feed at beginning", "\fstart"},
    {"Form feed in middle", "Hello\fWorld"},

    // Тесты с vertical tab \v
    {"Vertical tab at beginning", "\vstart"},
    {"Vertical tab in middle", "Hello\vWorld"},

    // Тесты с bell \a
    {"Bell at beginning", "\astart"},
    {"Bell in middle", "Hello\aWorld"},

    // Тесты с escape \e
    {"Escape at beginning", "\estart"},
    {"Escape in middle", "Hello\eWorld"},

    // Тесты с обратным слешем \\
    {"Backslash at beginning", "\\start"},
    {"Backslash in middle", "Hello\\World"},
    {"Backslash at end", "Hello\\"},
    {"Multiple backslashes", "\\\\\\"},

    // Тесты с кавычками
    {"Double quote", "\"Hello\""},
    {"Single quote", "\'Hello\'"},
    {"Mixed quotes", "\"\'Hello\'\""},

    // Комбинированные тесты
    {"Mixed escape sequences", "Hello\tWorld\nNew\rLine\b"},
    {"Escape sequence crossing buffer boundary", "1234567\n"},
    {"Escape sequence at buffer boundary", "1234567\\t"},
    {"Multiple escape sequences", "\t\n\r\b\f\v"},

    // Тесты с граничными условиями буфера (8 байт)
    {"Buffer boundary: 7 chars + tab", "1234567\t"},
    {"Buffer boundary: 6 chars + newline", "123456\n"},
    {"Buffer boundary: 5 chars + backspace+2", "12345\b67"},
    {"Buffer exactly full then escape", "12345678\t"},

    // Тесты с длинными строками
    {"Long string with escapes", "Start\tMiddle\nEnd\r\b\t\\\"\'"},
    {"Very long string", "This is a very long string with multiple\t\nescape\nsequences\rto\btest\fthe\vbuffer\amanagement\\\"\'"},

    // Тесты с комбинацией обычных символов и спецсимволов
    {"Mix of normal and special", "ABC\tDEF\nGHI\rJKL\bMNO\\PQR\"STU\'VWX"},

    // Пограничные случаи
    {"Only escape sequence", "\n"},
    {"Only backslash", "\\"},
    {"Backslash at end of string", "Hello World\\"},
    {"Empty escapes", "\\a\\b\\t\\n\\v\\f\\r\\e\\\\\\\'\\\""},

};

const size_t NumOfTests = sizeof(Tests)/sizeof(Tests[0]);

int main()
{
    //for (int i = 0; i < NumOfTests; i++)
    //    TestFunc(Tests[i].TestType, Tests[i].Format, i);

    int res0 = MyPrint("First_character_a_=_%c_bananaaaa_%c_\n", 'a', 'b'); printf("res = %d\n",   res0);
    int res1 =  printf("First_character_a_=_%c_bananaaaa_%c_\n", 'a', 'b'); printf("res = %d\n\n", res1);
    int res2 = MyPrint("First_string_=_%s_%s_\n", "my", "banana");          printf("res = %d\n",   res2);
    int res3 =  printf("First_string_=_%s_%s_\n", "my", "banana");          printf("res = %d\n\n", res3);

    return 0;
}

void TestFunc(const char* const TestType, const char* const Format, const int TestNum)
{
    assert(TestType); assert(Format);

    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Start\n"
           "Test number %d.\n"
           "Test name: " BLUE "%s\n" RESET
           "\n"
           "Test results:\n"
           "MyPrint:    \n"
           "            string:\n"
           "--------------------------------------\n" GREEN, TestNum, TestType);

    int test = MyPrint(Format);

    printf(RESET "--------------------------------------\n"
                 "      return value: " GREEN "%d\n"
                 "\n" RESET, test);
    printf("printf:     \n"
           "            string:\n"
           "--------------------------------------\n" RED);

    int ans = printf(Format);

    printf(RESET "--------------------------------------\n"
                 "\n      return value: " RED "%d\n" RESET
                 "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End\n\n", ans);

    return;
}
