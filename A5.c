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
    pthread_cond_t* cond;
    int* id;
} producerStruct;

typedef struct
{
    prod_cons_queue* queue;
    pthread_mutex_t* locker;
    pthread_cond_t* cond;
} consumerStruct;

void* messageForProducerThread(void* args)
{
    printf("producer thread initializer\n");  
    
    // grab queue holder from args
	producerStruct* prodStruct = (producerStruct*) args;
        
    for (int i=0; i<10; i++)
    {
        // lock
        pthread_mutex_lock(prodStruct->locker);
            
        if(DEBUG==0)
        {
            printf("[CURRENT]:\t%i\n", *prodStruct->id, currThread);
        }
        
        /* DEBUG 
		if(prodStruct->queue->wait==1)
		{
			printf("\nthread #: %i is waiting...\n", currThread);
		} 
        
        // wait
		while(prodStruct->queue->wait==1)
		{
			pthread_cond_wait(prodStruct->cond, prodStruct->locker);
		}*/
        
        // add element
        queue_add(prodStruct->queue, *prodStruct->id);
       
        // signal done
        // pthread_cond_signal(prodStruct->cond);
        
        // unlock
        pthread_mutex_unlock(prodStruct->locker);
        
        
		interator--;
    }
    /* DEBUG*/
   for(int i=0;i<20;i++)
    {
		if(currThread<11)
		{
        	printf("id: %i\t%i: %i\n", *prodStruct->id, i, prodStruct->queue->element[i]);
    	}
	} 
    
    if (DEBUG==0)
    {
        printf("id: %i\n", prodStruct->id);
    }
	pthread_exit(NULL);
}

void* consumerInit(void* args)
{
    printf("consumer thread inializer\n"); 
    // grab consStruct
    consumerStruct* consStruct = (consumerStruct*) args;
    
    // loop 100 times
	for(int i=0; i<100; i++)
	{
        // lock
		pthread_mutex_lock(consStruct->locker);
        
        // wait
		/*while(consStruct->queue->wait==0)
		{
            pthread_cond_wait(consStruct->cond,consStruct->locker);
		}*/
        
        // remove from queue
		int result = queue_remove(consStruct->queue);
        
        // signal done
        // pthread_cond_signal(consStruct->cond);
        
        // unlock
        pthread_mutex_unlock(consStruct->locker);
        
        /* DEBUG
		printf("result is: %i \n", result);
		printf("consumer thread has been called:\n");
		for(int i=0;i<20;i++)
		{
		    	printf(" %i\n", consStruct->queue->element[i]);
		} */
	 }  
	 pthread_exit(NULL);
}


int main()
{
    printf("Program Start\n");
    
    // initialize the lock
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	
    // initialize the condition variable
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    
    // initialize threads
    pthread_t producerThreads[10];
    pthread_t consumerThread[1];
    
    // initialize the queue
    prod_cons_queue queue;
    queue_initialize(&queue);

    // initialize the holder for consumer
    consumerStruct consStruct;
    consStruct.queue = &queue;
    consStruct.locker = &lock;
    consStruct.cond = &cond;

    
	printf("queue before: \n");
	for (int i=0; i<20; i++)
	{
		printf("%i:\t%i\n",i, queue.element[i]);
	}
    
    // loop through and create threads
    for (int k=0; k<10; k++)
    {
        if(DEBUG==0)
        {
            printf("enter for loop (main): %i\n", k);
        }
        
        // properly modify id for thread
        int identity = k+1;
        
        /* DEBUG */
        currThread = identity;
        
        // initialize producer struct
		producerStruct prodStruct;
        
		prodStruct.locker = &lock;
		prodStruct.queue = &queue;
		prodStruct.id = &identity;
        prodStruct.cond = &cond;

        /// create the thread
        pthread_create(&producerThreads[k], NULL, messageForProducerThread, &prodStruct);
	}
  
    printf("\n");
    
    // create and join the consumer thread
    pthread_create(&consumerThread[0], NULL, consumerInit, &consStruct);

    // join the threads
	for(int k=0;k<10;k++)
	{
        // result placeholder
        void* result;
        
        // join thread
        pthread_join(producerThreads[k], &result);
    }  
    
    // result placeholder
    void* result;
    
    // join thread
    pthread_join(consumerThread[0], &result);

    return 0;
}

