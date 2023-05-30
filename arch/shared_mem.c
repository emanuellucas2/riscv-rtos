#include <hal.h>
#include <libc.h>
#include <proc.h>
#include <syscall_defines.h>
#include <syscall.h>
#include <task.h>
#include <mutex.h>
#include <shared_mem.h>
#include <scheduler.h>


static void SharedMemory_Init() {

    task_in_execution->shared_mem = NULL;

}


static void SharedMemory_Write() {

    uint32_t task_id = (uint32_t)MoveFromPreviousGuestGPR(REG_A0);
    uint32_t data = (uint32_t)MoveFromPreviousGuestGPR(REG_A1);

    vector_tasks[task_id].shared_mem = data;

}


static void SharedMemory_Read() {

    uint32_t task_id = (uint32_t)MoveFromPreviousGuestGPR(REG_A0);

    uint32_t mem = vector_tasks[task_id].shared_mem;

    MoveToPreviousGuestGPR(REG_A0,mem);
    
}

/**
 * @brief Driver init call.  Registers the hypercalls. 
 */
void shared_mem_init(){
    if (register_syscall(SharedMemory_Init, SCALL_SHARED_MEM_INIT) < 0){
        printf("Error registering the SCALL_SHARED_MEM_INIT syscall");
        return;
    }

    if (register_syscall(SharedMemory_Write, SCALL_SHARED_MEM_WRITE) < 0){
        printf("Error registering the SCALL_SHARED_MEM_WRITE syscall");
        return;
    }

    if (register_syscall(SharedMemory_Read, SCALL_SHARED_MEM_READ) < 0){
        printf("Error registering the SCALL_SHARED_MEM_READ syscall");
        return;
    }
    
    printf("\nSyscalls Shared Mem implemented.");
    
}
