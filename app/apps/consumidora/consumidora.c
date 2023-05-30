#include <libc.h>
#include <syscalls.h>
#include <mutex.h>
#include <shared_mem.h>

#define TASK_ID_CONSUMIDORA 1
#define TASK_ID_PRODUTORA 0

uint32_t wait_time(uint32_t old_time, uint32_t ms_delay){
	uint32_t diff_time;
	uint32_t now = get_mtimer_value();
    
	if (now >= old_time)
		diff_time = now - old_time;
	else
		diff_time = 0xffffffffffffffff - (old_time - now);

	if(diff_time > (ms_delay * MILISECOND)){
		return 1;
	}
	
	return 0;
}


void mdelay(uint32_t msec){
 	uint32_t now = get_mtimer_value();
    
    while(!wait_time(now, msec));       
  
}
int main(){

	uint32_t i = 0;

	shared_mem_init();
	mutex_init();
	mutex_lock(TASK_ID_PRODUTORA);

	while(1){

	   	mutex_lock(TASK_ID_CONSUMIDORA);
    
	   	mdelay(1000);
	   	shared_mem_write(TASK_ID_CONSUMIDORA,(uint32_t)i++);

		  mutex_unlock(TASK_ID_PRODUTORA);

        }
	
}