#ifndef _GRP_CONTEXT_H
#define _GRP_CONTEXT_H

#include <types.h>

#define GPR_SIZE 128
/** 
  * These functions can read/write the saved registers from the stack.  
*/
void print_stack();
void MoveToPreviousGuestGPR(uint32_t reg, uint32_t value);
uint32_t MoveFromPreviousGuestGPR(uint32_t reg);
void gpr_context_restore(uint32_t* gpr_p);
void gpr_context_save(uint32_t* gpr_p);


#endif /* _GRP_CONTEXT_H */
