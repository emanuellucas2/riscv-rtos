/**
 * @file exception.c
 * 
 * @section DESCRIPTION
 * 
 * General exception handler implementation.  
 * 
 */

#include <types.h>
#include <exception.h>
#include <libc.h>
#include <proc.h>
#include <timer.h>
#include <scheduler.h>
#include <task.h>


uint32_t count = 0;

/**
 * @brief Guest exit exception handler. 
 * 
 */
void guest_exit_exception(uint32_t cause, uint32_t mepc){
	uint32_t mstatus;

	mstatus = read_csr(mstatus);

	switch (cause) {
		case IRQ_U_EXT:	
			syscall_execution();
			break;
		default:
			printf("Error %d mepc:0x%x mstatus:0x%x\n",cause, mepc, mstatus);
			task_in_execution->state = TASK_BLOCKED;
			break;
	}

	/* Advance to the next instruction. */
	task_in_execution->pc = mepc+4;

	setEPC(task_in_execution->pc);	

	if(task_in_execution->state == TASK_BLOCKED){
		run_scheduler();
	}

}

/**
 * @brief General exception handler. 
 * 
 */
void general_exception_handler(uint32_t mcause, uint32_t mepc){
	uint32_t cause = read_csr(mcause) & MCAUSE_MASK;

	/* Interruption */
	if(MCAUSE_INT & mcause){
		switch(cause){
			case IRQ_M_TIMER:
				timer_interrupt_handler();
				break;
			default:
				printf("\nDefault Interruption\n");
		}
	}else{ /* Exceptions */ 
		guest_exit_exception(cause, mepc);
	}
}
