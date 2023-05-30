#ifndef _TASK_H_
#define _TASK_H_

#include <queue.h>
#include <hal.h>

#define STACK_MAGIC			0xb00bb00b

#define ERROR -1

#define MAX_TASKS 10

#define MPP_MASK 0xFFFFE7FF

#define VM_NAME_SZ 16

/* task status definitions */
#define TASK_IDLE			0		/*!< task does not exist / not ready */
#define TASK_READY			1		/*!< task ready to run (on run queue) */
#define TASK_RUNNING			2		/*!< task running (only one task/core can be in this state, on run queue) */
#define TASK_BLOCKED			3		/*!< task blocked, can be resumed later (on run queue) */
#define TASK_DELAYED			4		/*!< task being delayed (on delay queue) */
#define TASK_WAITING			5		/*!< task waiting for an event (on event queue) */

struct task_entry{
	uint16_t id;					/*!< task id */
	int8_t name[20];				/*!< task description (or name) */
	uint8_t state;					/*!< 0 - idle,  1 - ready,  2 - running, 3 - blocked, 4 - delayed, 5 - waiting */
	uint8_t priority;				/*!< [1 .. 29] - critical, [30 .. 99] - system, [100 .. 255] - application */
	uint8_t priority_rem;				/*!< remaining priority */
	uint8_t critical;				/*!< critical event, interrupt request */
	uint8_t init;
	uint32_t delay;					/*!< delay to enter in the run/RT queue */
	uint32_t rtjobs;				/*!< total RT task jobs executed */
	uint32_t bgjobs;				/*!< total BE task jobs executed */
	uint32_t deadline_misses;			/*!< task realtime deadline misses */
	uint16_t period;				/*!< task period */
	uint16_t capacity;				/*!< task capacity */
	uint16_t deadline;				/*!< task deadline */
	uint16_t period_rem;				/*!< remaining period counter*/
	uint16_t capacity_rem;				/*!< remaining capacity on period */
	uint16_t deadline_rem;				/*!< countdown to deadline */
	uint32_t cp0_registers[3];
	uint32_t pc;
	uint32_t task_sp;
	uint32_t task_tp;
	void (*ptask)(void);				/*!< task entry point, pointer to function */
	size_t *pstack;					/*!< task stack area (bottom) */
	uint32_t stack_size;				/*!< task stack size */
	uint32_t *gpr;                   /*!< pointer to the address where gpr will be saved*/
	uint32_t *shared_mem;
	uint32_t *mutex;
	void *other_data;				/*!< pointer to other data related to this task */
};

struct tasksconf_t{
    char app_name[VM_NAME_SZ];
    uint32_t priority;
    uint32_t ram_base;
    uint32_t size;
};    


struct task_entry vector_tasks[MAX_TASKS];
struct task_entry *krnl_task;
uint32_t quantity_tasks;

int32_t create_task(void (*task)(), uint16_t period, uint16_t capacity, uint16_t deadline, int8_t *name, uint32_t stack_size);
void reset_tasks();
void initialize_tasks();
void contextRestore();
void contextSave();
void setSP(uint32_t pstack);
void config_idle_cpu();
void setEPC(uint32_t epc);


#endif