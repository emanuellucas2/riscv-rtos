#ifndef _HAL_H_
#define _HAL_H_

#include <proc.h>
#include <types.h>

void hyper_init();
uint32_t getSyscallCode();
void ei();
void di();
void syscalls();


#endif /* _HAL_H_ */
