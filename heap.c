#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

#define parent(pos) (pos/2)
#define leftChild(pos) (2*pos)
#define rigthChild(pos) ((2*pos) + 1)

Heap *createHeap(){
	Heap *heap = (Heap *) malloc(sizeof(Heap));
	heap -> vector = NULL;
	heap -> size = 0;
	return heap;
}

void swap(HeapNode **vector, int a, int b){
	HeapNode *c = vector[a];
	vector[a] = vector[b];
	vector[b] = c;
}

void heapDown(Heap *heap, int i){
	int smaller;

	if(leftChild(i) < heap -> size && heap -> vector[leftChild(i)] -> key < heap -> vector[i] -> key)
		smaller = leftChild(i);
	else
		smaller = i;
	if(rigthChild(i) < heap -> size && heap -> vector[rigthChild(i)] -> key < heap -> vector[smaller] -> key)
		smaller = rigthChild(i);

	if(smaller != i){
		swap(heap -> vector, i, smaller);
		heapDown(heap, smaller);
	}
}

void buildMinHeap(Heap *heap){
	int i;

	for(i = 0; i < (heap -> size/2) - 1; i++){
		heapDown(heap, i);
	}
}

int heapMinimum(Heap *heap){
	return heap -> size == 0 ? -1 : heap -> vector[0] -> key;
}

HeapNode *extractMin(Heap *heap){
	if(heap -> size < 1)
		return NULL;

	HeapNode *min = heap -> vector[0];
	heap -> vector[0] = heap -> vector[heap -> size - 1];

	heapDown(heap, 0);
	heap -> size --;

	return min;
}

void heapUp(Heap *heap, int i, HeapNode *key){
	while(i >= 1 && heap -> vector[parent(i)] -> key > heap -> vector[i] -> key){
		swap(heap -> vector, parent(i), i);
		i = parent(i);
	}
}

void heapInsert(Heap *heap, int id, int key){
	HeapNode *node = (HeapNode*) malloc(sizeof(HeapNode));
	node -> id = id;
	node -> key = key;
	heap -> vector = realloc(heap -> vector, (heap -> size + 1) * sizeof(HeapNode *));
	heap -> vector[heap -> size] = node;
	heapUp(heap, heap -> size, node);
	heap -> size ++;
}

void printHeap(Heap *heap){
	int i;
	printf("Heap\n");
	for(i = 0; i < heap -> size; i++)
		printf("%d %d\n", heap -> vector[i] -> id, heap -> vector[i] -> key);
}

int setKey(Heap *heap, int id, int key){
	int i = 0;

	while(i < heap -> size && heap -> vector[i] -> id != id)
		i++;

	if(i == heap -> size)
		return -1;

	heap -> vector[i] -> key = key;

	heapUp(heap, i, heap -> vector[i]);

	return 0;
}

void freeHeap(Heap **heap){
	int i;
	for(i = 0; i < (*heap) -> size; i++)
		free((*heap) -> vector[i]);
	free((*heap) -> vector);
	free(*heap);
	*heap = NULL;
}

int heapFind(Heap *heap, int n){
	int i;
	for(i = 0; i < heap -> size; i++){
		if(heap -> vector[i] -> id == n)
			return 1;
	}

	return 0;
}

int empty(Heap *heap){
	return !(heap -> size);
}