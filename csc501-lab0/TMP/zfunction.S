.section .text

.globl zfunction
.type zfunction,@function

zfunction:

	pushl %ebp
	movl %esp,%ebp

	movl 8(%ebp),%eax
	andl $0xffc01fff,%eax
	shll $4,%eax	

	#Clear from the 10th to 18th bits,
	#shift the parameter param by 4 bits to the left, 
	#and then fill the right most bits with 0
	#The x86 processor uses the little-endian format. Thus, the leftmost bit is the 0th bit.

	movl %ebp,%esp
	popl %ebp
	ret

