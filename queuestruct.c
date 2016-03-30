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
		q->element[i]=NULL;
	}
	if(DEBUG==0)
	{
		printf("exited for loop");
	}
	q->head = NULL;
	q->tail = NULL;

	q->remaining_elements=MAX_QUEUE_SIZE;

}



void queue_add(prod_cons_queue *q, int element)
{
	q->head=20;
	q->tail=(q->remaining_elements)-1;
	printf("%i \n",q->tail);
	if((q->tail>=0)&&(q->tail<=20))
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
	if(q->element[q->head]!=NULL)
	{
		if(DEBUG==1)
		{
			printf("there is something in the end of the array\n");
		}
		int value = q->element[q->head];
		q->head=q->head - 1;
		return value;
	}
	else
	{
		printf("there isn't anything in the end of the array\n");
		return 0;
	}
}
