#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <types.h>

void guest_exit_exception(uint32_t cause, uint32_t mepc);

#endif 