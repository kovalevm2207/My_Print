; Simple Print version, which can analyze and output only text string without specifiers

; Read and count in rcx number of string's symbols, while we don't find '%' or '\0'
%macro  COUNT_LEN
        xor     rcx, rcx
        push    rsi

..@Next:mov     bl, byte [rsi]

        cmp     bl, '%'
        je      ..@Break

        cmp     bl, '\0'
        je      ..@Break

        inc     rcx
        inc     rsi
        xor     bl, bl
        jmp     ..@Next

..@Break:
        pop     rsi
%endmacro

section .data

OPBuf:  resb 256

section .text

global MyPrint

MyPrint:

        mov     rsi, rcx
        lea     rdi, OPBuf
        COUNT_LEN ; = rcx

        rep     movsb

        ret

