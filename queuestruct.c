#include <stdio.h>
#include <pthread.h>
#include "queuestruct.h"

int DEBUG=1;

void queue_initialize(prod_cons_queue *q)
{
	printf("queue inializer\n"); 
 
    // initialize the queue 
	for (int i=0; i<20; i++)
	{		
        if(DEBUG==0)
        {
            printf("for loop(queue inialize): %i \n", i);
        }

        q->element[i] = NULL;
	}
    
	if(DEBUG==0)
	{
    printf("exited for loop\n");
    }
    
    // initialize head and tail
	q->head = NULL;
	q->tail = NULL;
        
    if(DEBUG==0)
	{
		printf("initialize head and tail\n");
    }

    // initialize remaining_elements
	q->remaining_elements=MAX_QUEUE_SIZE;
	q->current=20;
	q->wait=0;

	pthread_cond_init(&q->cond, NULL); 
}



void queue_add(prod_cons_queue *q, int element)
{
    //need to check if something is waiting in here.
    // make the head position the end of the queue
	q->head=19;
    if(q->current>20)
	{
		q->current=0;
	}
    // make the tail position the index of the remaining element
    q->tail=(q->current)-1;
	if(q->tail>19)
	{
		q->tail=0;
	}

	/*DEBUG*/
    printf("curent: %i\ttail:%i\telem:%i\n", q->current,q->tail,q->element[q->tail]);
    
    // check if the tail is within the array 
	if((q->element[q->head])==NULL || (q->element[q->head])==-3)
	{
        // put the id into that position
		q->element[q->tail]= element;
        
        // make the remaining element the tail
		q->remaining_elements=q->remaining_elements-1;
		q->current=q->current-1;
		if(q->remaining_elements>20)
		{
			q->remaining_elements=0;
		}
	}
	else
	{
		if(DEBUG==1)
        {
            printf("have reached the head of the queue (queue_add in queuestruct)\n");
        }

		q->wait=1;
    } 
}

int queue_remove(prod_cons_queue *q)
{
    if(DEBUG==1)
    {
        printf("head: %i\ttail: %i\tremain: %i\telem: %i\n", q->head, q->tail, q->remaining_elements, q->element[q->head]);
    }
    
	if((q->element[q->head])!=NULL || (q->element[q->head])!=-3)
	{		
        int value = q->element[q->head];
        q->element[q->head]=-3;
        q->head=q->head - 1;
		q->remaining_elements=q->remaining_elements+1;
		q->current=q->head+2;
		q->wait=0;
		//somehow we have to prioritize certian threads.
		if(q->remaining_elements>20)
		{
			q->remaining_elements=20;	
		}
		if(q->current>20)
		{
			q->current=20;
		}
		if (q->tail==0)
		{
			q->tail=19;
		}
		else
		{
			q->tail=q->tail-1;
		}
		
        return value;
	}
	else
	{
		printf("there isn't anything in the end of the array\n");
		
        return 0;
	}
}
