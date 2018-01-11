.global main
.text
main:
	 mov r0, #0xff
    mov r1, #0x34
 
    sub r2, r0, r1
    subs r3, r0, r1	@C devient 1
    sbc r4, r0, r1 	@r4 = r0 - r1 - !(C=1) et C ne change pas
    sbcs r5, r0, r1	@r5 = r0 - r1 - !(C=1) et C reste a 1
    
    sub r6, r0, #0xaf
    subs r7, r0, #0xaf	@C reste a 1
   
    adds r8, r0, #0x1 @C devient 0
    sbc r9, r0, #0xaf	@r8 = r1 - 0xaf - !(C=0) et C ne change pas
    sbcs r10, r0, #0xaf	@r8 = r1 - 0xf8 - !(C=0) et C reste a 0
         
   swi 0x123456
