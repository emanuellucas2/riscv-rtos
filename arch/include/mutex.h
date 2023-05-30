#ifndef _MUTEX_H
#define _MUTEX_H

typedef struct {
    uint32_t locked;
    uint32_t num_waiting;
    uint32_t waiting_tasks[MAX_TASKS];
} Mutex_t;

static void Mutex_Init();
static void Mutex_Lock();
static void Mutex_TryLock();
static void Mutex_Unock();
void mutex_init();

#endif 