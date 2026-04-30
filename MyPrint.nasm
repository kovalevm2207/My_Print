extern GetStdHandle
extern WriteFile
extern printf

%macro PUSH_REGS 1-*
    %rep %0
        push    %1
        %rotate 1
    %endrep
%endmacro

%macro POP_REGS 1-*
    %rep %0
        %rotate -1
        pop     %1
    %endrep
%endmacro

%macro INC_REGS 1-*
    %rep %0
        inc     %1
        %rotate 1
    %endrep
%endmacro

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
; Read and count in rcx number of string's symbols, while we don't find '%' or '\0'
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
%macro  COUNT_LEN_FOR_COPY_IN_OPBuf 0
        xor     rcx, rcx

        mov     byte [rel RF], 0
        mov     byte [rel PF], 0

        %%Next:
                mov     rdx, rcx
                add     rdx, r12
                cmp     rdx, OPBuf_size
                je      %%case_OverFlow

                mov     dl, byte [rsi]

                cmp     dl, '%'
                je      %%Spec

                cmp     dl, 0           ; end of format string
                je      %%Break

                movsb
                inc     rcx

                jmp     %%Next

        %%case_OverFlow:
                mov     byte [rel RF], 1
                jmp     %%Break

        %%Spec:
                mov     byte [rel PF], 1

        %%Break:

        add     rax, rcx        ; update return value
        add     rbx, rcx        ; update shift in format string
        add     r12, rcx        ; update len of cur buffer

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

%macro CLEVER_DROP_BUFFER 0
        PUSH_REGS r13, rcx, rdx

        DROP_BUFFER
        xor     r12, r12
        mov     rdi, OPBuf

        POP_REGS  r13, rcx, rdx
%endmacro

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
section .text

global MyPrint
global GetFPValue

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
; rax - return value
; rbx - shift in format string
; r12 -  len of cur buffer
; rbp + 8  - ptr on return address
; rbp + 16 - ptr on first symbol of format string
; r13 - for different saves
; r14 - argument number

; rcx - using as a counter in cycles
; rsi - source in functions, shows the position in format string
; rdi - destination, shaws the positions in output buffer

; remaining Volatile-registers:
;       r15
;       xmm6 - xmm15
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MyPrint:
        ; save registers in shadow space
        mov     qword [rsp+8 ], rcx     ; save 1st argument
        mov     qword [rsp+16], rdx     ; save 2nd argument
        mov     qword [rsp+24], r8      ; save 3rd argument
        mov     qword [rsp+32], r9      ; save 4th argument

        ; Save Nonvolatile registers
        ; Save address beginning of arguments
        push    rbp
        lea     rbp, [rsp+16]    ; return address is on rbp  position
                                ; first argument is on rbp+8 position e.t.c.
        PUSH_REGS rdi, rsi, rbx, r12, r13, r14, r15

        ; save nonvolatile registers
        sub     rsp, 32
        movsd   [rsp],    xmm0
        movsd   [rsp+16], xmm6

        ; We must end our program with old rsp to save it
        ; We must implement this: push    r11 ; if we will use this registers

        xor     rax, rax        ; return value = NULL
        xor     rbx, rbx        ; shift in format string = NULL
        xor     r14, r14        ; set start value for argument counter

  Next:
        xor     r12, r12        ; len of cur buffer
        ; move part of format string to OPBuf
        mov     rsi, [rbp]   ; start of format string
        add     rsi, rbx        ; rsi = ptr in format string = address of start + shift, which equals number of processed symbols
        mov     rdi, OPBuf
    AfterPercent:
        COUNT_LEN_FOR_COPY_IN_OPBuf ; set: rcx = num of symbols to copy

        mov     cl, byte [rel PF]
        cmp     cl, 1
        je      Percent
    Drop:
        DROP_BUFFER

        cmp     byte [rel RF], 1
        je     Next

        ; restore Nonvolatile registers
        movsd   xmm6, [rsp+16]
        movsd   xmm0, [rsp]
        add     rsp, 32
        POP_REGS rbp, rdi, rsi, rbx, r12, r13, r14, r15

    ;-----------------------------------------------------------------------std printf call
    ;   ; restore args
    ;   mov     rcx, qword [rsp+8 ]
    ;   mov     rdx, qword [rsp+16]
    ;   mov     r8,  qword [rsp+24]
    ;   mov     r9,  qword [rsp+32]
    ;
    ;   mov     qword [rel MyRetVal], rax
    ;   pop     rax
    ;   mov     qword [rel MyRetAddr], rax      ; save return ptr for MyPrint in nonvolatile register
    ;
    ;   call    printf
    ;
    ;   mov     rax, qword [rel MyRetAddr]
    ;   push    rax
    ;   mov     rax, qword [rel MyRetVal]
    ;-----------------------------------------------------------------------std printf call

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
                movzx   rcx, byte [rsi]
                cmp     cl, '%'
                jne     AfterPercent
                        mov       byte [rdi], '%'
                        INC_REGS  rdi, r12, rax
                jmp     AfterPercent
    Correct:
        add     r14, 8             ; increment argument counter
        add     rbx, 2                  ; skip  % and  specifier
        add     rsi, 2                  ; '%' 'c' '*'
                                        ;          ^
                                        ;    rsi _/
        jmp     rcx
