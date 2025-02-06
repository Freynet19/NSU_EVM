PiLeibniz:
        test    rdi, rdi                        ; проверка чему равен n [rdi]
        jle     .L6                             ; если <= 0 то переход к L6, иначе идем дальше
        mov     eax, 0                          ; i [eax] = 0
        pxor    xmm0, xmm0                      ; зануление result [xmm0]
        movsd   xmm2, QWORD PTR .LC1[rip]       ; запись константы 1.0 в xmm2
        jmp     .L5                             ; переход к L5
.L3:
        subsd   xmm0, xmm3                      ; result [xmm0] -= t [xmm3]
.L4:
        add     rax, 1                          ; i += 1
        cmp     rdi, rax                        ; сравнение n [rdi], i [rax]
        je      .L2                             ; если равны переход к L2, иначе к L5
.L5:
        pxor    xmm1, xmm1                      ; зануление xmm1
        cvtsi2sd        xmm1, rax               ; приведение i [rax] к double и запись в xmm1
        addsd   xmm1, xmm1                      ; xmm1 *= 2
        addsd   xmm1, xmm2                      ; xmm1 += 1.0 [xmm2]
        movapd  xmm3, xmm2                      ; t [xmm3]  = 1.0 [xmm2]
        divsd   xmm3, xmm1                      ; t [xmm3] /= xmm1
        test    al, 1                           ; проверка последнего бита в i [rax]
        jne     .L3                             ; если 1 (i нечет) то переход к L3
        addsd   xmm0, xmm3                      ; иначе (i чет) result [xmm0] += t [xmm3]
        jmp     .L4                             ; переход к L4
.L6:
        pxor    xmm0, xmm0                      ; зануление result [xmm0]
.L2:
        mulsd   xmm0, QWORD PTR .LC2[rip]       ; result [xmm0] *= 4.0
        ret                                     ;
.LC3:
        .string "pi = %.8lf\n"
main:
        sub     rsp, 8                          ; выделяем место в стеке под локальные переменные
        mov     edi, 1000000000                 ; запись значения макроса N_CONST в регистр edi для использования его функцией
        call    PiLeibniz                       ; помещаем адрес воврата в стек, вызываем функцию
        mov     edi, OFFSET FLAT:.LC3           ; запись константы-строки в edi
        mov     eax, 1                          ; запись кол-ва дробных аргументов printf в eax
        call    printf                          ; печать строки
        mov     eax, 0                          ; запись 0 в eax для возврата из main
        add     rsp, 8                          ; освобождаем стек
        ret                                     ; возвращаемся в вызывающий код
.LC1:
        .long   0
        .long   1072693248
.LC2:
        .long   0
        .long   1074790400
