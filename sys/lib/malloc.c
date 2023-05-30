#include "types.h"
#include "malloc.h"
#include "libc.h"

extern uint32_t _heap_start;
extern uint32_t _heap_size;


/* new memory allocator implementation
   based on K&R The C Programming Language book 2nd. ed., p.149
*/
 
static mem_header *freep = NULL;		/* start free list */
static mem_header *sfreep, sfree;		/* saved values to restore */

/**
 *  Initialize or reinitialize the heap 
 * 
 * @param heap heap pointer
 * @param len  heap size in bytes
 */
void HeapInit(void *heap, uint32_t len){
	mem_header *prevp, *p;
	uint32_t nunits;

	if ((prevp = freep) == NULL){
		nunits = ((len+sizeof(mem_header)-1)/sizeof(mem_header))+1; 
		p = heap;
		p->s.size = nunits - 1;
		p->s.ptr = p;
		freep = p;
		sfreep = p;
		sfree.s.ptr = p->s.ptr;
		sfree.s.size = p->s.size; 
	}else{
		freep = sfreep;
		freep->s.ptr = sfree.s.ptr;
		freep->s.size = sfree.s.size;           
	}
}

void *malloc(uint32_t size){
	mem_header *p, *prevp;
        uint32_t nunits;

	nunits = ((size+sizeof(mem_header)-1)/sizeof(mem_header))+1;
	if ((prevp = freep) == NULL)
		return NULL;

	for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
		if (p->s.size >= nunits){
			if (p->s.size == nunits){
				prevp->s.ptr = p->s.ptr;
			}else{
				p->s.size -= nunits;
				p += p->s.size;
				p->s.size = nunits;
			}
			freep = prevp;
			return (void *)(p+1);
		}
		if (p == freep){
			return NULL;
		}
	}
}

void free(void *ptr){
        mem_header *bp, *p;

	bp = (mem_header *) ptr - 1;
	for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
		if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
			break;

	if (bp + bp->s.size == p->s.ptr){
		bp->s.size += p->s.ptr->s.size;
		bp->s.ptr = p->s.ptr->s.ptr;
	}else{
		bp->s.ptr = p->s.ptr;
	}

	if (p + p->s.size == bp){
		p->s.size += bp->s.size;
		p->s.ptr = bp->s.ptr;
	}else{
		p->s.ptr = bp;
	}

	freep = p;
}

void *calloc(uint32_t qty, uint32_t type_size){
	void *buf;
	
	buf = (void *)malloc((qty * type_size));
	if (buf)
		memset(buf, 0, (qty * type_size));

	return (void *)buf;
}

void *realloc(void *ptr, uint32_t size){
	void *buf;

	if (ptr == NULL)
		return (void *)malloc(size);
	if (size == 0){
		free(ptr);
		return NULL;
	}

	buf = (void *)malloc(size);
	if (buf){
		memcpy(buf, ptr, size);
		free(ptr);
	}

	return (void *)buf;
}

uint32_t init_mem() {
  
	HeapInit((void*)(( (uint32_t)(&_heap_start) + 8) & 0xFFFFFFFC), ((uint32_t)(&_heap_size) -8) & 0xFFFFFFFC);  

	return 0;
}

