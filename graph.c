#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "list.h"

int addVertex(Graph *graph){					//adds a vertex to the graph
	if(!graph)
		return -1;
	graph -> vector = realloc(graph -> vector, sizeof(List*) * (graph -> size + 1));
	graph -> vector[graph -> size] = createList();
	(graph -> size)++;
	return 1;
}

int addEdge(Graph *graph, int src, int dest, int weight){
	if(!graph || src < 0 || dest < 0 || dest >= graph -> size || src >= graph -> size)
	    return -1;

	addListNode(graph -> vector[src], src, dest, weight, NULL);

	return 1;
}

Graph* createGraph(int vertex){
	if(vertex <= 0)
		return NULL;

	int i;

	Graph *newGraph = (Graph *) malloc(sizeof(Graph));
	newGraph -> vector = (List **) malloc(sizeof(List*) * vertex);
	newGraph -> size = vertex;

    for(i = 0; i < vertex; i++)
	   	newGraph -> vector[i] = createList();

    return newGraph;
}

int printGraph(Graph *graph){
	if(!graph)
	    return -1;
        
	int i;

	for(i = 0; i < graph -> size; i++){
	    printf("%d.", i);
	    printList(graph -> vector[i]);
	}

	return 1;
}

void freeGraph(Graph **graph){
	if(!graph || !(*graph))
		return;
	
	int i;

	for(i = 0; i < (*graph) -> size; i++)
		freeList(&((*graph) -> vector[i]));

	free((*graph) -> vector);
	free(*graph);
	*graph = NULL;
}