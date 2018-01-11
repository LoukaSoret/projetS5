.global main
.text
main:
	mov r0, #0x83
	mov r1, r0
	mov r2, #0x2
	
	mov r3, r0, ror #2
	mov r4, r0, ror r2
	
	@Positifs
	mov r5, r0, asr #4
	mov r6, r0, asr r2
	
	@Negatifs
	mov r7, r4, asr #4
	mov r8, r4, asr r2
	
	mov r9, r0, lsl #8
	mov r10, r0, lsl r2
	
	mov r11, r9, lsr #5
	mov r12, r10, lsr r2
   
   swi 0x123456
