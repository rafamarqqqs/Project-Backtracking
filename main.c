#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "graph.h"
#include "backtrack.h"
#include "maps.h"

void printVector(int *vector, Map *map, int n){
	int i;

	for(i = 0; i < n; i++){
		printf("%s: %s\n", map -> states[i] -> name, PICK_COLOR(vector[i]));
	}
}

HEURISTIC initialize(Map **map, Graph **graph){
    HEURISTIC flag;
	char aux;

    (*map) = (Map *) malloc(sizeof(Map));

	scanf("%d ", &((*map) -> size));
    scanf("%c\n", &aux);
    
    flag = ((int) aux) - 97;

	(*graph) = createGraph((*map) -> size);

	readMap((*map));
	transformMap((*map), (*graph));

    return flag;
}

int main(int argc, char *argv[]){

	Graph *graph;
	int *vector = NULL;
	int assignments;
	Map *map;
	clock_t start, end;
    HEURISTIC flag;

    flag =	initialize(&map, &graph);

	start = clock();
	assignments = backtracking(graph, &vector, flag);
	end = clock();

    printVector(vector, map, graph -> size);
    printf("Heuristic %d\n", flag);
    printf("Time consumed: %lf\n", (double) (end - start)/CLOCKS_PER_SEC);
	printf("Assignments made: %d\n", assignments);

	freeGraph(&graph);
	freeMap(&map);
	free(vector);

	return 0;
}
