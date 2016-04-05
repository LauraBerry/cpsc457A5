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
        q->element[i] = 0;
	}
    
    // initialize head and tail
	q->head = NULL;
	q->tail = NULL;

    // initialize remaining_elements
	q->remaining_elements=MAX_QUEUE_SIZE;
	q->current=20;
	q->wait=0;
	pthread_cond_init(&q->cond, NULL); 
}

int checkerGreaterThan (int a, int b)
{
	if((int)a>b)
	{
		a=b;
	}
	return a;
}
int checker(int a, int b, int c)
{
	if((int)a>b||(int)a<c)
	{
		a=c;
	}
	return a;
}

int checkerZero(int a, int b)
{
	if((int)a>b)
	{
		return 0;
	}
	else
	{
		return a;
	}
}

void queue_add(prod_cons_queue *q, int element)
{
    // make the head position the end of the queue
	q->head=19;
    
	q->current=checker(q->current, 20, 0);

    // make the tail position the index of the remaining element
    q->tail = (q->current)-1;
    
	q->tail = checkerZero(q->tail,19);

	if((int)q->element[q->tail]<1 ||(int)q->element[q->tail]>10)
	{
		q->element[q->tail]=element;
	}
    
	/*DEBUG*/
    printf("curent: %i\ttail:%i\telem:%i\n", q->current,q->tail,q->element[q->tail]);
	int added=0;
        
	int i=19;
        
	while(added==0)
	{
		if(i<0||i>(int)q->tail)
		{
			break;
		}
		if((int)(q->element[i])==0)
		{
			// printf("added something\n");
                
            // put the id into that position
			q->element[i]= element;
				
            //decrement remaining_elements and current
			q->remaining_elements--;
			q->current--;

            //ensure reamining elemenst is not <0
			q->remaining_elements=checkerZero(q->remaining_elements,20);
			added=1;
		}
		i--;
	}
	q->current=q->tail+1;		
	if(added==0)
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
	if((int)(q->element[q->head])!=0)
	{		
        q->element[q->head]=0;
        q->head--;
		q->remaining_elements++;
		q->current=q->head+2;
		//somehow we have to prioritize certian threads.
		q->remaining_elements=checkerGreaterThan(q->remaining_elements,20);
		q->current=checkerGreaterThan(q->current, 20);
		
		if ((int)q->tail==0)
		{
			q->tail=19;
		}
		else
		{
			q->tail--;
		}
		q->tail=checkerGreaterThan(q->tail, 19);
		q->head=checkerGreaterThan(q->head,19);
	
		if(DEBUG==1)
		{
		    printf("head: %i\ttail: %i\tremain: %i\telem: %i\n", q->head, q->tail, q->remaining_elements, q->element[q->head]);
		}
    	q->wait=0;
        return q->element[q->head];
	}
	else
	{
		printf("there isn't anything in the end of the array\n");
		
        return 0;
	}
}
