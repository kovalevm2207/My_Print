; Simple Print version, which can analyze and output only text string without %cr

extern GetStdHandle
extern WriteFile
extern printf

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
; Read and count in rcx number of string's symbols, while we don't find '%' or '\0'
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
%macro  COUNT_LEN_FOR_COPY_IN_OPBuf 0
        xor     rcx, rcx
        mov     rdx, rsi        ; save str position
        mov     r13, rbx        ; save

        mov     byte [rel RF], 0
        mov     byte [rel PF], 0

        %%Next:
                mov     rbx, rcx
                add     rbx, r12
                cmp     rbx, OPBuf_size
                je      %%case_OverFlow

                mov     bl, byte [rsi]

                cmp     byte [rel SF], 1
                je      %%Skip

                        cmp     bl, '%'
                        je      %%Spec
        %%Skip:
                cmp     bl, 0           ; end of format string
                je      %%Break

                inc     rcx
                inc     rsi

                jmp     %%Next

        %%case_OverFlow:
                mov     byte [rel RF], 1
                jmp     %%Break

        %%Spec:
                mov     byte [rel PF], 1

        %%Break:
        mov     rbx, r13
        mov     rsi, rdx        ; restore str position
%endmacro

%macro DROP_BUFFER 0
        mov     r13, rax        ; save return value = shift in output buffer
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
; r13 - for different saves
; r14 - номер аргумента

; rcx - using as a counter in cycles
; rsi - source in functions, shows the position in format string
; rdi - destination, shaws the positions in output buffer

; remaining Volatile-registers:
;       r15
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
        xor     r14, r14        ; set start value for argument counter

Next:
        xor     r12, r12        ; len of cur buffer
        ; move part of format string to OPBuf
        mov     rsi, [rbp+16]   ; start of format string
        add     rsi, rbx        ; rsi = ptr in format string = address of start + shift, which equals number of processed symbols
        mov     rdi, OPBuf
        add     rdi, r12
    AfterPercent:
        COUNT_LEN_FOR_COPY_IN_OPBuf ; set: rcx = num of symbols to copy

        add     rax, rcx        ; update return value
        add     rbx, rcx        ; update shift in format string
        add     r12, rcx        ; update len of cur buffer

        rep     movsb           ; copy part of format string in output buffer

        mov     cl, byte [rel PF]
        cmp     cl, 1
        je      Percent
    Drop:
        DROP_BUFFER

        cmp     byte [rel RF], 1
        je     Next

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

        ;mov     qword [rel MyPrintRetVal], rax
        ;pop     rbx
        ;
        ;call    printf
        ;
        ;push    rbx
        ;mov     rax, qword [rel MyPrintRetVal]

        ret

; in this branch we have free registers: rcx, rdx
; r14 - number of argument, start value = 0 (format string)
; [rsi] = '%'
; rdi   - first free position in output buffer
Percent:
        ; /--------------- rcx = 00 00 00 **
        movzx   rcx, byte [rsi+1]        ; cl = opcode of after percent symbol
        shl     rcx, 3                   ; rcx*8 - because we save our labels with 8 shift (qword)
        lea     rdx, [rel JmpTable]
        mov     rcx, qword [rdx+rcx]   ; rcx = *(JmTable + rxc * 8)


        cmp     rcx, 0                  ; if we have wrong specifier we should skip him
        jne     Correct

                inc     rsi
                inc     rbx
                jmp     AfterPercent
    Correct:
        jmp     rcx

;---------------------------------------------------------------------------------------
; ShowWord: выводит в текстовую видеопамять двоичное/восьмеричное/шестнадцатеричное
; отображение, в зависимости от переданного параметра, числа
; Входные параметры:     - mask               <--| may be some one of this is optional
;                    rcx - shift for the mask <__|
;                    rsi - ptr on the number (source)
;                    rdi - dest
; Ожидаемое состояние:
; return value: += rax (number of the symbols we had displayed)
;               += r12 (shift in OPBuf)
;               += rdi (absolutely ptr in OPBuf) <------ ???
; Испорченные регистры: rcx,
;                       rdx -contains mask value
;                       (also we can use r13, r15 for different saves)
; Volatile-registers: rbp (ptr on arguments of format string)
;                     rbx (shift in format string)
;
;---------------------------------------------------------------------------------------

%macro ShowQWord 0
        ; at first
        ; make mask from rcx arg
        mov     rdx, -1
        mov     r13, 64
        sub     r13, rcx
        shl     rdx, r13

        xor     r13, r13        ; r13 - counter
        mov     r15, 64
        shl     r15, rcx        ; r15 - num of itarate
    %%Next:
                push    rsi; save rsi = value
                and     rsi, rdx                        ;
                shr     rbx, rcx                        ; change mask
                ; shift data from the high bytes to low bytes
                shr     rsi, 64-rcx-r13

        cmp     r13, r15
        jb      %%Next

%endmacro

