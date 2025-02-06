PiLeibniz:
        cmp     x0, 0                   ; сравнение n [x0], 0
        ble     .L6                     ; если <= то переход к L6
        movi    d29, #0                 ; result [d29] = 0
        mov     x1, 0                   ; i [x1] = 0
        fmov    d30, 2.0e+0             ; d30 = 2.0
        fmov    d31, 1.0e+0             ; d31 = 1.0
        b       .L5                     ; переход к L5
.L10:
        add     x1, x1, 1               ; i [x1] += 1
        fadd    d29, d29, d1            ; result
        cmp     x0, x1                  ; сравнение n [x0], i [x1]
        beq     .L9                     ; если == то переход к L9
.L5:
        scvtf   d1, x1                  ; d1 = (double)i [x1]
        fmadd   d1, d1, d30, d31        ; d1 = d1 * 2.0 [d30] + 1.0 [d31]
        fdiv    d1, d31, d1             ; t [d1] = 1.0 [d31] / d1
        tbz     x1, 0, .L10             ; последний бит x1: если ==0 то переход к L10 (i чет)
        add     x1, x1, 1               ; i [x1] += 1
        fsub    d29, d29, d1            ; result [d29] -= t [d1]
        cmp     x0, x1                  ; сравнение n [x0], i [x1]
        bne     .L5                     ; если != то переход к L5
.L9:
        fmov    d28, 4.0e+0             ; d28 = 4.0
        fmul    d0, d29, d28            ; d0 = result [d29] * 4.0 [d28]
        ret
.L6:
        movi    d0, #0                  ; d0 = 0
        ret
.LC0:
        .string "pi = %.8lf\n"
main:
        stp     x29, x30, [sp, -16]!
        mov     x0, 51712
        movk    x0, 0x3b9a, lsl 16
        mov     x29, sp
        bl      PiLeibniz
        adrp    x0, .LC0
        add     x0, x0, :lo12:.LC0
        bl      printf
        mov     w0, 0
        ldp     x29, x30, [sp], 16
        ret