case_Character:
        ; in this case we need only one free byte in OPBuf
        cmp     r12, OPBuf_size
        jb      WriteCharacter
                DROP_BUFFER             ; if we don't have memory -> drop buffer
                xor     r12, r12
                mov     rdi, OPBuf
    WriteCharacter:
        push    rsi             ; save position in format string

        lea     rsi, [rbp+r14]       ; ptr on argument
        movsb   ;[rdi++], [rsi = (rbp+r14+16)]

        pop     rsi             ; restore position in format string

        INC_REGS rax, r12               ; increment return value, number of characters ; position in OPBuf

        jmp     AfterPercent
case_Decimal:
        cmp     r12, OPBuf_size-20
        jb      case_Decimal.Continue
                CLEVER_DROP_BUFFER

    .Continue:
        push    rax                     ; should save return value
        movsxd  rax, dword [r14+rbp]       ; eax = number value

        ; if value equal zero, we can do it faster:
        cmp     eax, 0
        jne     case_Decimal.Skip
                mov     byte [rdi], '0'
                pop     rax
                INC_REGS rax, rdi, r12
                jmp     AfterPercent
    .Skip:

        push    rax
        shr     eax, 31         ; lets see highest bit
        cmp     eax, 1
        pop     rax
        jne     case_Decimal.Positive
                ; set minus sign
                mov     byte [rdi], '-'
                INC_REGS rdi, r12
                mov     rcx, qword [rsp]
                inc     rcx             ; increment return value
                mov     qword [rsp], rcx
                ; make positive
                not     eax
                inc     rax
    .Positive:

        ; save in stack numbers in correct order
        mov     r11, 10         ; value for division
        xor     rcx, rcx        ; counter of numbers to write
    .NextDiv:
                xor     rdx, rdx
                div     r11             ; rax = [rax/10] ; rdx = rax - 10 * [rax/10]
                push    rdx
                inc     rcx
        cmp     rax, 0
        jne     case_Decimal.NextDiv

        mov     rdx, rcx        ; save number of writing bytes

        ; write numbers from stack:
    .NextNum:
                pop     rax
                add     rax, '0'
                mov     byte [rdi], al
                INC_REGS rdi, r12
                dec     rcx

        cmp     rcx, 0
        ja      case_Decimal.NextNum

        pop     rax
        add     rax, rdx

        jmp  AfterPercent

