.global main
.text
main:
    mov r12, #2
    ldr r0, =x
    ldr r1, [r0], #4
    ldr r2, [r0]
    ldrh r3, [r0, -r12]
    ldrb r5, [r0, r12]
    ldrd r6, [r0, #-4]!
    ldrb r8, [r0]

    swi 0x123456

y:
    .word 0x25236521
x:
    .word 0x12345678
    .word 0x44444444
adresse:
    .word 0x500