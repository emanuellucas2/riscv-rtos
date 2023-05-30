#ifndef _SYSCALL_DEFINES_H_
#define _SYSCALL_DEFINES_H_

typedef void syscall_t();

#define SCALL_TABLE_SIZE 30
#define SCALL_CODE_INVALID       	-1
#define SCALL_CODE_USED          	-2
#define SCALL_NOT_IMPLEMENTED    	-3
#define SCALL_ADDRESS_NOT_ALLOWED 	-4
#define SCALL_USB_BUSY 			-5

/* GET MTIMER VALUE 
 * v0 = mtimer register value.
 */
#define SCALL_GET_MTIMER_VALUE 0

/* SEND MESSAGE 
 *   a0 = Mutex pointer
 */
#define SCALL_MUTEX_INIT  1

/* SEND MESSAGE 
 *   a0 = Mutex pointer
 */
#define SCALL_MUTEX_LOCK  2

/* SEND MESSAGE 
 *   a0 = Mutex pointer
 */
#define SCALL_MUTEX_TRYLOCK  3

/* SEND MESSAGE 
 *   a0 = Mutex pointer
 */
#define SCALL_MUTEX_UNLOCK  4

/* SEND MESSAGE 
 *   a0 = Mutex pointer
 */
#define SCALL_SEMAPHORE_INIT  5

/* SEND MESSAGE 
 *   a0 = Mutex pointer
 */
#define SCALL_SEMAPHORE_WAIT  6

/* SEND MESSAGE 
 *   a0 = Mutex pointer
 */
#define SCALL_SEMAPHORE_TRYWAIT  7

/* SEND MESSAGE 
 *   a0 = Mutex pointer
 */
#define SCALL_SEMAPHORE_POST  8

#define SCALL_SHARED_MEM_INIT  9

/* SEND MESSAGE 
 *   a0 = Mutex pointer
 */
#define SCALL_SHARED_MEM_WRITE  10

/* SEND MESSAGE 
 *   a0 = Mutex pointer
 */
#define SCALL_SHARED_MEM_READ  11

#endif

