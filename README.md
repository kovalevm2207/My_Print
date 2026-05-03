# My_Print
Реализация стандартной функции printf() библиотеки stdio.h

## Реализованы следующие спецификаторы стандартного printf():
%c - Character

%s - String

%d - Decimal

%b - Binary (Вывод числа в двоичной системе счисления)

%o - Octal

%x - Hex

%f - Float

%e - Exponential
## Компиляция в объектный файл :
    nasm -f win64 MyPrint.nasm -o MyPrint.o

## Использование в ваших проектах :
В области видимости, использующей MyPrint(), до вызова данной функции пропишите:

    #include "KVLVstdio.h"

Для получения исполняемого файла:

    gcc YourProgram.c MyPrint.o -o YourProgram.exe
