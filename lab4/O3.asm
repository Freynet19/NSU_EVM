PiLeibniz:
        cmp     x0, 0
        ble     .L6
        movi    d29, #0
        mov     x1, 0
        fmov    d30, 2.0e+0
        fmov    d31, 1.0e+0
        b       .L5
.L10:
        add     x1, x1, 1
        fadd    d29, d29, d1
        cmp     x0, x1
        beq     .L9
.L5:
        scvtf   d1, x1
        fmadd   d1, d1, d30, d31
        fdiv    d1, d31, d1
        tbz     x1, 0, .L10
        add     x1, x1, 1
        fsub    d29, d29, d1
        cmp     x0, x1
        bne     .L5
.L9:
        fmov    d28, 4.0e+0
        fmul    d0, d29, d28
        ret
.L6:
        movi    d0, #0
        ret
.LC0:
        .string "pi = %.8lf\n"
main:
        movi    d30, #0     ; команды из О2 но с инлайном в main
        mov     x1, 51712
        stp     x29, x30, [sp, -16]!
        mov     x0, 0
        fmov    d28, 2.0e+0
        fmov    d29, 1.0e+0
        movk    x1, 0x3b9a, lsl 16
        mov     x29, sp
.L15:
        scvtf   d31, x0
        fmadd   d31, d31, d28, d29
        fdiv    d31, d29, d31
        tbnz    x0, 0, .L12
.L17:
        add     x0, x0, 1
        fadd    d30, d30, d31
        scvtf   d31, x0
        fmadd   d31, d31, d28, d29
        fdiv    d31, d29, d31
        tbz     x0, 0, .L17
.L12:
        add     x0, x0, 1
        fsub    d30, d30, d31
        cmp     x0, x1
        bne     .L15
        fmov    d0, 4.0e+0
        adrp    x0, .LC0
        add     x0, x0, :lo12:.LC0
        fmul    d0, d30, d0
        bl      printf
        mov     w0, 0
        ldp     x29, x30, [sp], 16
        ret
