global main

extern printf

section .data
message1 db "Hello, World!", 13, 10, 0
message2 db "Name: %s  Age: %u  study: %s Salary: %u", 10, 13, 0   ; строка для вывода
name     db "Mikle", 0
age      dq 18
company  db "MIPT", 0
salary   dq 1150

section .text
main:
    sub rsp, 40

    lea rcx,  [rel message1]
    call printf

    xor rcx, rcx
    xor rax, rax

    lea rcx,  [rel message2]
    lea rdx,  [rel name]
    mov r8,   [rel age]
    lea r9,   [rel company]
    mov r10,  [rel salary]
    mov qword [rsp+32], r10
    call printf

    add rsp, 40

    ret
