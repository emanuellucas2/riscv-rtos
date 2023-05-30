#ifndef  __SYSCALLS_H
#define  __SYSCALLS_H

#include <syscall_defines.h>

#define MILISECOND 10000

#define get_mtimer_value() ({ int32_t __ret; \
asm volatile (                    \
"li a7, %1 \n\
 ecall \n\
 move %0, a0 " \
 : "=r" (__ret) : "I" (SCALL_GET_MTIMER_VALUE) : "a0", "a7"); \
 __ret; })

#define mutex_init() ({ int32_t __ret; \
asm volatile (                    \
"li a7, %1 \n\
 ecall \n \
 move %0, a0 " \
 : "=r" (__ret) : "I" (SCALL_MUTEX_INIT) : "a0", "a7"); \
 __ret;  })

#define mutex_lock(mutex) ({ \
asm volatile (                    \
"move a0, %z0 \n\
 li a7, %1 \n\
 ecall \n" \
 : : "r" ((uint32_t) (mutex)), "I" (SCALL_MUTEX_LOCK) : "a0", "a7"); \
})

#define mutex_trylock(mutex) ({ int32_t __ret; \
asm volatile (                    \
"move a0, %z1 \n\
 li a7, %2 \n\
 ecall \n \
 move %0, a0 " \
 : "=r" (__ret) : "r" ((uint32_t) (mutex)), "I" (SCALL_MUTEX_TRYLOCK) : "a0", "a7"); \
 __ret; })

#define mutex_unlock(mutex) ({ \
asm volatile (                    \
"move a0, %z0 \n\
 li a7, %1 \n\
 ecall \n" \
 : : "r" ((uint32_t) (mutex)), "I" (SCALL_MUTEX_UNLOCK) : "a0", "a7"); \
})

#define semaphore_init(semaphore,init_count) ({ \
asm volatile (                    \
"move a0, %z0 \n\
 move a1, %z1 \n\
 li a7, %2 \n\
 ecall \n" \
 : : "r" ((uint32_t) (semaphore)),  "r" ((uint32_t) (init_count)), "I" (SCALL_SEMAPHORE_INIT) : "a0","a1","a7"); \
})

#define semaphore_wait(semaphore) ({ \
asm volatile (                    \
"move a0, %z0 \n\
 li a7, %1 \n\
 ecall \n" \
 : : "r" ((uint32_t) (semaphore)), "I" (SCALL_SEMAPHORE_LOCK) : "a0","a7"); \
})

#define semaphore_trywait(semaphore) ({ int32_t __ret; \
asm volatile (                    \
"move a0, %z1 \n\
 li a7, %2 \n\
 ecall \n \
 move %0, a0 " \
 : : "=r" (__ret) : "r" ((uint32_t) (semaphore)), "I" (SCALL_SEMAPHORE_TRYLOCK) : "a0", "a7"); \
 __ret; })

#define semaphore_post(semaphore) ({ \
asm volatile (                    \
"move a0, %z0 \n\
 li a7, %1 \n\
 ecall \n" \
 : : "r" ((uint32_t) (semaphore)), "I" (SCALL_SEMAPHORE_UNLOCK) : "a0", "a7"); \
})

#define shared_mem_init() ({ \
asm volatile (                    \
"li a7, %0 \n\
 ecall \n" \
 : : "I" (SCALL_SHARED_MEM_INIT) : "a7"); \
})


#define shared_mem_write(task,data) ({ int32_t __ret; \
asm volatile (                    \
"move a0, %z1 \n\
 move a1, %z2 \n\
 li a7, %3 \n\
 ecall \n \
 move %0, a0 " \
 : "=r" (__ret) : "r" ((uint32_t) (task)), "r" ((uint32_t) (data)), "I" (SCALL_SHARED_MEM_WRITE) : "a0", "a1", "a7"); \
 __ret; })

#define shared_mem_read(task) ({ int32_t __ret; \
asm volatile (                    \
"move a0, %z1 \n\
 li a7, %2 \n\
 ecall \n \
 move %0, a0 " \
 : "=r" (__ret) : "r" ((uint32_t) (task)), "I" (SCALL_SHARED_MEM_READ) : "a0", "a7"); \
 __ret; })


#endif