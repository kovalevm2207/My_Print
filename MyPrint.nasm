; Simple Print version, which can analyze and output only text string without specifiers

default rel

extern GetStdHandle
extern WriteFile
extern printf

OPBuf_size      equ  8

; Read and count in rcx number of string's symbols, while we don't find '%' or '\0'
%macro  COUNT_LEN_FOR_COPY_IN_OPBuf 0
        xor     rcx, rcx
        mov     rdx, rsi        ; save str position
        mov     r13, rbx        ; save

        mov     byte [OverflowFlag], 0

%%Next: mov     bl, byte [rsi]

        cmp     bl, '%'
        je      %%Break

        cmp     bl, 0           ; end of format string
        je      %%Break

        inc     rcx
        inc     rsi
        xor     bl, bl

        cmp     rcx, OPBuf_size
        jb      %%Next

        mov     byte [OverflowFlag], 1

%%Break:
        mov     rbx, r13
        mov     rsi, rdx        ; restore str position
%endmacro

section .bss

OPBuf:  resb OPBuf_size

section .data

OverflowFlag    db 0

section .text

global MyPrint

; rax - return value
; rbx - shift in format string
; r12 -  len of cur buffer
; rbp + 8  - ptr on return address
; rbp + 16 - ptr on first symbol of format string

; rcx - using as a counter in cycles
; rsi - source in functions, shows the position in format string
; rdi - destination, shaws the positions in output buffer

; remaining Volatile-registers:
;       r13 r14 r15
;       xmm6 - xmm15

MyPrint:
        mov     qword [rsp+8 ], rcx     ; save 1st argument
        mov     qword [rsp+16], rdx     ; save 2nd argument
        mov     qword [rsp+24], r8      ; save 3rd argument
        mov     qword [rsp+32], r9      ; save 4th argument

        ; Save Nonvolatile registers
        ; Save address beginning of arguments
        push    rbp
        mov     rbp, rsp        ; return address is on rbp+8  position
                                ; first argument is on rbp+16 position e.t.c.
        push    rdi
        push    rsi
        push    rbx
        push    r13
        ; We must end our program with old rsp to save it
        ; We must implement this: push    r11 ; if we will use this registers

        xor     rax, rax        ; return value = NULL
        xor     rbx, rbx        ; shift in format string = NULL

.Next:  xor     r12, r12        ; len of cur buffer
        ; move part of format string to OPBuf
        mov     rsi, [rbp+16]   ; start of format string
        add     rsi, rbx        ; rsi = ptr in format string = address of start + shift, which equals number of processed symbols
        mov     rdi, OPBuf
        COUNT_LEN_FOR_COPY_IN_OPBuf ; set: rcx = num of symbols to copy

        add     rax, rcx        ; update return value = shift in output buffer
        add     rbx, rcx        ; update shift in format string
        add     r12, rcx        ; update len of cur buffer

        rep     movsb           ; copy part of format string in output buffer

        xor     rcx, rcx
        mov     cl, byte [OPBuf]
        add     rcx, r12
        cmp     r12, 0   ; check end of string, last byte must be equal zero
        jne     .Skip

        mov     byte [OverflowFlag], 0  ; it is branch, where rcx == OPBuf_size & the last symbol in this buffer is '\0'
.Skip:

        push    rax             ; save return value = shift in output buffer
        ; "push    rbx" is optional, because rbx is one of the volatile registers which will be save in functions GetStdHandle and WriteFile
        sub     rsp, 40         ; allocate Shadow Space

        mov     rcx, -11
        call    GetStdHandle

        mov     rcx, rax        ; put descriptor

        add     rsp, 40         ; restore stack
        pop     rax             ; restore return value = shift in output buffer
        push    rax             ; save shift in output buffer
        sub     rsp, 40         ; allocate Shadow Space

        mov     rdx, OPBuf      ; buffer
        mov     r8, r12         ; len
        xor     r9, r9
        mov     qword [rsp+32], 0
        call    WriteFile       ; display

        add     rsp, 40         ; restore stack
        pop     rax             ; restore return value = shift in output buffer

        cmp     byte [OverflowFlag], 1  ; if
        je      .Next

        ; restore Nonvolatile registers
        pop     r13
        pop     rbx
        pop     rsi
        pop     rdi
        pop     rbp

        mov     rcx, qword [rsp+8 ]
        mov     rdx, qword [rsp+16]
        mov     r8,  qword [rsp+24]
        mov     r9,  qword [rsp+32]

        pop     rbx
        call    printf

        push    rbx
        ret