; Floating-point number structure:
;       ------------------------------------
;       |S| E (Exp) |     M (Mantissa)     |    <-- binary representation
;       |1| 11 bit  |        52 bit        |
;       ------------------------------------
; value = (-1)^S * (1 + M/2^52) * 2^(E - 1023)
;(You can see, how to convert from Binary to Decimal, for example in case_Decimal)
; special cases:
;                0.000000e+00  <==> S == 0      && E == 0      && M == 0
;               -0.000000e+00  <==> S == 1      && E == 0      && M == 0
;                inf           <==> S == 0      && E == 2^11-1 && M == 0
;               -inf           <==> S == 1      && E == 2^11-1 && M == 0
;                nan           <==> S == {0, 1} && E == 2^11-1 && M != 0
; Denormal numbers (-1)^S * (M/2^52) * 2^(E - 1022)  <==>  E == 0 && M != 0
case_Exp:
        cmp     r12, OPBuf_size-13 ; - (sign(1 or 0) + before_dot_sym() + dot + frac_symbols)
        jl      case_Exp.WriteSign; we must use signed conditional jump
                CLEVER_DROP_BUFFER
    .WriteSign:
    ; at zero --> identify place, where arg is (xmm or stack)
        call    GetFPValue      ; xmm0 = float-point argument
        movq    r13, xmm0

    ; at first --> test on special cases, sch as inf, nan, 0.000000e+00 and Denormal numbers
        ; get E
        mov     rdx, r13
        shl     rdx, 1      ; delete sign
        shr     rdx, 52+1   ; delete mantissa (+ one shift from delete sign)

        ;get M
        mov     rcx, r13
        shl     rcx, 12
        shr     rcx, 12

        cmp     rdx, 2047 ; E >=< 2^11-1
        jne     case_Exp.CheckZ

                cmp     rcx, 0
                je      case_Exp.INF

                        ; write nan
                        mov     rcx, NanLen
                        push    rsi
                        mov     rsi, NanStr
                        ; rdi already set
                        rep     movsb
                        pop     rsi
                        add     r12, NanLen
                        add     rax, NanLen

                        jmp     AfterPercent
            .INF:
                ; set sign
                shr     r13, 63 ; see highest bit = sign
                cmp     r13, 1
                jne     case_Exp.InfPos
                        mov     byte [rdi], '-'
                        INC_REGS rdi, r12, rax
                        mulsd   xmm0, [rel neg_one]
                .InfPos:
                ; write inf
                mov     rcx, InfLen
                push    rsi
                mov     rsi, InfStr
                ; rdi already set
                rep     movsb
                pop     rsi
                add     rax, InfLen
                add     r12, InfLen

                jmp     AfterPercent
        .CheckZ:
        cmp     rdx, 0    ; E >=< 0
        jne     case_Exp.Normal
                cmp     rcx, 0    ; M >=< 0
                jne     case_Exp.Normal
                        ; sign
                        shr     r13, 63 ; see highest bit = sign
                        cmp     r13, 1
                        jne     case_Exp.ZPos
                                mov     byte [rdi], '-'
                                INC_REGS rdi, r12, rax
                                mulsd   xmm0, [rel neg_one]
                        .ZPos:
                        ; write zero str "0.000000e+00"
                        mov     rcx, ZELen
                        push    rsi
                        mov     rsi, ZEStr
                        ; rdi already set
                        rep     movsb
                        pop     rsi
                        add     r12, ZELen
                        add     rax, ZELen
                        jmp     AfterPercent
        .Normal:
    ; at second --> set sign
        shr     r13, 63 ; see highest bit = sign
        cmp     r13, 1
        jne     case_Exp.Positive
                mov     byte [rdi], '-'
                INC_REGS rdi, r12, rax
                mulsd   xmm0, [rel neg_one]
        .Positive:

    ; at third --> normalize number
        xor     r13, r13        ; we will save here exp value
        .NormL:
        comisd  xmm0, [rel one] ; example
        jae     case_Exp.NormH
                mulsd   xmm0, [rel ten]
                dec     r13
                jmp     case_Exp.NormL
        .NormH:
        comisd  xmm0, [rel ten]
        jb      case_Exp.NormEnd
                divsd   xmm0, [rel ten]
                inc     r13
                jmp     case_Exp.NormH
        .NormEnd:

    ; at fourth --> display normalized mantissa with dot
        ; add round for frac part
        addsd     xmm0, [rel round_frac]

        ; rdx = [integer part]
        cvttsd2si rdx, xmm0     ; convert Scalar Double to signed integer (delete after dot part)
        cmp       rdx, 10
        jae       case_Exp.NormH

        add       rdx, '0'
        mov       byte [rdi], dl
        INC_REGS  rdi, r12, rax
        ; display dot
        mov       byte [rdi], '.'
        INC_REGS  rdi, r12, rax

        ; display other symbols
        sub       rdx, '0'      ; restore the integer part
        cvtsi2sd  xmm6, rdx     ; convert signed integer to signed double
        subsd     xmm0, xmm6    ; xmm0 = fractional part

        mov     rcx, 6
        .NextFrac:
                ; make integer
                mulsd     xmm0, [rel ten]
                cvttsd2si rdx, xmm0 ; convert Scalar Double to signed integer (delete after dot part)
                add       dl, '0'
                mov       byte [rdi], dl
                INC_REGS  rdi, r12, rax
                ; update xmm0
                sub       rdx, '0'
                cvtsi2sd  xmm6, rdx
                subsd     xmm0, xmm6
        dec     rcx
        jnz     case_Exp.NextFrac

    ; at fifth --> display pre exp equation (e- or e+) (we saved an exp in r13)
        ; set exp sym
        mov     byte [rdi], 'e'
        INC_REGS  rdi, r12, rax

        ; set exp sign
        cmp     r13, 0
        jge     case_Exp.PositiveExp
                mov     byte [rdi], '-'
                INC_REGS  rdi, r12, rax
                neg     r13
                jmp     case_Exp.ExpEnd
        .PositiveExp:
        mov     byte [rdi], '+'
        INC_REGS  rdi, r12, rax
        .ExpEnd:

    ; at sixth --> display abs value of exp
            push    rax
            mov     rax, r13
            xor     rdx, rdx
            mov     rcx, 10
            div     rcx             ; r13/10 ==>  al = tens, dl = units

        cmp     rax, 10
        jae     case_Exp.Three

            add     al, '0'
            mov     byte [rdi], al
            pop     rax
            INC_REGS  rdi, r12, rax

            add     dl, '0'
            mov     byte [rdi], dl
            INC_REGS  rdi, r12, rax

            jmp     AfterPercent

        .Three:
            mov     rbx, rdx

            xor     rdx, rdx
            div     rcx

            add     al, '0'
            mov     byte [rdi], al
            pop     rax
            INC_REGS  rdi, r12, rax

            add     dl, '0'
            mov     byte [rdi], dl
            INC_REGS  rdi, r12, rax

            add     bl, '0'
            mov     byte [rdi], bl
            INC_REGS  rdi, r12, rax


        jmp     AfterPercent
