PiLeibniz:
        cmp     x0, 0                   ; сравнение n [x0], 0
        ble     .L6                     ; если <= то переход к L6
        mov     x1, 0                   ; i [x1] = 0
        movi    d30, #0                 ; result [d30] = 0.0
        fmov    d29, 1.0e+0             ; d29 = 1.0
        b       .L5                     ; переход к L5
.L3:
        fsub    d30, d30, d31           ; result [d30] -= t [d31]
.L4:
        add     x1, x1, 1               ; i [x1] += 1
        cmp     x0, x1                  ; сравнение n [x0], i [x1]
        beq     .L2                     ; если == то переход к L2
.L5:
        scvtf   d31, x1                 ; d31 = (double)i [x1]
        fadd    d31, d31, d31           ; d31 += d31
        fadd    d31, d31, d29           ; d31 += 1.0 [d29]
        fdiv    d31, d29, d31           ; t [d31] = 1.0 [d29] / d31
        tbnz    x1, 0, .L3              ; последний бит x1: если !=0 то переход к L3 (i нечет)
        fadd    d30, d30, d31           ; result [d30] += t [d31]
        b       .L4                     ; переход к L4
.L6:
        movi    d30, #0                 ; result [d30] = 0
.L2:
        fmov    d0, 4.0e+0              ; d0 = 4.0
        fmul    d0, d30, d0             ; d0 *= result [d30]
        ret
.LC0:
        .string "pi = %.8lf\n"
main:
        stp     x29, x30, [sp, -16]!    ; то же самое, но уменьшение указателя стека на 16
        mov     x29, sp
        mov     x0, 51712
        movk    x0, 0x3b9a, lsl 16
        bl      PiLeibniz
        adrp    x0, .LC0
        add     x0, x0, :lo12:.LC0
        bl      printf
        mov     w0, 0
        ldp     x29, x30, [sp], 16
        ret
