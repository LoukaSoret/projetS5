.global main
.text
main:
	mov r0, #0x82
	mov r1, #0x83
	mov r2, #0x83
	mov r4, #0x82
	
	cmp r0, #0x82
	beq test
	
	cmp r0, r1
	beq test_fail
	
	add r2, r0, r1
	
test:
	add r5, r0, r1
	
	cmp r0, #0x83
	beq test_fail
	
	cmp r1, r2
	beq end
	
test_fail:
	add r6, r0, r1

end:	
	add r7, r0, r1
	swi 0x123456
