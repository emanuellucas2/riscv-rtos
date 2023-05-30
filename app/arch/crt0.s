.equ REGBYTES, 4

.macro lxsp a, b
lw \a, ((\b)*REGBYTES)(sp)
.endm

.macro sxsp a, b
sw \a, ((\b)*REGBYTES)(sp)
.endm
    

.align 8
trap_vector:

    # Invoke the handler.
    csrr    a0, ucause
    csrr    a1, uepc


.section .e_entry
.globl _start
_start:
    
    # setup default trap vector
    la      t0, trap_vector
    #csrw    utvec, t0

    #la  a0, _bss_end
    #la  a1, _bss_start
BSS_CLEAR:
1:  #sw  zero, 0(a1)
    #addi a1, a1, 4
    #bne a1, a0, 1b
    #nop
	jal     main
    nop
2:  j 2b
    nop


	
	
