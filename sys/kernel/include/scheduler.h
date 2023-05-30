#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include <queue.h>
#include <task.h>

#define QUANTUM_SCHEDULER_MS 10

extern struct scheduler_info_t scheduler_info;

#define is_task_executing (((struct task_entry*)scheduler_info.task_executing) ? ((struct task_entry*)scheduler_info.task_executing) : NULL)
#define task_in_execution ((struct task_entry*)scheduler_info.task_executing)

struct scheduler_info_t{
	struct queue_t *krnl_queue;
	struct task_entry *task_executing;
};

void fast_interrupt_delivery(struct task_entry *target);
void run_scheduler();
struct task_entry* get_fast_int_vcpu_node(uint32_t fast_int);
struct task_entry* get_task_from_id(uint32_t id);
void add_krnl_queue(struct task_entry* krnl_task);
#endif /* __SCHEDULER_H */
