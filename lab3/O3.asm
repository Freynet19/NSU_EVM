PiLeibniz:
        test    rdi, rdi
        jle     .L6
        movsd   xmm2, QWORD PTR .LC1[rip]
        xor     eax, eax
        pxor    xmm0, xmm0
        jmp     .L5
.L10:
        add     rax, 1
        addsd   xmm0, xmm3
        cmp     rdi, rax
        je      .L9
.L5:
        pxor    xmm1, xmm1
        movapd  xmm3, xmm2
        cvtsi2sd        xmm1, rax
        addsd   xmm1, xmm1
        addsd   xmm1, xmm2
        divsd   xmm3, xmm1
        test    al, 1
        je      .L10
        add     rax, 1
        subsd   xmm0, xmm3
        cmp     rdi, rax
        jne     .L5
.L9:
        mulsd   xmm0, QWORD PTR .LC2[rip]
        ret
.L6:
        pxor    xmm0, xmm0
        ret
.LC3:
        .string "pi = %.8lf\n"
main:
        sub     rsp, 8
        movsd   xmm2, QWORD PTR .LC1[rip]
        pxor    xmm0, xmm0
        xor     eax, eax
.L15:
        pxor    xmm1, xmm1
        movapd  xmm3, xmm2
        cvtsi2sd        xmm1, rax
        addsd   xmm1, xmm1
        addsd   xmm1, xmm2
        divsd   xmm3, xmm1
        test    al, 1
        jne     .L12
        addsd   xmm0, xmm3
        add     rax, 1
        jmp     .L15
.L12:
        add     rax, 1
        subsd   xmm0, xmm3
        cmp     rax, 1000000000
        jne     .L15
        mulsd   xmm0, QWORD PTR .LC2[rip]
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
