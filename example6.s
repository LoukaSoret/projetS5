.global main
.text
main:
    mov r1, #1
    mov r2, #26
    mov r4, #2
    mov r5, #38
    mov r6, #1
    mov r7, #2

    ldr r0, =adresse
    ldr r0, [r0]
    str r1,[r0], #4
    str r2,[r0]
    strh r4,[r0,r12]
    strb r5,[r0, #-8]!
    str r6,[r0]

    ldr r0, =adresse
    ldr r0, [r0]
    ldr r5,[r0], #4
    ldr r4,[r0]
    ldrh r2,[r0,r12]
    ldrb r1,[r0, #-8]!
    ldr r3,[r0]
    swi 0x123456

y:
    .word 0x25236521
x:
    .word 0x12345678
    .word 0x44444444
adresse:
    .word 0x500