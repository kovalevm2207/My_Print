; Simple Print version, which can analyze and output only text string without specifiers

extern GetStdHandle
extern WriteFile

; Read and count in rcx number of string's symbols, while we don't find '%' or '\0'
%macro  COUNT_LEN_FOR_COPY_IN_OPBuf 0
        xor     rcx, rcx
        mov     rdx, rsi        ; save str position

%%Next:mov     bl, byte [rsi]

        cmp     bl, '%'
        je      %%Break

        cmp     bl, 0           ; end of format string
        je      %%Break

        inc     rcx
        inc     rsi
        xor     bl, bl

        cmp     rcx, 256
        jb      %%Next

%%Break:
        mov     rsi, rdx        ; restore str position
%endmacro

section .bss

OPBuf:  resb 256

section .text

global MyPrint

MyPrint:
        ; Save address beginning of arguments
        push    rbp
        mov     rbp, rsp        ; return address is on rbp+8  position
                                ; first argument is on rbp+16 position e.t.c.

        ; Save Nonvolatile registers
        push    rdi
        push    rsi
          ; We must end our program with old rsp to save it
        ; We must implement this: push    r11 ; if we will use this registers

        xor     rax, rax        ; return value = NULL

        ; move part of format string to OPBuf
        mov     rsi, rcx        ; rsi = format string
        mov     rdi, OPBuf
        COUNT_LEN_FOR_COPY_IN_OPBuf ; set: rcx = num of symbols to copy
        add     rax, rcx        ; update return value

        rep     movsb           ; copy part of format string in output buffer

        ;cmp     byte [rsi], 0   ; check end of string, last byte must be equal zero

        push    rax             ; save return value
        sub     rsp, 40         ; allocate Shadow Space

        mov     rcx, -11
        call    GetStdHandle

        mov     rcx, rax        ; put descriptor

        add     rsp, 40         ; restore stack
        pop     rax             ; restore return value
        push    rax
        sub     rsp, 40         ; allocate Shadow Space

        mov     rdx, OPBuf      ; buffer
        mov     r8, rax        ; len
        xor     r9, r9
        mov     qword [rsp+32], 0
        call    WriteFile       ; display

        add     rsp, 40         ; restore stack
        pop     rax

        ; restore Nonvolatile registers
        pop     rsi
        pop     rdi
        pop     rbp

        ret
