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
    int my_res, std_res;

    printf("========================================\n");
    printf("         COMPLETE TEST SUITE            \n");
    printf("========================================\n\n");

    // ==========================================
    //               %c - CHARACTER
    // ==========================================
    printf("=== %%c - CHARACTER TESTS ===\n\n");

    // Обычные символы
    my_res = MyPrint("Char basic: _%c_ _%c_ _%c_\n", 'A', 'z', '0');
    std_res = printf("Char basic: _%c_ _%c_ _%c_\n", 'A', 'z', '0');
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Специальные символы
    my_res = MyPrint("Char special: _%c_ _%c_ _%c_\n", '\n', '\t', '\\');
    std_res = printf("Char special: _%c_ _%c_ _%c_\n", '\n', '\t', '\\');
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Граничные значения
    my_res = MyPrint("Char bounds: _%c_ _%c_\n", 0, 255);
    std_res = printf("Char bounds: _%c_ _%c_\n", 0, 255);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Много символов подряд
    my_res = MyPrint("Char multiple: _%c%c%c%c%c_\n", 'H', 'e', 'l', 'l', 'o');
    std_res = printf("Char multiple: _%c%c%c%c%c_\n", 'H', 'e', 'l', 'l', 'o');
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // ==========================================
    //               %s - STRING
    // ==========================================
    printf("=== %%s - STRING TESTS ===\n\n");

    // Обычные строки
    my_res = MyPrint("String basic: _%s_ _%s_\n", "Hello", "World");
    std_res = printf("String basic: _%s_ _%s_\n", "Hello", "World");
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Пустая строка
    my_res = MyPrint("String empty: _%s_\n", "");
    std_res = printf("String empty: _%s_\n", "");
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Длинные строки
    my_res = MyPrint("String long: _%s_\n",
        "This is a very long string that exceeds the buffer size to test the buffer overflow handling mechanism");
    std_res = printf("String long: _%s_\n",
        "This is a very long string that exceeds the buffer size to test the buffer overflow handling mechanism");
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Строки со спецсимволами
    my_res = MyPrint("String with special: _%s_\n", "Line1\nLine2\tTabbed");
    std_res = printf("String with special: _%s_\n", "Line1\nLine2\tTabbed");
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // ==========================================
    //               %d - DECIMAL
    // ==========================================
    printf("=== %%d - DECIMAL TESTS ===\n\n");

    // Ноль и единица
    my_res = MyPrint("Decimal zero/one: _%d_ _%d_ _%d_\n", 0, 1, -1);
    std_res = printf("Decimal zero/one: _%d_ _%d_ _%d_\n", 0, 1, -1);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Положительные числа
    my_res = MyPrint("Decimal positive: _%d_ _%d_ _%d_\n", 42, 12345, 1000000);
    std_res = printf("Decimal positive: _%d_ _%d_ _%d_\n", 42, 12345, 1000000);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Отрицательные числа
    my_res = MyPrint("Decimal negative: _%d_ _%d_ _%d_\n", -42, -12345, -1000000);
    std_res = printf("Decimal negative: _%d_ _%d_ _%d_\n", -42, -12345, -1000000);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Граничные значения 32-bit
    my_res = MyPrint("Decimal bounds 32: _%d_ _%d_\n", 2147483647, -2147483648);
    std_res = printf("Decimal bounds 32: _%d_ _%d_\n", 2147483647, -2147483648);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Много чисел подряд
    my_res = MyPrint("Decimal sequence: _%d_ _%d_ _%d_ _%d_ _%d_\n", 1, 12, 123, 1234, 12345);
    std_res = printf("Decimal sequence: _%d_ _%d_ _%d_ _%d_ _%d_\n", 1, 12, 123, 1234, 12345);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Степени десятки
    my_res = MyPrint("Decimal powers: _%d_ _%d_ _%d_ _%d_\n", 10, 100, 1000, 10000);
    std_res = printf("Decimal powers: _%d_ _%d_ _%d_ _%d_\n", 10, 100, 1000, 10000);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // ==========================================
    //               %o - OCTAL
    // ==========================================
    printf("=== %%o - OCTAL TESTS ===\n\n");

    // Базовые значения
    my_res = MyPrint("Octal basic: _%o_ _%o_ _%o_\n", 0, 1, 7);
    std_res = printf("Octal basic: _%o_ _%o_ _%o_\n", 0, 1, 7);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Степени восьмерки
    my_res = MyPrint("Octal powers: _%o_ _%o_ _%o_\n", 8, 64, 512);
    std_res = printf("Octal powers: _%o_ _%o_ _%o_\n", 8, 64, 512);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Разные числа
    my_res = MyPrint("Octal mixed: _%o_ _%o_ _%o_ _%o_\n", 10, 100, 255, 1024);
    std_res = printf("Octal mixed: _%o_ _%o_ _%o_ _%o_\n", 10, 100, 255, 1024);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Большие числа
    my_res = MyPrint("Octal large: _%o_ _%o_\n", 123456789, 2147483647);
    std_res = printf("Octal large: _%o_ _%o_\n", 123456789, 2147483647);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Максимальное значение
    my_res = MyPrint("Octal max: _%o_\n", 01777777777777777777777LL);
    std_res = printf("Octal max: _%lo_\n", 01777777777777777777777LL);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Отрицательные числа (в дополнительном коде)
    my_res = MyPrint("Octal negative: _%o_ _%o_\n", -1, -8);
    std_res = printf("Octal negative: _%o_ _%o_\n", -1, -8);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // ==========================================
    //               %x - HEXADECIMAL
    // ==========================================
    printf("=== %%x - HEXADECIMAL TESTS ===\n\n");

    // Базовые значения
    my_res = MyPrint("Hex basic: _%x_ _%x_ _%x_\n", 0, 10, 15);
    std_res = printf("Hex basic: _%x_ _%x_ _%x_\n", 0, 10, 15);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Степени 16
    my_res = MyPrint("Hex powers: _%x_ _%x_ _%x_\n", 16, 256, 4096);
    std_res = printf("Hex powers: _%x_ _%x_ _%x_\n", 16, 256, 4096);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Разные значения
    my_res = MyPrint("Hex mixed: _%x_ _%x_ _%x_ _%x_\n", 42, 255, 1000, 65535);
    std_res = printf("Hex mixed: _%x_ _%x_ _%x_ _%x_\n", 42, 255, 1000, 65535);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Буквы a-f
    my_res = MyPrint("Hex letters: _%x_ _%x_ _%x_ _%x_ _%x_ _%x_\n",
                      0xa, 0xb, 0xc, 0xd, 0xe, 0xf);
    std_res = printf("Hex letters: _%x_ _%x_ _%x_ _%x_ _%x_ _%x_\n",
                      0xa, 0xb, 0xc, 0xd, 0xe, 0xf);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Смешанные буквы и цифры
    my_res = MyPrint("Hex mixed letters: _%x_ _%x_ _%x_\n", 0xdead, 0xbeef, 0xcafe);
    std_res = printf("Hex mixed letters: _%x_ _%x_ _%x_\n", 0xdead, 0xbeef, 0xcafe);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Большие числа
    my_res = MyPrint("Hex large: _%x_ _%x_\n", 0x7fffffff, 0xffffffff);
    std_res = printf("Hex large: _%x_ _%x_\n", 0x7fffffff, 0xffffffff);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // 64-битные числа
    my_res = MyPrint("Hex 64-bit: _%x_ _%x_\n", 0x123456789abcdef0LL, 0xfedcba9876543210LL);
    std_res = printf("Hex 64-bit: _%lx_ _%lx_\n", 0x123456789abcdef0LL, 0xfedcba9876543210LL);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Отрицательные числа
    my_res = MyPrint("Hex negative: _%x_ _%x_\n", -1, -16);
    std_res = printf("Hex negative: _%x_ _%x_\n", -1, -16);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // ==========================================
    //               %e - EXPONENTIAL
    // ==========================================
    printf("=== %%e - EXPONENTIAL TESTS ===\n\n");

    // Ноль
    my_res = MyPrint("Exp zero: _%e_ _%e_\n", 0.0, -0.0);
    std_res = printf("Exp zero: _%e_ _%e_\n", 0.0, -0.0);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Целые числа
    my_res = MyPrint("Exp integers: _%e_ _%e_ _%e_\n", 1.0, 42.0, -123.0);
    std_res = printf("Exp integers: _%e_ _%e_ _%e_\n", 1.0, 42.0, -123.0);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Дробные числа
    my_res = MyPrint("Exp fractions: _%e_ _%e_ _%e_\n", 0.1, 0.01, 0.001);
    std_res = printf("Exp fractions: _%e_ _%e_ _%e_\n", 0.1, 0.01, 0.001);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Разные экспоненты
    my_res = MyPrint("Exp various: _%e_ _%e_ _%e_\n", 1.23e-10, 4.56e+10, -7.89e-5);
    std_res = printf("Exp various: _%e_ _%e_ _%e_\n", 1.23e-10, 4.56e+10, -7.89e-5);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Округление вверх
    my_res = MyPrint("Exp round up: _%e_ _%e_ _%e_\n", 1.9999995, 2.0000005, 3.9999995);
    std_res = printf("Exp round up: _%e_ _%e_ _%e_\n", 1.9999995, 2.0000005, 3.9999995);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Округление вниз
    my_res = MyPrint("Exp round down: _%e_ _%e_ _%e_\n", 1.9999994, 2.0000004, 3.9999994);
    std_res = printf("Exp round down: _%e_ _%e_ _%e_\n", 1.9999994, 2.0000004, 3.9999994);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Каскадный перенос
    my_res = MyPrint("Exp cascade: _%e_ _%e_ _%e_\n", 9.9999995, 99.9999995, 999.9999995);
    std_res = printf("Exp cascade: _%e_ _%e_ _%e_\n", 9.9999995, 99.9999995, 999.9999995);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Границы экспоненты (1-2-3 цифры)
    my_res = MyPrint("Exp bounds: _%e_ _%e_ _%e_\n", 1.0e+1, 1.0e+12, 1.0e+123);
    std_res = printf("Exp bounds: _%e_ _%e_ _%e_\n", 1.0e+1, 1.0e+12, 1.0e+123);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Отрицательные экспоненты
    my_res = MyPrint("Exp negative: _%e_ _%e_ _%e_\n", 1.0e-1, 1.0e-12, 1.0e-123);
    std_res = printf("Exp negative: _%e_ _%e_ _%e_\n", 1.0e-1, 1.0e-12, 1.0e-123);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Специальные значения
    my_res = MyPrint("Exp special: _%e_ _%e_ _%e_\n", 1.0/0.0, -1.0/0.0, 0.0/0.0);
    std_res = printf("Exp special: _%e_ _%e_ _%e_\n", 1.0/0.0, -1.0/0.0, 0.0/0.0);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Денормализованные числа
    my_res = MyPrint("Exp denormal: _%e_ _%e_\n", 1.0e-308, 2.225e-308);
    std_res = printf("Exp denormal: _%e_ _%e_\n", 1.0e-308, 2.225e-308);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Точность double
    my_res = MyPrint("Exp precision: _%e_\n", 1.234567890123456789);
    std_res = printf("Exp precision: _%e_\n", 1.234567890123456789);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // ==========================================
    //         MIXED FORMAT TESTS
    // ==========================================
    printf("=== MIXED FORMAT TESTS ===\n\n");

    // Все спецификаторы вместе
    my_res = MyPrint("All formats: %c %s %d %o %x %e\n", 'X', "test", 42, 64, 255, 3.14);
    std_res = printf("All formats: %c %s %d %o %x %e\n", 'X', "test", 42, 64, 255, 3.14);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Много аргументов
    my_res = MyPrint("Many args: %d %d %d %d %d %d %d %d %d %d\n",
                     1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    std_res = printf("Many args: %d %d %d %d %d %d %d %d %d %d\n",
                     1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Чередование типов
    my_res = MyPrint("Mixed types: %c %d %s %x %e %o\n",
                     'A', -123, "hello", 0xdead, 2.71828, 0755);
    std_res = printf("Mixed types: %c %d %s %x %e %o\n",
                     'A', -123, "hello", 0xdead, 2.71828, 0755);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Без спецификаторов
    my_res = MyPrint("No formats here\n");
    std_res = printf("No formats here\n");
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Только спецификаторы
    my_res = MyPrint("%c%s%d%o%x%e\n", 'A', "BC", 123, 077, 0xff, 1.23e+4);
    std_res = printf("%c%s%d%o%x%e\n", 'A', "BC", 123, 077, 0xff, 1.23e+4);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // ==========================================
    //         BUFFER OVERFLOW TESTS
    // ==========================================
    printf("=== BUFFER OVERFLOW TESTS ===\n\n");

    // Строка больше буфера
    my_res = MyPrint("Buffer test 1: %s\n",
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
        "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC");
    std_res = printf("Buffer test 1: %s\n",
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
        "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC");
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Много спецификаторов (вызовет сброс буфера)
    my_res = MyPrint("Buffer test 2: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
                     1000000, 2000000, 3000000, 4000000, 5000000, 6000000, 7000000, 8000000,
                     9000000, 10000000, 11000000, 12000000, 13000000, 14000000, 15000000, 16000000);
    std_res = printf("Buffer test 2: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
                     1000000, 2000000, 3000000, 4000000, 5000000, 6000000, 7000000, 8000000,
                     9000000, 10000000, 11000000, 12000000, 13000000, 14000000, 15000000, 16000000);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // ==========================================
    //            EDGE CASES
    // ==========================================
    printf("=== EDGE CASES ===\n\n");

    // Процент в строке
    my_res = MyPrint("Percent sign: 100%% complete\n");
    std_res = printf("Percent sign: 100%% complete\n");
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Неизвестный спецификатор
    my_res = MyPrint("Unknown spec: _%k_ _%y_\n", 123, 456);
    std_res = printf("Unknown spec: _%k_ _%y_\n", 123, 456);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Пустая строка формата
    my_res = MyPrint("");
    std_res = printf("");
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Только завершающий ноль
    my_res = MyPrint("\0hidden");
    std_res = printf("\0hidden");
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    printf("========================================\n");
    printf("            TESTS COMPLETED             \n");
    printf("========================================\n");

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
