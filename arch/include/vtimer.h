#ifndef _VTIMER_H
#define _VTIMER_H

#include <hal.h>
#include <libc.h>
#include <proc.h>
#include <syscall_defines.h>
#include <syscall.h>

static void get_mtimer();
void vtimer_init();

#endif