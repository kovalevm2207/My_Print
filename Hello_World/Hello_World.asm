global main

extern WriteFile    ; Подключаем нативные функции,
extern GetStdHandle ; чтобы ими можно было воспользоваться
extern WriteConsolA

section .text
main:
    mov  rcx, -11
    call GetStdHandle   ; Возвращает дескриптор для взаимодействия с консолью в ax

    sub  rsp, 40         ; Выделяем место в стеке для аргументов функции WriteFile
    mov  rcx, rax
    mov  rdx, message
    mov  r8d, len
    xor  r9, r9          ; Количество считанных байтов
    mov  qword [rsp+32], 0
    call WriteFile
    add  rsp, 40

    ret

section .data
message: db "Hello, World!", 13, 10
len     equ $-message
