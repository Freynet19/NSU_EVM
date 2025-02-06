PiLeibniz:
        push    rbp                                 ; сохраняем старое значение регистра RBP
        mov     rbp, rsp                            ; помещаем указатель стека RSP в регистр RBP
        mov     QWORD PTR [rbp-40], rdi             ; записываем переменную n на стек со сдвигом 40; размер long long 8 байт
        pxor    xmm0, xmm0                          ; запись 0 в регистр xmm0 (xor сам себя)
        movsd   QWORD PTR [rbp-8], xmm0             ; double result [rbp-8] = xmm0
        mov     QWORD PTR [rbp-16], 0               ; long long i [rbp-16] = 0
        jmp     .L2                                 ; переход на L2
.L5:
        pxor    xmm0, xmm0                          ; зануление xmm0
        cvtsi2sd        xmm0, QWORD PTR [rbp-16]    ; приведение long long i [rbp-16] к double и запись в xmm0
        movapd  xmm1, xmm0                          ; xmm1 = xmm0
        addsd   xmm1, xmm0                          ; xmm1 += xmm0 т.е. умножили на 2
        movsd   xmm0, QWORD PTR .LC1[rip]           ; запись константы 1.0 в регистр xmm0
        addsd   xmm1, xmm0                          ; xmm1 = xmm0
        movsd   xmm0, QWORD PTR .LC1[rip]           ; запись константы 1.0 в регистр xmm0
        divsd   xmm0, xmm1                          ; xmm0 /= xmm1
        movsd   QWORD PTR [rbp-24], xmm0            ; запись xmm0 в double t [rbp-24]
        mov     rax, QWORD PTR [rbp-16]             ; запись i [rpb-16] в регистр rax
        and     eax, 1                              ; eax принимает значение последнего бита, i нечет->1, чет->0
        test    rax, rax                            ; проверка значения rax
        jne     .L3                                 ; если не равно 0, переход на L3
        movsd   xmm0, QWORD PTR [rbp-8]             ; иначе запись result [rbp-8] в регистр xmm0
        addsd   xmm0, QWORD PTR [rbp-24]            ; xmm0 += t
        jmp     .L4                                 ; переход на L4
.L3:
        movsd   xmm0, QWORD PTR [rbp-8]             ; запись result [rbp-8] в регистр xmm0
        subsd   xmm0, QWORD PTR [rbp-24]            ; xmm0 -= t
.L4:
        movsd   QWORD PTR [rbp-8], xmm0             ; result [rbp-8] = xmm0
        add     QWORD PTR [rbp-16], 1               ; i += 1
.L2:
        mov     rax, QWORD PTR [rbp-16]             ; запись i в rax
        cmp     rax, QWORD PTR [rbp-40]             ; сравнение i, n
        jl      .L5                                 ; если i < n то переход к L5
        movsd   xmm1, QWORD PTR [rbp-8]             ; иначе запись result [rbp-8] в регистр xmm1
        movsd   xmm0, QWORD PTR .LC2[rip]           ; запись константы 4 в регистр xmm0
        mulsd   xmm0, xmm1                          ; xmm0 *= xmm1
        pop     rbp                                 ; восстанавливаем в RBP значение для вызывающего кода
        ret                                         ; возвращаемся в вызывающий код
.LC3:
        .string "pi = %.8lf\n"
main:
        push    rbp                                 ; сохраняем старое значение регистра RBP
        mov     rbp, rsp                            ; помещаем указатель стека RSP в регистр RBP
        sub     rsp, 16                             ; выделяем место в стеке под локальные переменные
        mov     edi, 1000000000                     ; запись значения макроса N_CONST в регистр edi для использования его функцией
        call    PiLeibniz                           ; помещаем адрес воврата в стек, вызываем функцию
        movq    rax, xmm0                           ; запись всех битов из xmm0 в регистр rax
        mov     QWORD PTR [rbp-8], rax              ; запись значения rax в result [rbp-8]
        mov     rax, QWORD PTR [rbp-8]              ; запись значения result [rbp-8] в rax
        movq    xmm0, rax                           ; xmm0 = rax
        mov     edi, OFFSET FLAT:.LC3               ; запись константы-строки в edi
        mov     eax, 1                              ; запись кол-ва дробных аргументов printf в eax
        call    printf                              ; печать строки
        mov     eax, 0                              ; запись 0 в eax для возврата из main
        leave                                       ; <=> mov rsp, rbp; pop rbp
        ; удаляем локальные переменные и очищаем стек, восстанавливаем в RBP значение для вызывающего кода
        ret                                         ; возвращаемся в вызывающий код
.LC1:
        .long   0
        .long   1072693248
.LC2:
        .long   0
        .long   1074790400
