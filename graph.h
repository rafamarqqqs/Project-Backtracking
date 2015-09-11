#ifndef _graph_H_
#define _graph_H_
#include "list.h"

typedef struct graph {

	List **vector;
	int size;
	int is_digraph;

} Graph;

Graph* createGraph(int, int, int);
int addVertex(Graph *);
int printGraph(Graph *);
int addEdge(Graph*, int, int, int);
int removeEdge(Graph*, int, int);
void freeGraph(Graph **);

#endif

