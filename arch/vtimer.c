#include <hal.h>
#include <libc.h>
#include <proc.h>
#include <syscall_defines.h>
#include <syscall.h>

/**
 * @brief Syscall implementation. Returns the mtime register value. 
 * V0 guest register will be replaced with the mtime register value. 
 */
static void get_mtimer(){
	uint32_t mtime = MTIME;
	
	MoveToPreviousGuestGPR(REG_A0,mtime);
}

/**vtimer_init
 * @brief Driver init call.  Registers the hypercalls. 
 */
void vtimer_init(){
	if (register_syscall(get_mtimer, SCALL_GET_MTIMER_VALUE) < 0){
		printf("Error registering the SCALL_GET_MTIMER_VALUE syscall");
		return;
	}
    
	printf("\nSyscall MTimer Value implemented.");
	
}




