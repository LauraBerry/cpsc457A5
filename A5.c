#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include "queuestruct.c"


int interator=19;
int currThread;
typedef struct
{
    prod_cons_queue* queue;
    pthread_mutex_t* locker;
    int* id;
} producerStruct;

typedef struct
{
    prod_cons_queue* queue;
    pthread_mutex_t* locker;
} consumerStruct;

void* messageForProducerThread(void* args)
{
    printf("producer thread inializer\n");  
    // grab queue holder from args
    producerStruct* queueAndId = (producerStruct*) args;
    
    // grab queue
    prod_cons_queue* maggie = queueAndId->queue;

    // grab locker
    pthread_mutex_t* lock = queueAndId->locker;

        
    // grab id
    int Identify = queueAndId->id;
    
    if(DEBUG==0)
	{
		printf("variables initalized\n");
	} 
    for (int i=0; i<10; i++)
    {
        if(DEBUG==0 && currThread<3)
        {
            printf("inside for loop (producer thread) iteration: %i\n", i);
        }
        pthread_mutex_lock(&lock);
        queue_add(maggie, Identify);
        pthread_mutex_unlock(&lock);
		interator--;
    }
    /* DEBUG*/
   for(int i=0;i<20;i++)
    {
		if(currThread==3)
		{
        	printf("id: %i\t%i: %i\n", Identify, i, maggie->element[i]);
    	}
	} 
    
    if (DEBUG==0)
    {
        printf("id: %i\n", Identify);
    }
}

void* consumerInit(void* args)
{
    printf("consumer thread inializer\n");
    
    // grab consStruct
    consumerStruct* consStruct = (consumerStruct*) args;
    
    // grab queue
    prod_cons_queue* q = consStruct->queue;
    
    // grab locker
    pthread_mutex_t* lock = consStruct->locker;

    pthread_mutex_lock(&lock);
    int result = queue_remove(q);
    pthread_mutex_unlock(&lock);
    
    printf("result is: %i \n", result);
}


int main()
{
    printf("Program Start\n");

    // create locker
    pthread_mutex_t* lock = PTHREAD_MUTEX_INITIALIZER;

    // initialize threads
    pthread_t producerThreads[10];
    pthread_t consumerThread[1];
    
    if(DEBUG==1)
	{
		printf("threads created\n");
	}
    
    // initialize the queue
    prod_cons_queue* queue;
    queue_initialize(&queue);

    /*if(DEBUG==1)
	{
		printf("remaininelems: %i\n", queue->remaining_elements); 
    }*/

    // initialize the holder for producer
    producerStruct prodStruct;
    prodStruct.queue=&queue;
    prodStruct.locker=&lock;
    
    if(DEBUG==1)
	{
		printf("prodStruct created\n");
	}

    // initialize the holder for consumer
    consumerStruct consStruct;
    consStruct.queue=&queue;
    consStruct.locker=&lock;
    
    if(DEBUG==1)
	{
		printf("prodStruct created\n");
	}

    // result placeholder
    void* result;
    
    if(DEBUG==1)
	{
		printf("result created\n");
	}
    
    // loop through and create threads
    for (int k=0; k<10; k++)
    {
        if(DEBUG==1)
        {
            printf("enter for loop (main): %i\n", k);
        }
        
        // set the id
        int* id= k+1;
		currThread=k+1;        
		prodStruct.id = id;
        
        /// create the thread
        pthread_create(&producerThreads[k], NULL, messageForProducerThread, &prodStruct);
        /*if(currThread<3)
		{
			printf("what went in: %i  what came out: %i\n", id, queue->element[interator]);
		}*/
        // join the thread
        pthread_join(producerThreads[k], (void**)&result);
    }    
    printf("\n");
    
    // create and join the consumer thread
    pthread_create(&consumerThread[0], NULL, consumerInit, &consStruct);
    pthread_join(consumerThread[0], (void**)&result);

    return 0;
}

