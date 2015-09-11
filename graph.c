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

	if(!(graph -> is_digraph))
		addListNode(graph -> vector[dest], dest, src, weight, NULL);

	return 1;
}

Graph* createGraph(int is_digraph, int vertex, int edges){
	    if(vertex <= 0 || edges < 0)
			return NULL;

	    int i;
	    int src, dest, weight;

	    Graph *newGraph = (Graph *) malloc(sizeof(Graph));
	    newGraph -> vector = (List **) malloc(sizeof(List*) * vertex);
	    newGraph -> size = vertex;
	    newGraph -> is_digraph = is_digraph;

	    for(i = 0; i < vertex; i++)
	    	newGraph -> vector[i] = createList();

	    for(i = 0; i < edges; i++){
	    	scanf("%d %d %d", &src, &dest, &weight);
	    	addEdge(newGraph, src, dest, weight);
	    }

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
	if(!(*graph))
		return;
	
	int i;

	for(i = 0; i < (*graph) -> size; i++)
		freeList(&((*graph) -> vector[i]));

	free((*graph) -> vector);
	free(*graph);
	*graph = NULL;
}

int removeEdge(Graph *graph, int src, int dest){
	if(!graph)
		return -1;

	removeListNode(graph -> vector[src], dest);

	if(!graph -> is_digraph)
		removeListNode(graph -> vector[dest], src);

	return 1;
}