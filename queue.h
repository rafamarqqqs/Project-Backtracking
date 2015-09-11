#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef struct node {
	int content;
	struct node *next;
} Node;

typedef struct queue {
	Node *head;	
} Queue;

Queue *createQueue();
int enqueue(Queue *, int);
int dequeue(Queue *);
void printQueue(Queue *);
void freeQueue(Queue **);
int isEmpty(Queue *);
int getFront(Queue *);

#endif
