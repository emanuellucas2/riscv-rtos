#ifndef _SHAREDMEM_H
#define _SHAREDMEM_H

#define SHARED_MEMORY_SIZE 1024

typedef struct {
    uint8_t data[SHARED_MEMORY_SIZE];
    uint32_t write_index;
    uint32_t read_index;
    uint8_t is_full;
} SharedMemory_t;

#endif 