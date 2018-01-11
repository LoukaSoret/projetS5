.global main
.text
main:
	mov r0, #0xff
	mov r1, r0, lsl #24
	mov r2, #0x1
	mov r3, r2, lsl #24
	
	add r4, r1, r3		@debordement mais pas de changement de C
	adds r5, r1, r3	@C devient 1
	adc r6, r0, r3		@r6 = r0 + r3 + C=1 et C ne change pas
	adcs r7, r0, r3	@C devient 0
	
	add r8, r0, #0x35		
	adds r9, r0, #0x89	@C reste a 0
	adc r10, r0, #0x54	@r10 = r0 + r3 + C=0 et C ne change pas
	adcs r11, r0, #0xaf	@r11 = r0 + r3 + 0xaf et C reste a 0
   
   swi 0x123456
