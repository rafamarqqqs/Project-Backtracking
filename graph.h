#ifndef _graph_H_
#define _graph_H_
#include "list.h"

typedef struct graph {

	List **vector;
	int size;

} Graph;

Graph* createGraph(int);
int addVertex(Graph *);
int printGraph(Graph *);
int addEdge(Graph*, int, int, int);
void freeGraph(Graph **);

#endif

