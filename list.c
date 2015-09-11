#include <stdlib.h>
#include <stdio.h>
#include "list.h"

List *createList(){
	List *list = (List *) malloc(sizeof(List));
	list -> head = (ListNode *) malloc(sizeof(ListNode));
	list -> head -> weight = -1;
	list -> head -> dest = -1;
	list -> head -> next = NULL;
	return list;
}

ListNode *createNode(int src, int dest, int weight){
	ListNode *listnode = (ListNode *) malloc(sizeof(ListNode));
	listnode -> dest = dest;
	listnode -> src = src;
	listnode -> weight = weight;
	listnode -> next = NULL;
	return listnode;
}

int compareDestinationVertex(ListNode *node, int src, int dest, int weight){
	return node -> dest < dest;
}

int compareMST(ListNode *node, int src, int dest, int weight){	//menor igual para manter a ordem de entrada
	return node -> weight <= weight;
}

int addListNode(List *list, int src, int dest, int weight, Compare compare){
	if(!list)
		return -1;

	if(!compare)
		compare = compareDestinationVertex;

	ListNode *current = list -> head;

	while(current -> next != NULL && compare(current -> next, src, dest, weight))
		current = current -> next;

	if(current -> next){
		ListNode *aux = createNode(src, dest, weight);
		aux -> next = current -> next;
		current -> next = aux;
	}
	else
		current -> next = createNode(src, dest, weight);

	return 1;
}

int removeListNode(List *list, int id){
	if(!list)
		return -1;

	ListNode *current = list -> head;

	while(current -> next != NULL && current -> next -> dest != id)
		current = current -> next;

	if(current -> next){
		int removedId = current -> next -> dest;
		ListNode *removed = current -> next;
		current -> next = current -> next -> next;
		free(removed);
		return removedId;
	}
	return 0;
}

int printList(List *list){
	if(!list)
		return -1;

	ListNode *current = list -> head -> next;

	while(current != NULL){
		printf(" (%d %d)%d ", current -> src, current -> dest, current -> weight);
		current = current -> next;
	}
	printf("\n");

	return 1;
}

void freeList(List **list){
	if(!list || !(*list))
		return;

	ListNode *current = (*list) -> head;
	ListNode *aux;

	while(current -> next != NULL){
		while(current -> next -> next != NULL)
			current = current -> next;
	

		aux = current -> next;
		current -> next = NULL;
		free(aux);
		current = (*list) -> head;
	}

	free((*list) -> head);
	free(*list);
	*list = NULL;
}

int searchNode(List *list, int id){
	if(!list)
		return -1;

	ListNode *current = list -> head -> next;

	while(current != NULL && current -> dest != id)
		current = current -> next;

	return current == NULL ? -2 : current -> weight;
}

int getSmaller(List *list){
	if(!list)
		return -1;

	ListNode *current = list -> head -> next;
	int smaller = -2;

	while(current){
		if(current -> weight < smaller || smaller == -2)
			smaller = current -> weight;

		current = current -> next;
	}

	return smaller;
}
