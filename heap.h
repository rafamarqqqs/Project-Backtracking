#ifndef _heap_h_
#define _heap_h_

typedef struct heapNode {
	int key;
	int id;
} HeapNode;

typedef struct heap {
	struct heapNode **vector;
	int size;
} Heap;

Heap *createHeap();
HeapNode *extractMin(Heap *);
int heapMinimum(Heap *);
void heapInsert(Heap *, int, int);
void printHeap(Heap *);
int heapFind(Heap *, int);
int setKey(Heap *, int, int);
void freeHeap(Heap **);
int empty(Heap *heap);

#endif 