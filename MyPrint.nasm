; Simple Print version, which can analyze and output only text string without specifiers

; Read and count in rcx number of string's symbols, while we don't find '%' or '\0'
%macro  COUNT_LEN
        xor     rcx, rcx
        push    rsi

..@Next:mov     bl, byte [rsi]

        cmp     bl, '%'
        je      ..@Break

        cmp     bl, 0           ; end of format string
        je      ..@Break

        inc     rcx
        inc     rsi
        xor     bl, bl

        cmp     rcx, 256
        jbe     ..@Next

..@Break:
        pop     rsi
%endmacro

section .bss

OPBuf:  resb 256

section .text

global MyPrint

MyPrint:
        xor     rax, rax

        mov     rsi, rcx
        lea     rdi, OPBuf
        COUNT_LEN ; = rcx

        rep     movsb           ; copy part of format string in output buffer
        cmp     byte [rsi], '\0'

        add     rax, rdi-OPBuf  ; calculate return value

        push    rax             ; save return value
        sub     rsp, 40         ; allocate Shadow Space

        mov     rcx, -11
        call    GetStdHandle

        add     rsp, 40         ; restore stack

        mov     rcx, rax        ; put descriptor
        pop     rax             ; restore return value

        sub     rsp, 40         ; allocate Shadow Space

        mov     rdx, OPBuf      ; buffer
        mov     r8d, rax        ; len
        xor     r9, r9
        mov     qword [rsp+32], 0
        call    WriteFile       ; display

        add     rsp, 40         ; restore stack

        ret

