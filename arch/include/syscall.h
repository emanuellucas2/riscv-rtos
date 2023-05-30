
#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <types.h>
#include <syscall_defines.h>

int32_t register_syscall(syscall_t* sys, uint32_t code);
void syscall_execution();

#endif
