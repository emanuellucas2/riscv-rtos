#include <libc.h>
#include <timer.h>
#include <malloc.h>
#include <proc.h>
#include <task.h>
#include <config.h>
#include <gpr_context.h>
#include <vtimer.h>
#include <mutex.h>
#include <semaphore.h>
#include <shared_mem.h>

void hyper_init(){

    init_mem();

    syscalls();

	reset_tasks();

	initialize_tasks();

	start_timer();

	printf("Error!!!\n");

}

/**
 * @brief Extract the syscall code from syscall instruction. 
 * 
 * @return Syscall code.
 */
uint32_t getSyscallCode(){	
	return MoveFromPreviousGuestGPR(REG_A7);

}


void ei(){

	set_csr_bits(mstatus, MSTATUS_MIE);

}

void di(){

	clear_csr_bits(mstatus, MSTATUS_MIE);

}

/**
 * @brief Register each syscall. 

 */
void syscalls(){
	vtimer_init();
	mutex_init();
	semaphore_init();
	shared_mem_init();
}