;------------------------------------------------------------------------------
; return float-point argument value in xmm0; Input params: r14 = arg_number * 8
; determinate list: r13
;------------------------------------------------------------------------------
GetFPValue:
        cmp     r14, 8*3; = sizeof(arg)*(MaxArgNum-1)
        ja      Stack
        lea     r13, [rel FPTable]
        jmp     [r13+r14]

    ;.xmm0:  <-- optional, because zero argument in MyPrint always is format string ptr (char*) - saved in rcx
    ; so we will used xmm0 such as return value
    case_xmm1: movsd    xmm0, xmm1
               ret
    case_xmm2: movsd    xmm0, xmm2
               ret
    case_xmm3: movsd    xmm0, xmm3
               ret
    Stack:     movsd    xmm0, [rbp+r14]
               ret
case_Float:
        cmp     r12, OPBuf_size-17 ; - (sign(1 or 0) + before_dot_sym() + dot + frac_symbols)
        jl      case_Float.WriteSign; we must use signed conditional jump
                CLEVER_DROP_BUFFER
    .WriteSign:
    ; at zero --> identify place, where arg is (xmm or stack)
        call    GetFPValue      ; xmm0 = float-point argument
        movq    r13, xmm0

    ; at first --> test on special cases, sch as inf, nan, 0.000000e+00 and Denormal numbers
        ; get E
        mov     rdx, r13
        shl     rdx, 1      ; delete sign
        shr     rdx, 52+1   ; delete mantissa (+ one shift from delete sign)

        ;get M
        mov     rcx, r13
        shl     rcx, 12
        shr     rcx, 12

        cmp     rdx, 2047 ; E >=< 2^11-1
        jne     case_Float.CheckZ

                cmp     rcx, 0
                je      case_Float.INF

                        ; write nan
                        mov     rcx, NanLen
                        push    rsi
                        mov     rsi, NanStr
                        ; rdi already set
                        rep     movsb
                        pop     rsi
                        add     r12, NanLen
                        add     rax, NanLen

                        jmp     AfterPercent
            .INF:
                ; set sign
                shr     r13, 63 ; see highest bit = sign
                cmp     r13, 1
                jne     case_Float.InfPos
                        mov     byte [rdi], '-'
                        INC_REGS rdi, r12, rax
                        mulsd   xmm0, [rel neg_one]
                .InfPos:
                ; write inf
                mov     rcx, InfLen
                push    rsi
                mov     rsi, InfStr
                ; rdi already set
                rep     movsb
                pop     rsi
                add     rax, InfLen
                add     r12, InfLen

                jmp     AfterPercent
        .CheckZ:
        cmp     rdx, 0    ; E >=< 0
        jne     case_Float.Normal
                cmp     rcx, 0    ; M >=< 0
                jne     case_Float.Normal
                        ; sign
                        shr     r13, 63 ; see highest bit = sign
                        cmp     r13, 1
                        jne     case_Float.ZPos
                                mov     byte [rdi], '-'
                                INC_REGS rdi, r12, rax
                                mulsd   xmm0, [rel neg_one]
                        .ZPos:
                        ; write zero str "0.000000e+00"
                        mov     rcx, ZFLen
                        push    rsi
                        mov     rsi, ZFStr
                        ; rdi already set
                        rep     movsb
                        pop     rsi
                        add     r12, ZFLen
                        add     rax, ZFLen
                        jmp     AfterPercent
        .Normal:
    ; at second --> set sign
        shr     r13, 63 ; see highest bit = sign
        cmp     r13, 1
        jne     case_Float.Positive
                mov     byte [rdi], '-'
                INC_REGS rdi, r12, rax
                mulsd   xmm0, [rel neg_one]
        .Positive:

    ; at third --> show integer part of float value

        push    rax

        cvttsd2si rax, xmm0
        cvtsi2sd  xmm6, rax     ; xmm6 == rax
        subsd     xmm0, xmm6    ; xmm0 == float part of float-double number

        ; save in stack numbers in correct order
        mov     r11, 10         ; value for division
        xor     rcx, rcx        ; counter of numbers to write
        .NextDiv:
                xor     rdx, rdx
                div     r11             ; rax = [rax/10] ; rdx = rax - 10 * [rax/10]
                push    rdx
                inc     rcx
        cmp     rax, 0
        jne     case_Float.NextDiv

        mov     rdx, rcx        ; save number of writing bytes

        ; write numbers from stack:
        .NextNum:
                pop     rax
                add     rax, '0'
                mov     byte [rdi], al
                INC_REGS rdi, r12
                dec     rcx

        cmp     rcx, 0
        ja      case_Float.NextNum

        pop     rax
        add     rax, rdx

    ; at fourth --> display dot
        mov       byte [rdi], '.'
        INC_REGS  rdi, r12, rax

    ; at fifth --> display after dot numbers:
        push      rax
        xor       rdx, rdx
    .NextADNum:
                mulsd     xmm0, [rel ten]
                cvttsd2si rax, xmm0
                cvtsi2sd  xmm6, rax
                subsd     xmm0, xmm6

                add       rax, '0'
                mov       byte [rdi], al
                INC_REGS  rdi, r12, rdx
        cmp     rdx, 6
        jb      case_Float.NextADNum

        pop     rax
        add     rax, 6

        jmp       AfterPercent
