#include <libc.h>
#include <task.h>
#include <queue.h>
#include <malloc.h>
#include <proc.h>
#include <scheduler.h>
#include <config.h>
#include <gpr_context.h>
#include <mutex.h>

int32_t create_task(void (*task)(), uint16_t period, uint16_t capacity, uint16_t deadline, int8_t *name, uint32_t stack_size){

	int32_t i=0;

	if((deadline<capacity) || (period<capacity))
		return ERROR;

	while ((vector_tasks[i].ptask != 0) && (i < MAX_TASKS))
		i++;

	if(i==MAX_TASKS)
		return ERROR;

	di();

	quantity_tasks++;
	krnl_task = &vector_tasks[i];
	krnl_task->id = i;
	krnl_task->state = TASK_RUNNING;
	krnl_task->priority = 100;
	krnl_task->priority_rem = 100;
	krnl_task->delay = 0;
	krnl_task->period = period;
	krnl_task->capacity = capacity;
	krnl_task->deadline = deadline;
	krnl_task->period_rem = period;
	krnl_task->capacity_rem = capacity;
	krnl_task->deadline_rem = deadline;
	krnl_task->rtjobs = 0;
	krnl_task->bgjobs = 0;
	krnl_task->deadline_misses = 0;
	krnl_task->ptask = task;
	krnl_task->pc = (uint32_t)task;
	stack_size += 3;
	stack_size >>= 2;
	stack_size <<= 2;
	krnl_task->stack_size = stack_size;
	krnl_task->pstack = (size_t *)malloc(stack_size);
	krnl_task->task_sp = (size_t)krnl_task->pstack + (stack_size - 4);
	krnl_task->task_tp = (size_t)krnl_task->ptask;
	krnl_task->gpr = malloc(GPR_SIZE);
	krnl_task->shared_mem = 0;
	krnl_task->mutex = malloc(sizeof(Mutex_t));

	if(krnl_task->pstack){

		krnl_task->pstack[0] = STACK_MAGIC;
		printf("KERNEL: [%s], id: %d, p:%d, c:%d, d:%d, addr: %x, sp: %x, ss: %d bytes\n", krnl_task->name, krnl_task->id, krnl_task->period, krnl_task->capacity, krnl_task->deadline, krnl_task->ptask, krnl_task->task_sp, stack_size);
		add_krnl_queue(krnl_task);

	}else{

		krnl_task->ptask = 0;
		quantity_tasks--;
		printf("KERNEL: task not added (out of memory)\n");
		i = ERROR;

	}

	ei();

	return i;
}

void reset_tasks(){

	uint32_t i;

	for(i = 0; i < MAX_TASKS; i++){
		krnl_task = &vector_tasks[i];
		krnl_task->id = -1;
		memset(krnl_task->name, 0, sizeof(krnl_task->name));
		krnl_task->state = TASK_IDLE;
		krnl_task->priority = 0;
		krnl_task->priority_rem = 0;
		krnl_task->delay = 0;
		krnl_task->init = 1;
		krnl_task->rtjobs = 0;
		krnl_task->bgjobs = 0;
		krnl_task->deadline_misses = 0;
		krnl_task->period = 0;
		krnl_task->capacity = 0;
		krnl_task->deadline = 0;
		krnl_task->period_rem = 0;
		krnl_task->capacity_rem = 0;
		krnl_task->deadline_rem = 0;
		krnl_task->pc = 0;
		krnl_task->cp0_registers[0] = 0;
		krnl_task->cp0_registers[1] = 0;
		krnl_task->cp0_registers[2] = 0;
		krnl_task->task_sp = 0;
		krnl_task->task_tp = 0;
		krnl_task->ptask = NULL;
		krnl_task->pstack = NULL;
		krnl_task->stack_size = 0;
		krnl_task->other_data = 0;
		krnl_task->gpr = NULL;
		krnl_task->shared_mem = NULL;
		krnl_task->mutex = NULL;
	}

	quantity_tasks = 0;
	
}

/**
 * @brief Initialize all tasks.
 */
void initialize_tasks(){
	int i;

	for(i = 0; i < NVMACHINES; i++){

		create_task((VMCONF[i].ram_base)<<12, 4, 1, 4, VMCONF[i].app_name, 256);

	}

}

/**
 * @brief Save the Task context.
 */
void contextSave(){
	struct task_entry *tasktosave;

	tasktosave = task_in_execution;
	
	if (!is_task_executing){
		return;
	}

	gpr_context_save(tasktosave->gpr);

	tasktosave->cp0_registers[0] = read_csr(mstatus);
	tasktosave->cp0_registers[1] = read_csr(mie);
	tasktosave->cp0_registers[2] = read_csr(mip);
	tasktosave->pc = read_csr(mepc);
	tasktosave->task_sp = readSP();
}
/**
 * @brief  Restore the Task context on context switch. 
 */
void contextRestore(){

	struct task_entry *task = task_in_execution;

	/* There are not Tasks ready to execute. Put CPU in adle mode. */
	if(!task){
		config_idle_cpu();
		return;
	}
	
	if(task->init){
		task->init = 0;

		setSP((uint32_t)task->task_sp);
	}else{
		write_csr(mstatus, task->cp0_registers[0]);
		write_csr(mie, task->cp0_registers[1]);
		write_csr(mip, task->cp0_registers[2]);

		setSP((uint32_t)task->task_sp);
		gpr_context_restore(task->gpr);
	}

	/*PMP configuration*/

	/*write PMP addresses*/
	write_csr(pmpaddr0,0x80000000>>2);
	write_csr(pmpaddr1,((uint32_t)(task->pstack))>>2);
	write_csr(pmpaddr2,((uint32_t)((task->pstack)+(task->stack_size)))>>2);
	write_csr(pmpaddr3,((uint32_t)(VMCONF[task->id].ram_base))<<10);
	write_csr(pmpaddr4,((uint32_t)((VMCONF[task->id].ram_base)+(VMCONF[task->id].size)))<<10);
	write_csr(pmpaddr5,0x3fffffff);


	/*config PMP*/
	write_csr(pmpcfg0,0x80f080f);
	write_csr(pmpcfg1,0x80f);

	write_csr(mstatus, (read_csr(mstatus) & MPP_UMODE_MASK));

	setEPC(task->pc);

}

void setSP(uint32_t pstack){

	MoveToPreviousGuestGPR(REG_SP,pstack);

}

int readSP(){

	return MoveFromPreviousGuestGPR(REG_SP);

}

void config_idle_cpu(){

	while(1){

	};
}

void setEPC(uint32_t epc){

	write_csr(mepc, epc);
}
