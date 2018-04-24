/* pfintr.S - pfintr */
/*

        1) store error code in a global variable
        2) store flag register
        3) clear all interrupts
        4) save all general registers
        5) page fault handling
        6) restore general registers
        7) restore flag register
        8) iret

*/

    	   .text
pferrcode: .long 0
           .globl  pfintr,pferrcode 
pfintr:
			popl pferrcode
			pushfl
			cli
			pushal
			call pfint
			popal
			popfl
			iret
