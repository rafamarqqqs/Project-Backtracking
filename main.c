#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "graph.h"
#include "backtrack.h"
#include "maps.h"

//utilizar a implentação de grafos em matrix ao inves de lista ?
//backtracking tem muita operação de colocar valor em aresta e acessar vertices e arestas
//e a implentação de matriz é mais rápida nesse quesit

void printVector(int *vector, Map *map, int n){
	int i;

	for(i = 0; i < n; i++){
		printf("%s: %s\n", map -> states[i] -> name, PICK_COLOR(vector[i]));
	}
}

void checkResult(Graph *graph, int *vector){
	int i;

	for(i = 0; i < graph -> size; i++){
		ListNode *current = graph -> vector[i] -> head -> next;

		while(current != NULL){
			if(vector[current -> src] == vector[current -> dest])
				printf("ERRROOOOOOOOOOOOOOOOOOOOU %d %d\n", current -> src, current -> dest);
			current = current -> next;
		}
	}
}

void initialize(Map **map, Graph **graph){
	(*map) = (Map *) malloc(sizeof(Map));

	scanf("%d\n", &((*map) -> size));

	(*graph) = createGraph((*map) -> size);

	readMap((*map));
	transformMap((*map), (*graph));
}

int main(int argc, char *argv[]){

	Graph *graph;
	int *vector = NULL;
	int assignments;
	Map *map;
	clock_t start, end;

	initialize(&map, &graph);

	start = clock();
	assignments = backtracking(graph, &vector, atoi(argv[1]));
	end = clock();

	//printVector(vector, map, graph -> size);
    printf("Heuristic %d\n", atoi(argv[1]));
    printf("Time consumed: %lf\n", (double) (end - start)/CLOCKS_PER_SEC);
	printf("Assignments made: %d\n\n", assignments);

	//FUNÇÃO SÓ PRA TESTAR OS RESULTADOS NAO ESQUECER DE TIRAR ELA DEPOIS 
	checkResult(graph, vector);

	freeGraph(&graph);
	freeMap(&map);
	free(vector);

	return 0;
}
