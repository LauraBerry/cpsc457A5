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
        pthread_mutex_lock(&lock);
		if(maggie->wait==1)
		{
			printf("\nthread #: %i is waiting...\n",currThread);
		}
		while(maggie->wait==1)
		{
			pthread_cond_wait(&maggie->cond, &lock);
		}
        queue_add(maggie, Identify);
        pthread_mutex_unlock(&lock);
		interator--;
    }
    /* DEBUG*/
   for(int i=0;i<20;i++)
    {
		if(currThread<11)
		{
        	printf("id: %i\t%i: %i\n", Identify, i, maggie->element[i]);
    	}
	} 
    
    if (DEBUG==0)
    {
        printf("id: %i\n", Identify);
    }
	pthread_exit(NULL);
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
	if(q->wait!=0)
	{
		pthread_cond_signal(&q->cond);
	} 
   pthread_mutex_unlock(&lock);
    
    printf("result is: %i \n", result);
	printf("consumer thread has been called:\n");
	for(int i=0;i<20;i++)
    {
        	printf(" %i\n", q->element[i]);
    } 
    pthread_exit(NULL);
}


int main()
{
    printf("Program Start\n");


    pthread_mutex_t* lock = PTHREAD_MUTEX_INITIALIZER;
	
			//always lock and unlock with the SAME locker
    // initialize threads
    pthread_t producerThreads[10];
    pthread_t consumerThread[1];
    
    
    // initialize the queue
    prod_cons_queue* queue;
    queue_initialize(&queue);

    /*if(DEBUG==1)
	{
		printf("remaininelems: %i\n", queue->remaining_elements); 
    }*/


    // initialize the holder for consumer
    consumerStruct consStruct;
    consStruct.queue=&queue;
    consStruct.locker=&lock;

    // result placeholder
    void* result;
    
    if(DEBUG==0)
	{
		printf("result created\n");
	}
	
    // loop through and create threads
    for (int k=0; k<10; k++)
    {
        if(DEBUG==0)
        {
            printf("enter for loop (main): %i\n", k);
        }
        
		currThread=k+1; 
		producerStruct prodStruct;
		prodStruct.locker=&lock;
		prodStruct.queue=&queue;
		prodStruct.id = k+1;
        
        /// create the thread
        pthread_create(&producerThreads[k], NULL, messageForProducerThread, &prodStruct);
	}
  
    printf("\n");
    
    // create and join the consumer thread
    pthread_create(&consumerThread[0], NULL, consumerInit, &consStruct);
	//the consumer needs to indicate to the producers if they should sleep or wake up.
	
	for(int k=0;k<10;k++)
	{
        pthread_join(producerThreads[k], &result);
    }  
    pthread_join(consumerThread[0], &result);

    return 0;
}

