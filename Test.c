#include <stdio.h>
#include <assert.h>

extern int MyPrint(const char* const format, ...);

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
    //               %и - BINARY
    // ==========================================
    printf("=== %%b - BINARY TESTS ===\n\n");

    // Базовые значения
    my_res = MyPrint("Bin basic: _%b_ _%b_ _%b_\n", 0, 1, 2);
    printf("MyPrint: %d\n\n", my_res);

    // Простые числа
    my_res = MyPrint("Bin simple: _%b_ _%b_ _%b_\n", 3, 4, 5);
    printf("MyPrint: %d\n\n", my_res);

    // Степени двойки
    my_res = MyPrint("Bin powers: _%b_ _%b_ _%b_ _%b_\n", 8, 16, 32, 64);
    printf("MyPrint: %d\n\n", my_res);

    // Байтовые значения
    my_res = MyPrint("Bin byte: _%b_ _%b_\n", 0xFF, 0xAA);
    printf("MyPrint: %d\n\n", my_res);

    // 16-битные значения
    my_res = MyPrint("Bin 16bit: _%b_ _%b_\n", 0xFFFF, 0x1234);
    printf("MyPrint: %d\n\n", my_res);

    // Граничные значения 32-бит
    my_res = MyPrint("Bin bounds: _%b_ _%b_\n", 0x7FFFFFFF, 0xFFFFFFFF);
    printf("MyPrint: %d\n\n", my_res);

    // Несколько подряд
    my_res = MyPrint("Bin sequence: _%b_ _%b_ _%b_ _%b_\n", 10, 100, 1000, 10000);
    printf("MyPrint: %d\n\n", my_res);

    // Ведущие нули (проверка что 0 выводится как "0")
    my_res = MyPrint("Bin zero: _%b_\n", 0);
    printf("MyPrint: %d\n\n", my_res);

    // Отрицательные числа (как 32-битные)
    my_res = MyPrint("Bin negative: _%b_ _%b_\n", -1, -128);
    printf("MyPrint: %d\n\n", my_res);

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

    printf("=== %%f - FLOAT TESTS ===\n\n");

    // Ноль
    my_res = MyPrint("Float zero: _%f_ _%f_\n", 0.0, -0.0);
    std_res = printf("Float zero: _%f_ _%f_\n", 0.0, -0.0);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Целые числа
    my_res = MyPrint("Float integers: _%f_ _%f_ _%f_\n", 1.0, 42.0, -123.0);
    std_res = printf("Float integers: _%f_ _%f_ _%f_\n", 1.0, 42.0, -123.0);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Дробные числа
    my_res = MyPrint("Float fractions: _%f_ _%f_ _%f_\n", 0.1, 0.01, 0.001);
    std_res = printf("Float fractions: _%f_ _%f_ _%f_\n", 0.1, 0.01, 0.001);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Обычные числа
    my_res = MyPrint("Float normal: _%f_ _%f_ _%f_\n", 3.14, -2.718, 123.456);
    std_res = printf("Float normal: _%f_ _%f_ _%f_\n", 3.14, -2.718, 123.456);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Округление вверх
    my_res = MyPrint("Float round up: _%f_ _%f_ _%f_\n", 1.9999995, 2.0000005, 3.9999995);
    std_res = printf("Float round up: _%f_ _%f_ _%f_\n", 1.9999995, 2.0000005, 3.9999995);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Округление вниз
    my_res = MyPrint("Float round down: _%f_ _%f_ _%f_\n", 1.9999994, 2.0000004, 3.9999994);
    std_res = printf("Float round down: _%f_ _%f_ _%f_\n", 1.9999994, 2.0000004, 3.9999994);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Каскадный перенос
    my_res = MyPrint("Float cascade: _%f_ _%f_ _%f_\n", 9.9999995, 99.9999995, 999.9999995);
    std_res = printf("Float cascade: _%f_ _%f_ _%f_\n", 9.9999995, 99.9999995, 999.9999995);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Большие числа
    my_res = MyPrint("Float large: _%f_ _%f_\n", 1234567.0, 123456789.0);
    std_res = printf("Float large: _%f_ _%f_\n", 1234567.0, 123456789.0);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Очень большие числа (много цифр до точки)
    my_res = MyPrint("Float very large: _%f_\n", 1.234567e+10);
    std_res = printf("Float very large: _%f_\n", 1.234567e+10);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Очень маленькие числа (много нулей после точки)
    my_res = MyPrint("Float very small: _%f_ _%f_\n", 0.0000001, 0.000000000123456);
    std_res = printf("Float very small: _%f_ _%f_\n", 0.0000001, 0.000000000123456);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Специальные значения
    my_res = MyPrint("Float special: _%f_ _%f_ _%f_\n", 1.0/0.0, -1.0/0.0, 0.0/0.0);
    std_res = printf("Float special: _%f_ _%f_ _%f_\n", 1.0/0.0, -1.0/0.0, 0.0/0.0);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Точность
    my_res = MyPrint("Float precision: _%f_\n", 1.234567890123456789);
    std_res = printf("Float precision: _%f_\n", 1.234567890123456789);
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);

    // Float (32-bit) — должно повышаться до double
    float f = 3.14159f;
    my_res = MyPrint("Float 32bit: _%f_\n", f);
    std_res = printf("Float 32bit: _%f_\n", f);
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
