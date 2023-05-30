#include <syscall.h>
#include <scheduler.h>
#include <hal.h>
#include <proc.h>
#include <gpr_context.h>

/**
 * @brief Syscalls table 
 *
 * The syscall table is small to keep small footprint as well. It can be increased 
 * throught SCALL_TABLE_START_SIZE 
 */
static syscall_t* syscall_table[SCALL_TABLE_SIZE];

/**
 * @brief Register a syscall. 
 * @param sys Function pointer to the syscall code. 
 * @param code Syscall code.
 * @return Greater than zero on success, otherwise error code. 
 */
int32_t register_syscall(syscall_t* sys, uint32_t code){
    if (code > SCALL_TABLE_SIZE-1 || code < 0){
        return SCALL_CODE_INVALID;
    }
    
    /* Syscall code already registered. */
    if(syscall_table[code]){
        return SCALL_CODE_USED;
    }
    
    /* Register the syscall */
    syscall_table[code] = sys;
    
    return code;
}

/**
 * @brief Called during guest exit exception to execute the required syscall.
 */
void syscall_execution(){
    /* Get the syscall code */
    uint32_t code = getSyscallCode();

    /* Syscall not implemented */
    if ( (code > SCALL_TABLE_SIZE-1) || (code < 0) || (!syscall_table[code])){
        MoveToPreviousGuestGPR(REG_A0, SCALL_NOT_IMPLEMENTED);
        return;
    }
    

    /* Execute the syscall */
    syscall_table[code]();
    
}

