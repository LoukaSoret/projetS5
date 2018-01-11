.global main
.text
main:
    ldr r0, =limite+0x14
    ldmda r0, {r2,r3,r4,r5}
    swi 0x123456
.data
limite:
    .word 0x00000001
    .word 0x00000002
    .word 0x00000003
    .word 0x00000004
    .word 0x00000005

