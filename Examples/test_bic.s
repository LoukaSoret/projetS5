.global main
.text
main:
	mov r0, #0xff
	bic r1, r0, #0x82
	bic r2, r0, #0x0
	mov r3, #0xf3
	mov r4, #0xffffffff
	
	@On utilse ce sub pour mettre le flag C a 1
	subs r5, r3, r4
	bics r6, r0, r3
	swi 0x123456
