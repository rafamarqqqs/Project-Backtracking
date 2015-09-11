#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int main(int argc, char *argv[]){

	Graph *graph = createGraph(0, 3, 0);

	addEdge(graph, 0, 1, 0);
	addEdge(graph, 1, 2, 0);

	printGraph(graph);

	freeGraph(&graph);

	return 0;
}