/**
 * @file timer.c
 * 
 * @section DESCRIPTION
 * 
 * Timer interrupt subsystem. This timer is used for VCPU scheduling and  
 * virtual timer interrupt injection on Guests. 
 * 
 * Every guest receive timer interrupt each 1ms. This will be replaced soon with 
 * a configurable guest timer. 
 */

#include <libc.h>
#include <proc.h>
#include <timer.h>
#include <queue.h>
#include <task.h>
#include <scheduler.h>

/**
 * @brief Time interrupt handler.
 * 
 * Perfoms VCPUs scheduling and virtual timer interrupt injection on guests. 
 */
void timer_interrupt_handler(){

	MTIMECMP = MTIME + SYSTEM_TICK_INTERVAL;

	run_scheduler();

}

/**
 * @brief Configures the CP0 timer.
 * 
 * This function will never return to the calling code. It waits for the
 * first timer interrupt.
 */
void start_timer(){
	
	MTIMECMP = MTIME + SYSTEM_TICK_INTERVAL;

    set_csr_bits(mie, MIP_MTIP);
    set_csr_bits(mstatus, MSTATUS_MIE);

	/* Wait for a timer interrupt */
	for(;;);
}

