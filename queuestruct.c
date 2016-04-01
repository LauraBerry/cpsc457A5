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
}



void queue_add(prod_cons_queue *q, int element)
{
    
    // make the head position the end of the queue
	q->head=19;
    
    // make the tail position the index of the remaining element
    q->tail=(q->remaining_elements)-1;
	if(q->tail>19)
	{
		q->tail=0;
	}
    

    
    // check if the tail is within the array 
	if((q->tail>=0)&&(q->tail<=19)&&((q->element[q->tail])<1 || (q->element[q->tail])>10))
	{
        // put the id into that position
		q->element[q->tail]= element;
        
        // make the remaining element the tail
		q->remaining_elements=q->remaining_elements-1;
		if(q->remaining_elements>=20)
		{
			q->remaining_elements=0;
		}
 
	}
           if(DEBUG==1)
    {
        printf("tail:%i\telem:%i\n", q->tail,q->element[q->tail]);
    }
	else
	{
		if(DEBUG==0)
        {
            printf("have reached the head of the queue (queue_add in queuestruct)\n");
        }
    } 
}

int queue_remove(prod_cons_queue *q)
{
    if(DEBUG==1)
    {
        printf("head: %i\ttail: %i\tremain: %i\telem: %i\n", q->head, q->tail, q->remaining_elements, q->element[q->head]);
    }
    
	if((q->element[q->head])!=NULL)
	{		
        int value = q->element[q->head];
        q->element[q->head]=-3;
        q->head=q->head - 1;
		q->remaining_elements=q->remaining_elements+1;

		if(q->remaining_elements>20)
		{
			q->remaining_elements=20;
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
