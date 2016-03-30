#include <stdio.h>

#include "queuestruct.h"
int DEBUG=1;
void queue_initialize(prod_cons_queue *q)
{


	printf("queue inializer\n");

	for (int i=0; i<MAX_QUEUE_SIZE; i++)
	{
		if(DEBUG==0)
		{	
			printf("in for loop(queue inialize iteration: %i \n", (i+1));
		}
		q->element[i]=0;
	}
	if(DEBUG==0)
	{
		printf("exited for loop");
	}
	q->head = NULL;
	q->tail = NULL;
	if(DEBUG==0)
	{
		printf("head and tail initialized\n");	
	}
	q->remaining_elements=20;
	if(DEBUG==0)
	{
		printf("remaining_elements initalized\n");
	}
}



void queue_add(prod_cons_queue *q, int element)
{
	q->head=0;
	q->tail=(q->remaining_elements)-1;
	if(q->tail>=q->head)
	{
		q->element[q->tail]= element;
		q->remaining_elements=q->tail;
	}
	else
	{
		if(DEBUG==1)
		{
			printf("have reached the head of the queue (queue_add in queuestruct)\n");
		}
	}
}

int queue_remove(prod_cons_queue *q)
{
	return 0;
}
