#include<pthread.h>
#include <stdint.h>

#define MAX_QUEUE_SIZE 20

typedef struct queue
{
	int element[MAX_QUEUE_SIZE];
	uint8_t head;
	uint8_t tail;
	uint8_t remaining_elements;
	int current;
	int wait;
	pthread_cond_t cond; 
} prod_cons_queue;

void queue_initialize(prod_cons_queue *q);

void queue_add(prod_cons_queue *q, int element);

int queue_remove(prod_cons_queue *q);

