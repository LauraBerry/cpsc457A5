#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include "queuestruct.c"

/* PRODUCER STRUCT */
typedef struct
{
    prod_cons_queue* queue;
    pthread_mutex_t* locker;
    int* id;
} producerStruct;

/* CONSUMER STRUCT */
typedef struct
{
    prod_cons_queue* queue;
    pthread_mutex_t* locker;
} consumerStruct;

/* PRODUCER THREAD METHOD */
void* producerInit(void* arg)
{
    // grab queue holder from args
	producerStruct* prodStruct = (producerStruct*) arg;
	
    // loop through 10 times
    for (int i=0; i<10; i++)
    {
        // lock
        pthread_mutex_lock(prodStruct->locker);
            
        // wait if no elements to add
		if(prodStruct->queue->remaining_elements==0)
		{
            /* DEBUG */
            printf("queue full\n");
            
            // increase number of waiting producers
            prodStruct->queue->wait++;
            
            // wait
            pthread_cond_wait(&prodStruct->queue->cond1, prodStruct->locker);
		}
        
        // add element
        queue_add(prodStruct->queue, *prodStruct->id);
        
        // signal to the consumer it's done
        pthread_cond_signal(&prodStruct->queue->cond2);
       
        /* DEBUG */
        if(DEBUG==0)
        {
            for(int i=0; i<20;i++)
            {
                printf("%i: %i\n", i, prodStruct->queue->element[i]);
            }
        }
        
        // unlock
        pthread_mutex_unlock(prodStruct->locker);
    }
    
    /* DEBUG */
    printf("\tID passed in: %i\n", *prodStruct->id);
    
}

/* CONSUMER THREAD METHOD */
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
        
        // wait if the queue is empty (nothing to consume)
        if(consStruct->queue->remaining_elements == MAX_QUEUE_SIZE)
        {
            printf("queue empty\n"); 
            
            // wait
            pthread_cond_wait(&consStruct->queue->cond2, consStruct->locker);
        }
		
        // remove from queue
		int result = queue_remove(consStruct->queue);
        
        // check if someone is waiting
        if(consStruct->queue->wait > 0)
        {
            // decrease number of waiting producers
            consStruct->queue->wait--;
            
            // signal done
            pthread_cond_signal(&consStruct->queue->cond1);
        }
        
        // unlock
        pthread_mutex_unlock(consStruct->locker);
    }
}

/* MAIN FUNCTION */
int main()
{
    printf("Program Start\n");
    
    // initialize the lock
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	
    // initialize the condition variables
    pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
    pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
    
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
        
    // loop through and create threads
    for (int k=0; k<10; k++)
    {
        int identity = k+1;
        
        // initialize producer struct
		producerStruct prodStruct;
		prodStruct.queue = &queue;
		prodStruct.locker = &lock;
		prodStruct.id = &identity;
        
		/* DEBUG */
		//printf("id before: %i\n", *prodStruct.id);
       
        // create the thread
        pthread_create(&producerThreads[k], NULL, producerInit, &prodStruct);
    }
    
    // create and join the consumer thread
    pthread_create(&consumerThread[0], NULL, consumerInit, &consStruct);
    
    // result placeholder
    void* result;

    // join the consumer thread
    pthread_join(consumerThread[0], &result);
	
    // join the producer threads
	for(int j=0; j<10; j++)
	{
        // result placeholder
        void* result;
        
        // join thread
        pthread_join(producerThreads[j], &result);
    }  
    
	/* DEBUG */
    if(DEBUG==1)
	{
        printf("--------------\nfinal queue: \n");
        for(int i=0; i<20;i++)
        {
            printf("%i: %i\n", i, queue.element[i]);
        }
    }
    
    return 0;
}

