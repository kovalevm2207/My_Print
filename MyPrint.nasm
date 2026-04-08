; Simple Print version, which can analyze and output only text string without specifiers

extern GetStdHandle
extern WriteFile
extern printf

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
; Read and count in rcx number of string's symbols, while we don't find '%' or '\0'
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
%macro  COUNT_LEN_FOR_COPY_IN_OPBuf 0
        mov     rcx, r12
        mov     rdx, rsi        ; save str position
        mov     r13, rbx        ; save

        mov     byte [rel RF],   0
        mov     byte [rel Type], 0

        %%Next: mov     bl, byte [rsi]

                cmp     bl, '%'
                je      %%Spec

                cmp     bl, '\'
                je      %%Slash

                cmp     bl, 0           ; end of format string
                je      %%Break

                inc     rcx
                inc     rsi
                xor     bl, bl

                cmp     rcx, OPBuf_size
                jb      %%Next

                mov     byte [rel RF], 1
                jmp     %%Break

        %%Spec: mov     byte [rel Type], 1
                jmp     %%Break

        %%Slash:mov     byte [rel Type], 2

        %%Break:
        mov     rbx, r13
        mov     rsi, rdx        ; restore str position
%endmacro

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
section .text

global MyPrint

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
        push    r12
        push    r13
        push    r14
        push    r15
        ; We must end our program with old rsp to save it
        ; We must implement this: push    r11 ; if we will use this registers

        xor     rax, rax        ; return value = NULL
        xor     rbx, rbx        ; shift in format string = NULL

Next:   xor     r12, r12        ; len of cur buffer
        ; move part of format string to OPBuf
BeforeSlash:
        mov     rsi, [rbp+16]   ; start of format string
        add     rsi, rbx        ; rsi = ptr in format string = address of start + shift, which equals number of processed symbols
        mov     rdi, OPBuf
        add     rdi, r12
        COUNT_LEN_FOR_COPY_IN_OPBuf ; set: rcx = num of symbols to copy

        add     rax, rcx        ; update return value
        add     rbx, rcx        ; update shift in format string
        add     r12, rcx        ; update len of cur buffer

        rep     movsb           ; copy part of format string in output buffer

        movzx   rcx, byte [rel Type]
        cmp     rcx, 2
        ja      DefaultType

        shl     rcx, 3
        lea     rdx, [rel TypeJmpTable]
        jmp     [rdx + rcx]

Drop:
        mov     r13, rax         ; save return value = shift in output buffer
        sub     rsp, 40         ; allocate Shadow Space

        mov     rcx, -11
        call    GetStdHandle

        mov     rcx, rax        ; put descriptor
        mov     rdx, OPBuf      ; buffer
        mov     r8, r12         ; len
        xor     r9, r9
        mov     qword [rsp+32], 0
        call    WriteFile       ; display

        add     rsp, 40         ; restore stack
        mov     rax, r13        ; restore return value = shift in output buffer

        cmp     byte [rel RF], 1
        je      Next

        ; restore Nonvolatile registers
        pop     r15
        pop     r14
        pop     r13
        pop     r12
        pop     rbx
        pop     rsi
        pop     rdi
        pop     rbp

        ; restore args
        mov     rcx, qword [rsp+8 ]
        mov     rdx, qword [rsp+16]
        mov     r8,  qword [rsp+24]
        mov     r9,  qword [rsp+32]

        mov     qword [rel MyPrintRetVal], rax
        pop     rbx

        call    printf

        push    rbx
        mov     rax, qword [rel MyPrintRetVal]

        ret

Spec:
        jmp     Drop

Slash:  ; for all this variants we need only 1 byte of memory
        cmp     r12, OPBuf_size
        jb      WSO     ; write slash operator

                mov     byte [rel RF], 1
                jmp     Drop

    WSO:xor     rcx, rcx
        mov     cl, byte [rsi+1] ; skip '\'
        cmp     cl, 119 ; <-- maximum ASCII code of special symbols
        ja      DefaultType                                            ; TO DO: make special err func

        lea     rdx, [rel SlashTable]
        mov     cl, byte [rdx + rcx]
        cmp     cl, 0   ; if it is wrong symbol in Slash Table it equals zero
        je      DefaultType

        mov     byte [rdi + 1], cl
        inc     r12
        add     rbx, 2

        jmp     BeforeSlash

DefaultType:
        ; write fatal err situation
        sub     rsp, 40         ; allocate Shadow Space

        mov     rcx, -11
        call    GetStdHandle

        mov     rcx, rax        ; put descriptor

        mov     rdx, TypeErrMsg ; buffer
        mov     r8, MsgSize         ; len
        xor     r9, r9
        mov     qword [rsp+32], 0
        call    WriteFile       ; display

        add     rsp, 40         ; restore stack

        ; restore Nonvolatile registers
        pop     r15
        pop     r14
        pop     r13
        pop     r12
        pop     rbx
        pop     rsi
        pop     rdi
        pop     rbp

        ret

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
section .bss

OPBuf_size      equ  8
OPBuf:  resb OPBuf_size

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
section .data

MyPrintRetVal   dq 0

RF              db 0    ; repeat flag
Type            db 0

TypeErrMsg      db "Type err", 10, 0
MsgSize        equ $ - TypeErrMsg

TypeJmpTable:
        dq      Drop
        dq      Spec
        dq      Slash

; the most useful escape-sequence for beginning:
;Symbol:      'a', 'b', 't', 'n', 'v', 'f', 'r', 'e', '\\', '\'', '\"'
;ASCII code:   97,  98, 116, 110, 118, 102, 114, 101,   92,   39,   34
;Byte:          7,   8,   9,  10,  11,  12,  13,  27,   92,   39,   34
        ;       _______ maximum ASCII code of special symbols _____/
section .data

SlashTable:
        times 34 db 0
                 db 34  ; '"'
        times 4  db 0
                 db 39  ; '\''
        times 52 db 0
                 db 92  ; '\\'
        times 4  db 0
                 db 7   ; 'a'
                 db 8   ; 'b'
        times 2  db 0
                 db 27  ; 'e'
                 db 12  ; 'f'
        times 7  db 0
                 db 10  ; 'n'
        times 3  db 0
                 db 13  ; 'r'
        times 1  db 0
                 db 9   ; 't'
        times 1  db 0
                 db 11  ; 'v'
