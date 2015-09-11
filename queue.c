#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


Queue *createQueue(){
	Queue *newQueue = (Queue *) malloc(sizeof(Queue));

	newQueue -> head = (Node *) malloc(sizeof(Node));
	newQueue -> head -> content = -1;
	newQueue -> head -> next = NULL;

	return newQueue;
}

int enqueue(Queue *queue, int newVertex){
	if(!queue)
		return -1;

	Node *current = queue -> head;

	while(current -> next != NULL)
		current = current -> next;

	current -> next = (Node *) malloc(sizeof(Node));
	current -> next -> content = newVertex;
	current -> next -> next = NULL;

	return 1;
}

int dequeue(Queue *queue){
	if(!queue || isEmpty(queue))
		return -1;

	int removedVertex = queue -> head -> next -> content;
	Node *removedNode = queue -> head -> next;

	queue -> head -> next = queue -> head -> next -> next;
	free(removedNode);

	return removedVertex;
}

int getFront(Queue *queue){
	return queue -> head -> next == NULL ? -1 : queue -> head -> next -> content;
}

void printQueue(Queue *queue){
	Node *current = queue -> head -> next;

	while(current != NULL){
		printf("%d ", current -> content);
		current = current -> next;
	}
	printf("\n");
}

void freeQueue(Queue **queue){
	Node *current = (*queue) -> head;	
	
	while(current -> next != NULL){
		while(current -> next -> next!= NULL)	
			current = current -> next;
		
		Node *aux = current -> next;
		current -> next = NULL;
		free(aux);
		current = (*queue) -> head;
	}

	free((*queue) -> head);
	free(*queue);
}

int isEmpty(Queue *queue){
	return queue -> head -> next == NULL ? 1 : 0;
}
