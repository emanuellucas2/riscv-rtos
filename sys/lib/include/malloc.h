#ifndef _MALLOC_H_
#define _MALLOC_H_

#include <types.h>

typedef int32_t word_align;

union header{	 /* block header */
	struct{
		union header *ptr;	/* mnext block if on free list */
		uint32_t size;	 /* size of this block */
	} s;
	word_align x;	 /* force block alignment */
};

typedef union header mem_header;

void HeapInit(void *heap, uint32_t len);
void *malloc(uint32_t size);
void free(void *ptr);
void *calloc(uint32_t qty, uint32_t type_size);
void *realloc(void *ptr, uint32_t size);
uint32_t init_mem();

#endif
