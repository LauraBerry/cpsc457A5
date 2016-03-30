#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include "queuestruct.c"



typedef struct
{
	prod_cons_queue* queue;
	int* id;
}holder;

void* messageForProducerThread(void* args)
{
	printf("producer thread inializer\n");
	holder* queueAndId = (holder*) args;
	prod_cons_queue* maggie=queueAndId->queue;
	int Identify =*queueAndId->id;
	if(DEBUG==1)
	{
		printf("variables initalized\n");
	}

	for (int i=0; i<10; i++)
	{
			if(DEBUG==0)
			{
				printf("inside for loop (producer thread) iteration: %i\n", i);
			}
		
			queue_add(maggie, Identify);
	}
	if (DEBUG==0)
	{
		printf("id: %i\n", Identify);
	}
}

void* consumerInit()
{
	printf("consumer thread inializer\n");
	//TODO
	//needs to get the producer threads id somehow.
	//printf(contents of message);
}


int main()
{
	printf("Program Start\n");
	prod_cons_queue* queue;
	queue_initialize(queue);
	if(DEBUG==1)
	{	
		printf("left initalize\n");
	}
	pthread_t producerThreads[10];
	pthread_t consumerThread[1];
	
	if(DEBUG==0)
	{	
		printf("threads created\n");
	}
	holder queueHolder;
	queueHolder.queue=&queue;

	if(DEBUG==1)
	{		
		printf("queueHolder created\n");
	}
	void* result;
	if(DEBUG==0)
	{		
		printf("result created\n");
	}	
	for (int k=0; k<10; k++)
	{
		if(DEBUG==0)
		{		
			printf("enter for loop (main)\n");
		}
		int* var= k+1;
		queueHolder.id=&var;

		printf("%i: ", (k+1));
		pthread_create(&producerThreads[k], NULL, messageForProducerThread, &queueHolder);
		pthread_join(producerThreads[k], (void**)&result);
		if(DEBUG==0)
		{ 
			printf("end of for loop (main)\n");
		}
	}
	/*for(int k=0; k<10; k++)
	{
		if(DEBUG==1)
		{
			printf("join for loop(main)\n");
		}
		
		if(DEBUG==1)
		{
			printf("end of join for loop (main) iteration %i\n", (k+1));
		}
	}*/		
	printf("\n");
	pthread_create(&consumerThread[0], NULL, consumerInit, &queue);
	pthread_join(consumerThread[0], (void**)&result);

	return 0;
}

