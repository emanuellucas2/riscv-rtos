#ifndef _MUTEX_H
#define _MUTEX_H

#define MAX_TASKS 10

typedef struct {
    uint8_t locked;
    uint8_t num_waiting;
    uint8_t waiting_tasks[MAX_TASKS];
} Mutex_t;

#endif 