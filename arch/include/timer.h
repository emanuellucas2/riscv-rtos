#ifndef _TIMER_H
#define _TIMER_H

#define MILISECOND ((CPU_FREQ/2)/ 1000)
#define MICROSECOND (MILISECOND/1000)
#define SYSTEM_TICK_US 1000
#define SYSTEM_TICK_INTERVAL (SYSTEM_TICK_US * MICROSECOND)
#define QUEST_TICK_INTERVAL (GUEST_QUANTUM_MS * MILISECOND)

#define GUEST_TIMER_INT			1

void timer_interrupt_handler();
void start_timer();

#endif 