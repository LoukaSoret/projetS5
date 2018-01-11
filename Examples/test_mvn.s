.global main
.text
main:
	mvn r0, #0x80
	mov r1, #0xff
	mvn r2, r1
	
	mvn r3, #0x50
	mvn r4, r3
	swi 0x123456
