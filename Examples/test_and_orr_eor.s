.global main
.text
main:
	mov r0, #0xfa
	mov r1, #0x13
    
   and r2, r0, r1
   and r3, r1, #0x31
    
   orr r4, r0, r1
   orr r5, r1, #0x31
    
   eor r6, r0, r1
   eor r7, r1, #0x31
   
   swi 0x123456
