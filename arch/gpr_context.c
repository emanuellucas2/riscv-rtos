#include <gpr_context.h>
#include <libc.h>
#include <task.h>
#include <proc.h>

/** 
 * @brief Stack pointer defined at linker script. 
 */
extern uint32_t _stack;

void print_stack(){
	uint32_t i, j=0;

	printf("\n==========================");
	for(i=((uint32_t)(&_stack))-GPR_SIZE;  i<((uint32_t)(&_stack)); i+=sizeof(uint32_t)){
		printf("\n%d:0x%x",j++, *(uint32_t*)i);
	}
	printf("\n==========================");
}

/** 
 * @brief Move data to the previous guest GPR on stack.
 * @param reg GPR number.
 * @param value Value to write on the stack.
 */
void MoveToPreviousGuestGPR(uint32_t reg, uint32_t value){
	uint32_t* sp = (((uint32_t)(&_stack)) - GPR_SIZE);
	sp[reg] = value;
}


/** 
 * @brief Copy data from the previous guest GPR on stack.
 * @param reg GPR number.
 * @return GPR data.
 */
uint32_t MoveFromPreviousGuestGPR(uint32_t reg){
	uint32_t* sp = (((uint32_t)(&_stack)) - GPR_SIZE);
	return sp[reg];
}

/** 
 * @brief Copy the GPR from task to the stack. 
 * @param grp_p Pointer to the address where the gpr is saved. 
 */
void gpr_context_restore(uint32_t* gpr_p){
	uint32_t i, j;
	for(i=((uint32_t)(&_stack))-GPR_SIZE, j=0; i<((uint32_t)(&_stack)); i+=sizeof(uint32_t), j++){
		*(uint32_t*)i = gpr_p[j]; 
	}
}


/** 
 * @brief Copy the GPR from stack to the task. 
 * @param grp_p Pointer to the address where gpr will be saved. 
 */
void gpr_context_save(uint32_t* gpr_p){
	uint32_t i, j;
	for(i=((uint32_t)(&_stack))-GPR_SIZE, j=0; i<((uint32_t)(&_stack)); i+=sizeof(uint32_t), j++){
		gpr_p[j] = ((uint32_t)*(uint32_t*)i); 
	}
}

