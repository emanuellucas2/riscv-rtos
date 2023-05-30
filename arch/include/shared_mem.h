#ifndef _SHAREDMEM_H
#define _SHAREDMEM_H

#define SHARED_MEMORY_SIZE 1024


static void SharedMemory_Init();
static void Semaphore_Wait();
static void Semaphore_TryWait();
static void Semaphore_Post();
void semaphore_init();

#endif 