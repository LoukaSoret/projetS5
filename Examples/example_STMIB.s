.global main
.text
main:
    ldr r0, =X
    ldr r1, [r0]
    ldr r2, [r0,#4]
    ldr r3, [r0,#8]
    ldr r4, [r0,#12]
    stmib r0,{r2,r3,r4}
    swi 0x123456
.data
X:
	.word 0x1
	.word 0x2
	.word 0x3
	.word 0x4