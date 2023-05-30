#include <scheduler.h>
#include <queue.h>
#include <libc.h>
#include <proc.h>
#include <task.h>
#include <timer.h>

#define TICKS_BEFORE_SCHEDULING ( QUANTUM_SCHEDULER_MS / (SYSTEM_TICK_US/1000))

struct scheduler_info_t scheduler_info = {NULL, NULL};

/** 
 * @brief Tick counter. Used for VM's scheduling. 
 */ 
static uint32_t tick_count = 0;

/** 
 * @brief Critical event pending. 
 */ 
static uint32_t pending = 0;


/**
 * @brief priority round-robin scheduler implementation. 
 * 
 * @return Pointer to the task in the queue. 
 */
static struct task_entry* round_robin_scheduler(){
	uint32_t i;
	int32_t k;
	uint32_t highestp = 255;
	struct task_entry * task = NULL;
	struct task_entry * task_aux = NULL;
     
	k = queue_count(scheduler_info.krnl_queue);
         
	/* search for the highest priority task */
	for (i = 0; i < k; i++){
		task = queue_remhead(scheduler_info.krnl_queue);
		if (!task){
			goto error1;
		}
		if (queue_addtail(scheduler_info.krnl_queue, task)){
			goto error2;
		}
		/* high priority task. Bypass the queue */
		if (task->critical && task->state == TASK_RUNNING){
			task->critical = 0;
			task->bgjobs++;
			pending -= 1;
			return task;
		}
		if (highestp > task->priority_rem && task->state == TASK_RUNNING){
			highestp = task->priority_rem;
			task_aux = task;
		}
	}
     
	if (task_aux){
		/* update priorities of all tasks */
		for (i = 0; i < k; i++){
			task = queue_remhead(scheduler_info.krnl_queue);
			if (!task){
				goto error1;
			}
			if (queue_addtail(scheduler_info.krnl_queue, task)){
				goto error2;
			}
			if (task != task_aux  && task->state == TASK_RUNNING){
				task->priority_rem -= task_aux->priority_rem;
			}
		}
	
		task = task_aux;
		task->priority_rem = task->priority;
		task->bgjobs++;
		return task;
	}
	return NULL;
	
error1:
	printf("Error removing Task to head.");
	
error2:
	printf("Error adding Task to tail.");

	return NULL;
}


/**
 * @brief Performs a context-switching to a target Task. This is called 
 * from device drivers interrupt context and it allows for a 
 * task to attend to a interrupt faster. 
 * 
 * @param target Target Task. 
 * 
 */
void fast_interrupt_delivery(struct task_entry *target){
	if (task_in_execution != target){
		if (!target->critical){
			target->critical = 1;
			pending += 1;
		}
	}
}

/**
 * @brief Scheduler routine. Must be invoked in the timer interrupt routine. 
 * 
 */
void run_scheduler(){
	struct task_entry *aux;
	if ( tick_count % TICKS_BEFORE_SCHEDULING == 0 || /* its time to preempt the Task in execution. */
		pending || /* Critical event is pending. */
		(task_in_execution && task_in_execution->state == TASK_BLOCKED)) /* The current Task was blocked, perform a context-switching. */
	{		
		aux = round_robin_scheduler();
		if (aux != scheduler_info.task_executing || aux == NULL) { 
			contextSave(); 
			scheduler_info.task_executing = aux;
			contextRestore();
		}
		
	}
	tick_count++;
}

/**
 * @brief Returns a Task corresponding to the id.
 * @param id Task id number.
 * @return Pointer to the Task. 
 */
struct task_entry* get_task_from_id(uint32_t id){
	uint32_t i;
	struct task_entry* task;
	
	for(i=0; i < queue_count(scheduler_info.krnl_queue); i++){
		task = queue_get(scheduler_info.krnl_queue, i);
		if (task->id == id){
			return task;
		}
	}
	
	return NULL;
} 

void add_krnl_queue(struct task_entry* krnl_task){

	if(!scheduler_info.krnl_queue)
		scheduler_info.krnl_queue = queue_create(MAX_TASKS);

	queue_addtail(scheduler_info.krnl_queue,krnl_task);

}

