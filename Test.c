#include <stdio.h>
#include <assert.h>

#define TestPrint(format, ...)               \
    my_res = MyPrint(format, ##__VA_ARGS__); \
    std_res = printf(format, ##__VA_ARGS__); \
    printf("MyPrint: %d, printf: %d\n\n", my_res, std_res);


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
    TestPrint("Char basic: _%c_ _%c_ _%c_\n", 'A', 'z', '0');

    // Специальные символы
    TestPrint("Char special: _%c_ _%c_ _%c_\n", '\n', '\t', '\\');

    // Граничные значения
    TestPrint("Char bounds: _%c_ _%c_\n", 0, 255);

    // Много символов подряд
    TestPrint("Char multiple: _%c%c%c%c%c_\n", 'H', 'e', 'l', 'l', 'o');

    // ==========================================
    //               %s - STRING
    // ==========================================
    printf("=== %%s - STRING TESTS ===\n\n");

    // Обычные строки
    TestPrint("String basic: _%s_ _%s_\n", "Hello", "World");

    // Пустая строка
    TestPrint("String empty: _%s_\n", "");

    // Длинные строки
    TestPrint("String long: _%s_\n", "This is a very long string that exceeds the buffer size to test the buffer overflow handling mechanism");

    // Строки со спецсимволами
    TestPrint("String with special: _%s_\n", "Line1\nLine2\tTabbed");

    // ==========================================
    //               %d - DECIMAL
    // ==========================================
    printf("=== %%d - DECIMAL TESTS ===\n\n");

    // Ноль и единица
    TestPrint("Decimal zero/one: _%d_ _%d_ _%d_\n", 0, 1, -1);

    // Положительные числа
    TestPrint("Decimal positive: _%d_ _%d_ _%d_\n", 42, 12345, 1000000);

    // Отрицательные числа
    TestPrint("Decimal negative: _%d_ _%d_ _%d_\n", -42, -12345, -1000000);

    // Граничные значения 32-bit
    TestPrint("Decimal bounds 32: _%d_ _%d_\n", 2147483647, -2147483648);

    // Много чисел подряд
    TestPrint("Decimal sequence: _%d_ _%d_ _%d_ _%d_ _%d_\n", 1, 12, 123, 1234, 12345);

    // Степени десятки
    TestPrint("Decimal powers: _%d_ _%d_ _%d_ _%d_\n", 10, 100, 1000, 10000);

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
    TestPrint("Octal basic: _%o_ _%o_ _%o_\n", 0, 1, 7);

    // Степени восьмерки
    TestPrint("Octal powers: _%o_ _%o_ _%o_\n", 8, 64, 512);

    // Разные числа
    TestPrint("Octal mixed: _%o_ _%o_ _%o_ _%o_\n", 10, 100, 255, 1024);

    // Большие числа
    TestPrint("Octal large: _%o_ _%o_\n", 123456789, 2147483647);

    // Максимальное значение
    TestPrint("Octal max: _%o_\n", 01777777777777777777777LL);

    // Отрицательные числа (в дополнительном коде)
    TestPrint("Octal negative: _%o_ _%o_\n", -1, -8);

    // ==========================================
    //               %x - HEXADECIMAL
    // ==========================================
    printf("=== %%x - HEXADECIMAL TESTS ===\n\n");

    // Базовые значения
    TestPrint("Hex basic: _%x_ _%x_ _%x_\n", 0, 10, 15);

    // Степени 16
    TestPrint("Hex powers: _%x_ _%x_ _%x_\n", 16, 256, 4096);

    // Разные значения
    TestPrint("Hex mixed: _%x_ _%x_ _%x_ _%x_\n", 42, 255, 1000, 65535);

    // Буквы a-f
    TestPrint("Hex letters: _%x_ _%x_ _%x_ _%x_ _%x_ _%x_\n", 0xa, 0xb, 0xc, 0xd, 0xe, 0xf);

    // Смешанные буквы и цифры
    TestPrint("Hex mixed letters: _%x_ _%x_ _%x_\n", 0xdead, 0xbeef, 0xcafe);

    // Большие числа
    TestPrint("Hex large: _%x_ _%x_\n", 0x7fffffff, 0xffffffff);

    // 64-битные числа
    TestPrint("Hex 64-bit: _%x_ _%x_\n", 0x123456789abcdef0LL, 0xfedcba9876543210LL);

    // Отрицательные числа
    TestPrint("Hex negative: _%x_ _%x_\n", -1, -16);

    // ==========================================
    //               %e - EXPONENTIAL
    // ==========================================
    printf("=== %%e - EXPONENTIAL TESTS ===\n\n");

    // Ноль
    TestPrint("Exp zero: _%e_ _%e_\n", 0.0, -0.0);

    // Целые числа
    TestPrint("Exp integers: _%e_ _%e_ _%e_\n", 1.0, 42.0, -123.0);

    // Дробные числа
    TestPrint("Exp fractions: _%e_ _%e_ _%e_\n", 0.1, 0.01, 0.001);

    // Разные экспоненты
    TestPrint("Exp various: _%e_ _%e_ _%e_\n", 1.23e-10, 4.56e+10, -7.89e-5);

    // Округление вверх
    TestPrint("Exp round up: _%e_ _%e_ _%e_\n", 1.9999995, 2.0000005, 3.9999995);

    // Округление вниз
    TestPrint("Exp round down: _%e_ _%e_ _%e_\n", 1.9999994, 2.0000004, 3.9999994);

    // Каскадный перенос
    TestPrint("Exp cascade: _%e_ _%e_ _%e_\n", 9.9999995, 99.9999995, 999.9999995);

    // Границы экспоненты (1-2-3 цифры)
    TestPrint("Exp bounds: _%e_ _%e_ _%e_\n", 1.0e+1, 1.0e+12, 1.0e+123);

    // Отрицательные экспоненты
    TestPrint("Exp negative: _%e_ _%e_ _%e_\n", 1.0e-1, 1.0e-12, 1.0e-123);

    // Специальные значения
    TestPrint("Exp special: _%e_ _%e_ _%e_\n", 1.0/0.0, -1.0/0.0, 0.0/0.0);

    // Денормализованные числа
    TestPrint("Exp denormal: _%e_ _%e_\n", 1.0e-308, 2.225e-308);

    // Точность double
    TestPrint("Exp precision: _%e_\n", 1.234567890123456789);

    printf("=== %%f - FLOAT TESTS ===\n\n");

    // Ноль
    TestPrint("Float zero: _%f_ _%f_\n", 0.0, -0.0);

    // Целые числа
    TestPrint("Float integers: _%f_ _%f_ _%f_\n", 1.0, 42.0, -123.0);

    // Дробные числа
    TestPrint("Float fractions: _%f_ _%f_ _%f_\n", 0.1, 0.01, 0.001);

    // Обычные числа
    TestPrint("Float normal: _%f_ _%f_ _%f_\n", 3.14, -2.718, 123.456);

    // Округление вверх
    TestPrint("Float round up: _%f_ _%f_ _%f_\n", 1.9999995, 2.0000005, 3.9999995);

    // Округление вниз
    TestPrint("Float round down: _%f_ _%f_ _%f_\n", 1.9999994, 2.0000004, 3.9999994);

    // Каскадный перенос
    TestPrint("Float cascade: _%f_ _%f_ _%f_\n", 9.9999995, 99.9999995, 999.9999995);

    // Большие числа
    TestPrint("Float large: _%f_ _%f_\n", 1234567.0, 123456789.0);

    // Очень большие числа (много цифр до точки)
    TestPrint("Float very large: _%f_\n", 1.234567e+10);

    // Очень маленькие числа (много нулей после точки)
    TestPrint("Float very small: _%f_ _%f_\n", 0.0000001, 0.000000000123456);

    // Специальные значения
    TestPrint("Float special: _%f_ _%f_ _%f_\n", 1.0/0.0, -1.0/0.0, 0.0/0.0);

    // Точность
    TestPrint("Float precision: _%f_\n", 1.234567890123456789);

    // ==========================================
    //         MIXED FORMAT TESTS
    // ==========================================
    printf("=== MIXED FORMAT TESTS ===\n\n");

    // Все спецификаторы вместе
    TestPrint("All formats: %c %s %d %o %x %e\n", 'X', "test", 42, 64, 255, 3.14);

    // Много аргументов
    TestPrint("Many args: %d %d %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

    // Чередование типов
    TestPrint("Mixed types: %c %d %s %x %e %o\n", 'A', -123, "hello", 0xdead, 2.71828, 0755);

    // Без спецификаторов
    TestPrint("No formats here\n");

    // Только спецификаторы
    TestPrint("%c%s%d%o%x%e\n", 'A', "BC", 123, 077, 0xff, 1.23e+4);

    // ==========================================
    //         BUFFER OVERFLOW TESTS
    // ==========================================
    printf("=== BUFFER OVERFLOW TESTS ===\n\n");

    // Строка больше буфера
    TestPrint("Buffer test 1: %s\n",
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
        "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC");

    // Много спецификаторов (вызовет сброс буфера)
    TestPrint("Buffer test 2: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
                     1000000, 2000000, 3000000, 4000000, 5000000, 6000000, 7000000, 8000000,
                     9000000, 10000000, 11000000, 12000000, 13000000, 14000000, 15000000, 16000000);

    // ==========================================
    //            EDGE CASES
    // ==========================================
    printf("=== EDGE CASES ===\n\n");

    // Процент в строке
    TestPrint("Percent sign: 100%% complete\n");

    // Неизвестный спецификатор
    TestPrint("Unknown spec: _%k_ _%y_\n", 123, 456);

    // Пустая строка формата
    TestPrint("");

    // Только завершающий ноль
    TestPrint("\0hidden");

    printf("========================================\n");
    printf("            TESTS COMPLETED             \n");
    printf("========================================\n");

    return 0;
}
