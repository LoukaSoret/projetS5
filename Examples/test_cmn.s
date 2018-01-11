.global main
.text
main:
	mov r0, #0x82
	mvn r1, r0
	mov r2, #0x83
	mvn r3, r2
	
	cmn r0, r1
	beq test
	
	cmn r0, r3
	beq test_fail
	
	add r2, r0, r1
	
test:
	add r5, r0, r1
	
	cmn r1, #0x55
	beq test_fail
	
	cmn r1, #0x82
	beq end
	
test_fail:
	add r6, r0, r1

end:	
	add r7, r0, r1
	swi 0x123456
