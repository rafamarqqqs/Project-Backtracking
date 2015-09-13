#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"



//utilizar a implentação de grafos em matrix ao inves de lista ?
//backtracking tem muita operação de colocar valor em aresta e acessar vertices e arestas
//e a implentação de matriz é mais rápida nesse quesito

#define RED 0
#define BLUE 1
#define GREEN 2
#define YELLOW 3
#define COLORS 4

#define TRUE 1
#define FALSE 0

#define PICK_COLOR(color) ((color == RED ? "RED" : \
(color == BLUE ? "BLUE" : (color == GREEN ? "GREEN" : (color == YELLOW ? "YELLOW" : "UNASSIGNED")))))\

void printVector(int *vector, int n){
	int i;

	for(i = 0; i < n; i++){
		printf("%d: %s\n", i, PICK_COLOR(vector[i]));
	}
}

int assignmentComplete(int *vector, int n){
	int i;

	for(i = 0; i < n; i++){
		if(vector[i] == -1)
			return FALSE;
	}

	return TRUE;
}

int selectUnassignedVariable(int *vector, int n){
	int i;

	for(i = 0; i < n; i++){
		if(vector[i] == -1)
			break;
	}

	return i;	
}

int evaluateValue(Graph *graph, int *vector, int value, int vertex){
	//fazer função getElement para retornar valores da lista !!
	ListNode *current = graph -> vector[vertex] -> head -> next;

	//state doesn't have any boundaries
	if(current == NULL)
		return TRUE;

	//checks if the state can be painted with the color i
	//see if there any edge with value equals i
	while(current != NULL){
		//if the adjacent state has the same color, return false
		if(vector[current -> dest] == value)
			return FALSE;

		current = current -> next;
	}

	return TRUE;
}

void addValue(int *vector, int vertex, int color){
	vector[vertex] = color;
}

void removeValue(int *vector, int vertex){
	vector[vertex] = -1;
}

int _backtracking(Graph *graph, int *vector){
	if(assignmentComplete(vector, graph -> size))
		return TRUE;	

	int i;
	int result;
	int vertex = selectUnassignedVariable(vector, graph -> size);

	for(i = 0; i < COLORS; i++){

		if(evaluateValue(graph, vector, i, vertex)){
			addValue(vector, vertex, i);
			
			result = _backtracking(graph, vector);

			if(result)
				return TRUE;

			removeValue(vector, vertex);
		}
	}

	return FALSE;
}

int backtracking(Graph *graph, int *vector){
	memset(vector, -1, graph -> size * sizeof(int));
	return _backtracking(graph, vector);
}

int main(int argc, char *argv[]){

	Graph *graph = createGraph(0, 3, 0);
	int *vector = NULL;

	addEdge(graph, 0, 1, 0);
	addEdge(graph, 1, 2, 0);

	printGraph(graph);

	vector = (int *) malloc(sizeof(int) * graph -> size);
	
	backtracking(graph, vector);

	printVector(vector, graph -> size);

	freeGraph(&graph);
	free(vector);

	return 0;
}