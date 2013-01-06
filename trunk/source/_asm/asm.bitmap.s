#include "../../include/func.minAsm.h"
/*
@ void copyTransparentFast( int width , int height , _pixelArray data , _pixelArray dest , int dataWidth , int srcWidth );
@ r0 = width , r1 = height , r2 = data , r3 = dest , stack[4] = dataWidth , stack[8] = srcWidth

BEGIN_FUNC_ARM( copyTransparentFast , CSEC_TEXT )

	push  	{ r4 , r5 , r6 , r7 , r8 }	@@ save the stack
	sub 	r0 , r0 , #1		@@ width--
	ldr		r4 , [sp]			@@ r4 = dataWidth - width
	sub		r4 , r0				@@ 
	
	ldr		r5 , [sp,#4]		@@ r5 = destWidth - width
	sub		r5 , r0				@@ 
	
loopStart:

	mov		r8 , r0				@@ counter = width
	
innerLoopStart:

	ldrh	r6 , [r2]			@@ r6 = *data
	mov		r7 , r6				@@ r7 = r6
	ror		r7 , #15			@@ r7 >>= 15
	and		r7 , #1				@@ r7 &= 0x1
	
	cmp		r7 , #0				@@ if( ( *data >> 15 ) & 0x1 == true )
	beq		innerLoopBeforeEnd	@@ {
	
	adr		r6 , hwords
	strh    r6 , [r3]			@@     r3 -> *dest = *data <- r6

innerLoopBeforeEnd:				@@ }

    add     r6 , #2				@@ data++
    add     r3 , #2				@@ dest++
	
	sub 	r8 , #1				@@ counter--
	cmp		r8 , #0				@@ if( counter != 0 )
	bne		innerLoopStart		@@     goto innerLoopStart

	add		r2 , r4				@@ data += r4 (line:15)
	add		r3 , r5				@@ dest += r5 (line:15)
	
	sub		r1 , r1 , #2		@@ height--
	cmp		r1 , #0				@@ if( height != 0 )
	bne		loopStart			@@     goto loopStart
	
	pop 	{ r4 , r5 , r6 , r7 , r8 } @@ restore the stack
	bx		lr

END_FUNC( copyTransparentFast )

hwords:
	.word 0x8FFF
*/