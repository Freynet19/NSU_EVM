PiLeibniz:
        test    rdi, rdi                        ; проверка чему равен n [rdi]
        jle     .L6                             ; если <= 0 то переход к L6, иначе идем дальше
        movsd   xmm2, QWORD PTR .LC1[rip]       ; запись константы 1.0 в xmm2
        xor     eax, eax                        ; зануление i [eax]
        pxor    xmm0, xmm0                      ; зануление result [xmm0]
        jmp     .L5                             ; переход к L5
.L10:
        add     rax, 1                          ; i [rax] += 1
        addsd   xmm0, xmm3                      ; result [xmm0] += t [xmm3]
        cmp     rdi, rax                        ; сравнение n [rdi], i [rax]
        je      .L9                             ; если равны то переход к L9, иначе к L5
.L5:
        pxor    xmm1, xmm1                      ; зануление xmm1
        movapd  xmm3, xmm2                      ; t [xmm3] = 1.0 [xmm2]
        cvtsi2sd        xmm1, rax               ; xmm1  = (double)i [rax]
        addsd   xmm1, xmm1                      ; xmm1 *= 2
        addsd   xmm1, xmm2                      ; xmm1 += 1.0 [xmm2]
        divsd   xmm3, xmm1                      ; t [xmm3] /= xmm1
        test    al, 1                           ; проверка последнего бита в i [rax]
        je      .L10                            ; если 0 (i чет) то переход к L10
        add     rax, 1                          ; иначе i [rax] += 1
        subsd   xmm0, xmm3                      ; result [xmm0] -= t [xmm3]
        cmp     rdi, rax                        ; сравнение n [rdi], i [rax]
        jne     .L5                             ; если не равны (т.е. i<n) то переход к L5, иначе к L9
.L9:
        mulsd   xmm0, QWORD PTR .LC2[rip]       ; result [xmm0] *= 4
        ret                                     ;
.L6:
        pxor    xmm0, xmm0                      ; зануление result [xmm0]
        ret                                     ;
.LC3:
        .string "pi = %.8lf\n"
main:
        sub     rsp, 8
        mov     edi, 1000000000
        call    PiLeibniz
        mov     edi, OFFSET FLAT:.LC3
        mov     eax, 1
        call    printf
        xor     eax, eax
        add     rsp, 8
        ret
.LC1:
        .long   0
        .long   1072693248
.LC2:
        .long   0
        .long   1074790400
