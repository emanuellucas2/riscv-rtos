#include <hal.h>
#include <libc.h>
#include <proc.h>
#include <syscall_defines.h>
#include <syscall.h>
#include <task.h>
#include <semaphore.h>
#include <scheduler.h>


static void Semaphore_Init() {

    Semaphore_t* sem   = (Semaphore_t*)MoveFromPreviousGuestGPR(REG_A0);
    int32_t init_count = (int32_t)MoveFromPreviousGuestGPR(REG_A1);
 
    sem->count = init_count;
    sem->num_waiting = 0;

}


static void Semaphore_Wait(Semaphore_t* sem) {

    sem->count--;

    if (sem->count < 0) {
        // Add the current task to the waiting list
          sem->waiting_tasks[sem->num_waiting] = task_in_execution->id;
        sem->num_waiting++;
        // Put the task to sleep until the semaphore is available
        task_in_execution->state = TASK_BLOCKED;
    }

}

static void Semaphore_TryWait(Semaphore_t* sem) {

    if (sem->count <= 0) {
        // The semaphore is not available, so return immediately

         MoveFromPreviousGuestGPR(REG_A0,0);
    } else {
        // The semaphore is available, so decrement the count

       MoveFromPreviousGuestGPR(REG_A0,1);
    }
}

static void Semaphore_Post() {

    Semaphore_t* sem = (Semaphore_t*)MoveFromPreviousGuestGPR(REG_A0);

    sem->count++;

    if (sem->num_waiting > 0) {

        // Remove the task from the waiting list
        sem->num_waiting--;
        for (uint8_t i = 0; i < sem->num_waiting; i++) {
            sem->waiting_tasks[i] = sem->waiting_tasks[i+1];
        }
        // Wake up the task
        task_in_execution->state = TASK_WAITING;
    }

}

/**
 * @brief Driver init call.  Registers the hypercalls. 
 */
void semaphore_init(){
    if (register_syscall(Semaphore_Init, SCALL_SEMAPHORE_INIT) < 0){
        printf("Error registering the SCALL_SEMAPHORE_INIT syscall");
        return;
    }

    if (register_syscall(Semaphore_Wait, SCALL_SEMAPHORE_WAIT) < 0){
        printf("Error registering the SCALL_SEMAPHORE_WAIT syscall");
        return;
    }

    if (register_syscall(Semaphore_TryWait, SCALL_SEMAPHORE_TRYWAIT) < 0){
        printf("Error registering the SCALL_SEMAPHORE_TRYWAIT syscall");
        return;
    }

    if (register_syscall(Semaphore_Post, SCALL_SEMAPHORE_POST) < 0){
        printf("Error registering the SCALL_SEMAPHORE_POST syscall");
        return;
    }
    
    printf("\nSyscalls Semaphore implemented.");
    
}