case_Global:
        jmp Drop
case_Octal:
        cmp     r12, OPBuf_size-22
        jb      case_Octal.Write
                        CLEVER_DROP_BUFFER
    .Write:
        ; in this case we can't use rep movsb, so we should make an individual proc
        mov     edx, [r14+rbp]       ; rbx = number value
        mov     r11, rdx                ; r11 = saved number value

        cmp     rdx, 0
        jne     case_Octal.Skip
                mov     byte [rdi], '0'
                INC_REGS rax, rdi, r12
                jmp     AfterPercent
    .Skip:
                                ;          \/------- 1 letter = 4 bits ; bit_size(r13) = 64
        mov     r13, -1         ; r13 = ffff ffff ffff ffff  --- we need ---> r13 = (0000 0000) 0000 000f - mask
        shr     r13, 64-3       ; bit_size(r13) - log_2(16)        optional >--------^^^^^^^^^

        xor     rcx, rcx        ; counter
        .SNext:
                mov     rdx, r11    ; get argument value
                and     rdx, r13    ; masking
                shr     rdx, cl
                push    rdx         ; save in stack
                shl     r13, 3      ; update mask

                add     rcx, 3

        cmp     rcx, 32
        jb      case_Octal.SNext

        .SkipZ:         ; skip first zeros
                pop     rdx
                cmp     rdx, 0
                jne     case_Octal.InDotW
                    sub     rcx, 3
                    jmp     case_Octal.SkipZ

        .WNext:
                pop     rdx
            .InDotW:
                add     rdx, '0'
                mov     byte [rdi], dl
                INC_REGS rax, r12, rdi
                sub     rcx, 3
        cmp     rcx, 0
        ja      case_Octal.WNext

        jmp     AfterPercent
