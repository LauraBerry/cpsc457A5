#include <stdio.h>
#include <pthread.h>
#include "queuestruct.h"

int DEBUG = 1;

void queue_initialize(prod_cons_queue *q)
{
	printf("queue inializer\n"); 
 
    // initialize the queue 
	for (int i=0; i<20; i++)
	{		
        q->element[i] = 0;
	}
    
    // initialize head and tail
	q->head = 0;
	q->tail = 0;
    
    // number of waiting producers
    q->wait = 0;

    // initialize remaining_elements
	q->remaining_elements = MAX_QUEUE_SIZE;
    
    // initialize conditions
    pthread_cond_init(&q->cond1, NULL);
    pthread_cond_init(&q->cond2, NULL);
}

// should not check if full
void queue_add(prod_cons_queue *q, int element)
{
	// make head the element
	q->element[q->head] = element;
	
	// increment the head circularly
	q->head = (q->head + 1) % MAX_QUEUE_SIZE;

	// decrease the remaining elements
    if(q->remaining_elements > 0)
    {
        q->remaining_elements--;
    }
    
	/* DEBUG */
	if(DEBUG==1) { printf("added %i\t(%i REMAINING, %i WAITING)\n", element, q->remaining_elements, q->wait); }
}

// should not check if empty
int queue_remove(prod_cons_queue *q)
{
    int data = 0;
    
	// set data to the tail
	data = q->element[q->tail];
	
	// reset the element
	q->element[q->tail] = 0;
	
	// increment the tail circularly
	q->tail = (q->tail + 1) % MAX_QUEUE_SIZE;
	
	// increment the remaining elements
	q->remaining_elements++;

    /* DEBUG */
	if(DEBUG==1) { printf("removed %i\t(%i REMAINING, %i WAITING)\n", data, q->remaining_elements, q->wait); }
    
	// return what is in data
    return data;
}
