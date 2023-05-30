#include <hal.h>
#include <libc.h>
#include <proc.h>
#include <syscall_defines.h>
#include <syscall.h>
#include <task.h>
#include <mutex.h>
#include <scheduler.h>

static void Mutex_Init() {

    Mutex_t* mutex = (Mutex_t*)task_in_execution->mutex;

    mutex->locked = 0;
    mutex->num_waiting = 0;

    for (uint8_t i = 0; i < MAX_TASKS; i++) {

             mutex->waiting_tasks[i] = 0;
    }

}

static void Mutex_Lock() {

    uint32_t id = MoveFromPreviousGuestGPR(REG_A0);

    Mutex_t* mutex = (Mutex_t*)vector_tasks[id].mutex;

    if (mutex->locked == 1) {
        // Add the current task to the waiting list
        mutex->waiting_tasks[mutex->num_waiting] = task_in_execution->id;
        mutex->num_waiting++;
        
       task_in_execution->state = TASK_BLOCKED;
    } else {
        // The mutex is available, so lock it
        mutex->locked = 1;
    }

}

static void Mutex_TryLock() {

    uint32_t id = MoveFromPreviousGuestGPR(REG_A0);

    Mutex_t* mutex = (Mutex_t*)vector_tasks[id].mutex;

    if (mutex->locked == 0) {
        MoveToPreviousGuestGPR(REG_A0,0);
    } else {
        // The mutex is available, so lock it
        mutex->locked = 1;
        MoveToPreviousGuestGPR(REG_A0,1);
    }
}

static void Mutex_Unlock() {

    uint32_t id = MoveFromPreviousGuestGPR(REG_A0);

    Mutex_t* mutex = (Mutex_t*)vector_tasks[id].mutex;

    if (mutex->num_waiting > 0) {
        
        // There are tasks waiting for the mutex, so wake up the first one
        uint32_t task_id = mutex->waiting_tasks[0];
        // Remove the task from the waiting list
        mutex->num_waiting--;
        for (uint32_t i = 0; i < mutex->num_waiting; i++){
            mutex->waiting_tasks[i] = mutex->waiting_tasks[i+1];
        }
        
        vector_tasks[task_id].state = TASK_RUNNING;

    } else {
        // No tasks are waiting for the mutex, so unlock it
        mutex->locked = 0;
    }

}

/**
 * @brief Driver init call.  Registers the hypercalls. 
 */
void mutex_init(){
	if (register_syscall(Mutex_Init, SCALL_MUTEX_INIT) < 0){
		printf("Error registering the SCALL_MUTEX_INIT syscall");
		return;
	}

    if (register_syscall(Mutex_Lock, SCALL_MUTEX_LOCK) < 0){
        printf("Error registering the SCALL_MUTEX_LOCK syscall");
        return;
    }

    if (register_syscall(Mutex_TryLock, SCALL_MUTEX_TRYLOCK) < 0){
        printf("Error registering the SCALL_MUTEX_TRYLOCK syscall");
        return;
    }

    if (register_syscall(Mutex_Unlock, SCALL_MUTEX_UNLOCK) < 0){
        printf("Error registering the SCALL_MUTEX_UNLOCK syscall");
        return;
    }
    
	printf("\nSyscalls Mutex implemented.");
	
}




