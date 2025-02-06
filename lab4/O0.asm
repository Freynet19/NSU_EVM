PiLeibniz:
        sub     sp, sp, #48             ; выделение места на стеке под переменные
        str     x0, [sp, 8]             ; n [sp+8] = x0
        str     xzr, [sp, 40]           ; result [sp+40] = 0
        str     xzr, [sp, 32]           ; i [sp+32] = 0
        b       .L2                     ; переход к L2
.L5:
        ldr     d31, [sp, 32]           ; d31 = i [sp+32]
        scvtf   d31, d31                ; d31 = (double)i
        fadd    d30, d31, d31           ; d30 = 2*d31
        fmov    d31, 1.0e+0             ; d31 = 1.0
        fadd    d31, d30, d31           ; d31 += d30
        fmov    d30, 1.0e+0             ; d30 = 1.0
        fdiv    d31, d30, d31           ; d31 = 1.0 [d30] / d31
        str     d31, [sp, 24]           ; t [sp+24] = d31
        ldr     x0, [sp, 32]            ; x0 = i [sp+32]
        and     x0, x0, 1               ; взятие последнего бита x0
        cmp     x0, 0                   ; сравнение его с 0
        bne     .L3                     ; если != то переход к L3 (i нечет)
        ldr     d30, [sp, 40]           ; d30 = result [sp+40]
        ldr     d31, [sp, 24]           ; d31 = t [sp+24]
        fadd    d31, d30, d31           ; d31 = d30 + d31
        b       .L4                     ; переход к L4
.L3:
        ldr     d30, [sp, 40]           ; d30 = result [sp+40]
        ldr     d31, [sp, 24]           ; d31 = t [sp+24]
        fsub    d31, d30, d31           ; d31 = d30 - d31
.L4:
        str     d31, [sp, 40]           ; result [sp+40] = d31
        ldr     x0, [sp, 32]            ; x0 = i [sp+32]
        add     x0, x0, 1               ; x0 += 1
        str     x0, [sp, 32]            ; i [sp+32] = x0
.L2:
        ldr     x1, [sp, 32]            ; x1 = i [sp+32]
        ldr     x0, [sp, 8]             ; x0 = n [sp+8]
        cmp     x1, x0                  ; сравнение x1, x0
        blt     .L5                     ; если < то переход к L5
        ldr     d30, [sp, 40]           ; d30 = result [sp+40]
        fmov    d31, 4.0e+0             ; d31 = 4.0
        fmul    d31, d30, d31           ; d31 *= d30
        fmov    d0, d31                 ; d0 = d31
        add     sp, sp, 48              ; восстановление указателя стека
        ret
.LC0:
        .string "pi = %.8lf\n"
main:
        stp     x29, x30, [sp, -32]!    ; сохранение frame pointer и link register, уменьшение указателя стека на 32
        mov     x29, sp                 ; x29 = текущий указатель стека
        mov     x0, 51712               ; x0 = 51712
        movk    x0, 0x3b9a, lsl 16      ; x0: установка 15258 со сдвигом 16бит, получилось n = 15258*2^16 + 51712 = 10^9
        bl      PiLeibniz               ; вызов функции
        str     d0, [sp, 24]            ; result [sp+24] = d0
        ldr     d0, [sp, 24]            ; d0 = result [sp+24]
        adrp    x0, .LC0                ; x0 = адрес константной строки
        add     x0, x0, :lo12:.LC0      ; x0 += младшие 12бит адреса
        bl      printf                  ; вызов printf
        mov     w0, 0                   ; w0 = 0 для return
        ldp     x29, x30, [sp], 32      ; восстановление FR и LR из стека, увеличение указателя стека на 32
        ret
