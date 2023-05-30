#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

typedef struct {
    int32_t count;
    uint8_t num_waiting;
    uint8_t waiting_tasks[MAX_TASKS];
} Semaphore_t;

#endif 