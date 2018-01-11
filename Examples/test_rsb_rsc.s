.global main
.text
main:
	 mov r0, #0xff
    mov r1, #0x34
 
    rsb r2, r1, r0
    rsbs r3, r1, r0	@C devient 1
    rsc r4, r1, r0 	@r4 = r0 - r1 - !(C=1) et C ne change pas
    rscs r5, r1, r0 	@r5 = r0 - r1 - !(C=1) et C reste a 1
    
    rsb r6, r1, #0xaf
    rsbs r7, r1, #0xaf	@C reste a 1
   
    adds r8, r0, #0x1 @C devient 0
    rsc r9, r1, #0xaf	@r8 = r1 - 0xaf - !(C=0) et C ne change pas
    rscs r10, r1, #0xaf	@r8 = r1 - 0xf8 - !(C=0) et C reste a 0
         
   swi 0x123456
