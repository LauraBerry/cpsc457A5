#include <stdio.h>
#include <pthread.h>
#include <stdint.h>


#define MAX_QUEUE_SIZE 20

	typedef struct queue
	{
		int element[MAX_QUEUE_SIZE];
		uint8_t head;
		uint8_t tail;
		uint8_t remaining_elements;
	}prod_cons_queue;

	void queue_initialize(prod_cons_queue *q)
	{
		printf("queue inializer\n");
		for (int i=0; i<MAX_QUEUE_SIZE; i++)
		{
			q->element[i]=0;
		}
		q->head=NULL;
		q->tail=NULL;
		q->remaining_elements=20;
	}
	
	void queue_add(prod_cons_queue *q);
	void* messageForProducerThread(void* args)
	{
		printf("producer thread inializer\n");
		int* identifier = (int*) args;
		//TODO
		/*for (int i=0; i<10; i++)
		{
			add message to thread
		}*/
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
		int identifier;
		pthread_t producerThreads[10];
		pthread_t consumerThread[1];
		int storage[10];
		for (int n=0; n<10; n++)
		{
			storage[n]=n+1;
		}
		for (int k=0; k<10; k++)
		{
			identifier=k+1;
			printf("%i: ", k);
			pthread_create(&producerThreads[k], NULL, messageForProducerThread, &storage[k]);
			pthread_join(producerThreads[k], (void**)&identifier);

		}
		printf("\n");
		pthread_create(&consumerThread[0], NULL, consumerInit, &consumerThread[0]);
		pthread_join(consumerThread[0], (void**)&identifier);

		return 0;
	}

