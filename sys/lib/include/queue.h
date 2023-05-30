#ifndef _QUEUE_H
#define _QUEUE_H

#include <types.h>
/**
 * @brief Queue data structure.
 */
struct queue_t {
    int32_t size;                   /*!< queue size (maximum number of elements) */
    int32_t elem;                   /*!< number of elements queued */
    int32_t head;                   /*!< first element of the queue */
    int32_t tail;                   /*!< last element of the queue */
    void **data;                    /*!< pointer to an array of pointers to node data */
};
 
struct queue_t *queue_create(int32_t size);
int32_t queue_destroy(struct queue_t *q);
int32_t queue_count(struct queue_t *q);
int32_t queue_addtail(struct queue_t *q, void *ptr);
void *queue_remhead(struct queue_t *q);
void *queue_remtail(struct queue_t *q);
void *queue_get(struct queue_t *q, int32_t elem);
int32_t queue_set(struct queue_t *q, int32_t elem, void *ptr);
int32_t queue_swap(struct queue_t *q, int32_t elem1, int32_t elem2);

#endif