case_Character:
        ; in this case we need only one free byte in OPBuf
        cmp     r12, OPBuf_size
        jb      WriteCharacter
                DROP_BUFFER             ; if we don't have memory -> drop buffer
                xor     r12, r12
                mov     rdi, OPBuf
    WriteCharacter:
        inc     r14             ; increment argument counter

        push    rsi             ; save position in format string
        push    r14             ; save number of argument

        mov     rsi, rbp
        shl     r14, 3
        add     rsi, r14
        add     rsi, 16          ; ptr on argument
        movsb   ;[rdi++], [rsi = (rbp+r14+16)]

        pop     r14             ; restore number of argument
        pop     rsi             ; restore position in format string

        inc     rax             ; increment return value, number of characters
        inc     r12             ; position in OPBuf
        add     rbx, 2                  ; skip  % and  specifier
        add     rsi, 2                  ; '%' 'c' '*'
                                        ;          ^
                                        ;    rsi _/

        jmp     AfterPercent
case_Float:
        jmp     Drop
case_Octal:
        jmp     Drop
case_String:
        ; in this case we don't know, how much free bytes we need, so we will write this string, such a format string
        ; only change source: rsi = absolute addr of symbol in format string    -->    rsi = absolute addr of symbol in argument string
        ;                     rbx = shift in format string on symbol            -->    rbx = shift in argument string on symbol
                                ; next values we will use to continue
        add     rsi, 2          ; skip % and specifier in format sting
        add     rbx, 2          ; skip % and specifier in shift of format string

        inc     r14     ; update argument number
        ; save params for work with format string
        push    rsi
        push    rbx

        push    r14     ; save argument number

        xor     rbx, rbx        ; set shift in argument string

        shl     r14, 3
        mov     rsi, rbp
        mov     rsi, [r14+rsi+16]      ; now: rsi = *(rbp+r14*8+16) = start address of string

        pop     r14     ; restore argument number
        push    rsi     ; save start address of argument string

    .Next:
        COUNT_LEN_FOR_COPY_IN_OPBuf
        add     rax, rcx        ; update return value
        add     rbx, rcx        ; update shift in format string
        add     r12, rcx        ; update len of cur buffer

        rep     movsb           ; copy part of format string in output buffer

        cmp     byte [rel RF], 1
        jne     .End
                DROP_BUFFER
                xor     r12, r12
                mov     rsi, [rsp]
                add     rsi, rbx
                mov     rdi, OPBuf
                add     rdi, r12
                jmp     case_String.Next
    .End:
        add     rsp, 8         ; skip  saved start of address string
        pop     rbx
        pop     rsi

        jmp     AfterPercent
case_Hex:
        ; in this case we can't use rep movsb, so we should make an individual proc
        add     rbx, 2                  ;/////////////////////////////////////////////
        add     rsi, 2                  ;
                                        ;
        push    rsi                     ;
        push    rbx                     ;
                                        ;
        xor     rbx, rbx                ; see, for example, case_String
                                        ;
        inc     r14                     ;
        push    r14                     ;
                                        ;
        shl     r14, 3                  ;
        mov     rsi, rbp                ;
        mov     rsi, [r14+rsi+16]       ;/////////////////////////////////////////////
        ; now rsi = num_arg, which we will display

        pop     r14

        mov     rcx, 1      ; shift fot the mask, in case_Octal for example it will be 3 and in case_Hex 4

        jmp     Drop

;DefaultType:
;        ; write fatal err situation
;        sub     rsp, 40         ; allocate Shadow Space
;
;        mov     rcx, -11
;        call    GetStdHandle
;
;        mov     rcx, rax        ; put descriptor
;
;        mov     rdx, TypeErrMsg ; buffer
;        mov     r8, MsgSize         ; len
;        xor     r9, r9
;        mov     qword [rsp+32], 0
;        call    WriteFile       ; display
;
;        add     rsp, 40         ; restore stack
;
;        ; restore Nonvolatile registers
;        pop     r15
;        pop     r14
;        pop     r13
;        pop     r12
;        pop     rbx
;        pop     rsi
;        pop     rdi
;        pop     rbp
;
;        ret

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
section .bss

OPBuf_size      equ  128
OPBuf:  resb OPBuf_size

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
section .data

MyPrintRetVal   dq 0

RF              db 0    ; repeat flag
PF              db 0    ; percent flag
SF              db 0    ; string flag

;TypeErrMsg      db "Type err", 10, 0
;MsgSize        equ $ - TypeErrMsg

JmpTable:
times 'c'       dq 0                    ; ... -  a
                dq case_Character       ; c
times 'f'-'c'-1 dq 0                    ; d, e
                dq case_Float           ; f
times 'o'-'f'-1 dq 0                    ; g, h, i, g, k, l, m, n
                dq case_Octal           ; o
times 's'-'o'-1 dq 0                    ; p, q, r
                dq case_String          ; s
times 'x'-'s'-1 dq 0                    ; t, u, v, w, x, y, z
                dq case_Hex             ; x
times 255-'x'-1 dq 0                    ; y - ...
