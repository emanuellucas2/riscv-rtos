#include <libc.h>
#include <syscalls.h>
#include <mutex.h>
#include <shared_mem.h>

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
	uint32_t* p = 0x80000000;

	while(1){

		mdelay(1000);

		printf("Task 2(%d)\n",i);

		printf("Invasao(%x)\n",*p);
	}
	
}