case_String:
        ; in this case we don't know, how much free bytes we need, so we will write this string, such a format string
        ; only change source: rsi = absolute addr of symbol in format string    -->    rsi = absolute addr of symbol in argument string
        ;                     rbx = shift in format string on symbol            -->    rbx = shift in argument string on symbol
                                ; next values we will use to continue
        ; save params for work with format string
        push    rsi
        push    rbx

        xor     rbx, rbx        ; set shift in argument string

        mov     rsi, [r14+rbp]      ; now: rsi = *(rbp+r14*8+16) = start address of string

        push    rsi     ; save start address of argument string

    .Next:
        COUNT_LEN_FOR_COPY_IN_OPBuf

        cmp     byte [rel RF], 1
        jne     .End
                DROP_BUFFER
                xor     r12, r12
                mov     rsi, [rsp]
                add     rsi, rbx
                mov     rdi, OPBuf
                jmp     case_String.Next
    .End:
        add     rsp, 8         ; skip  saved start of address string
        pop     rbx
        pop     rsi

        jmp     AfterPercent
case_Hex:
        cmp     r12, OPBuf_size-16
        jb      case_Hex.Write
                CLEVER_DROP_BUFFER
    .Write:
        ; in this case we can't use rep movsb, so we should make an individual proc
        mov     edx, [r14+rbp]       ; rbx = number value
        mov     r11, rdx                ; r11 = saved number value

        cmp     rdx, 0
        jne     case_Hex.Skip
                mov     byte [rdi], '0'
                INC_REGS rax, rdi, r12
                jmp     AfterPercent
    .Skip:
                                ;          \/------- 1 letter = 4 bits ; bit_size(r13) = 64
        mov     r13, -1         ; r13 = ffff ffff ffff ffff  --- we need ---> r13 = (0000 0000) 0000 000f - mask
        shr     r13, 64-4       ; bit_size(r13) - log_2(16)        optional >--------^^^^^^^^^

        xor     rcx, rcx        ; counter
        .SNext:
                mov     rdx, r11    ; get argument value
                and     rdx, r13    ; masking
                shr     rdx, cl
                push    rdx         ; save in stack
                shl     r13, 4      ; update mask

                add     rcx, 4

        cmp     rcx, 32
        jb      case_Hex.SNext

        .SkipZ:         ; skip first zeros
                pop     rdx
                cmp     rdx, 0
                jne     case_Hex.InDotW
                    sub     rcx, 4
                    jmp     case_Hex.SkipZ

        .WNext:
                pop     rdx
            .InDotW:
                cmp     rdx, 10
                jae     case_Hex.Letter
                        add     rdx, '0'
                        mov     byte [rdi], dl
                        jmp     case_Hex.Inc
            .Letter:
                add     rdx, 'a'-10
                mov     byte [rdi],dl
            .Inc:
                INC_REGS rax, r12, rdi
                sub     rcx, 4
        cmp     rcx, 0
        ja      case_Hex.WNext

        jmp     AfterPercent

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
section .bss

OPBuf_size      equ  128
OPBuf:  resb OPBuf_size

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
section .data
MyRetVal        dq  0
MyRetAddr       dq  0

; float-point constants for work with xmm registers
one             dq  1.0
neg_one         dq -1.0
ten             dq  10.0
round_frac      dq  0.0000005

; Float-point different cases
NanStr:         db  "nan"
NanLen:        equ  $ - NanStr
InfStr:         db  "inf"
InfLen:        equ  $ - InfStr
ZEStr:         db  "0.000000e+00"
ZELen:        equ  $ - ZEStr
ZFStr:         db  "0.000000"
ZFLen:        equ  $ - ZFStr

; Flags of status format string
RF              db 0    ; repeat flag
PF              db 0    ; percent flag

; jmp table for float-point arguments
FPTable:
                dq 0
                dq case_xmm1
                dq case_xmm2
                dq case_xmm3

; jmp table for different cases of specifier
JmpTable:
times 'c'       dq 0                    ; ... -  a
                dq case_Character       ; c
                dq case_Decimal         ; d
                dq case_Exp             ; e
                dq case_Float           ; f
                dq case_Global          ; g
times 'o'-'g'-1 dq 0                    ; h, i, g, k, l, m, n
                dq case_Octal           ; o
times 's'-'o'-1 dq 0                    ; p, q, r
                dq case_String          ; s
times 'x'-'s'-1 dq 0                    ; t, u, v, w, x, y, z
                dq case_Hex             ; x
times 255-'x'-1 dq 0                    ; y - ...
