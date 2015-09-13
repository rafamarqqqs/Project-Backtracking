#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

typedef struct state {
	char *name;
	char **adjacents;
	int size;
} State;

typedef struct map {
	State **states;
	int size;
} Map;

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

char *readState(){
	char *state = NULL;
	int size = 1;
	char aux;

	while((aux = getchar()), aux != ':'){
		state = realloc(state, sizeof(char) * (size + 1));
		state[size - 1] = aux;
		size++;
	}

	state[size - 1] = '\0';

	//reads the space
	getchar();

	return state;
}

void readAdjacents(State *state, int n){
	int size = 1;
	char aux;

	state -> adjacents = (char **) malloc(sizeof(char*));
	state -> adjacents[state -> size] = NULL;
	
	while((aux = getchar()), aux != '.'){
		state -> adjacents[state -> size] = realloc(state -> adjacents[state -> size], sizeof(char) * (size + 1));

		if(aux == ','){
			//reads the space
			getchar();
			state -> adjacents[state -> size][size - 1] = '\0';
			state -> size++;
			state -> adjacents = (char **) realloc(state -> adjacents, sizeof(char *) * (state -> size + 1));
			size = 1;
			state -> adjacents[state -> size] = NULL;
		}
		else{
			state -> adjacents[state -> size][size - 1] = aux;
			size++;
		}
	}

	state -> adjacents[state -> size][size - 1] = '\0';
	state -> size++;
	//reads the '\n'
	getchar();
}

void readMap(Map *map){
	int i;

	map -> states = (State **) malloc(sizeof(State *) * (map -> size));
	
	//reads the '\n'
	getchar();

	for(i = 0; i < map -> size; i++){
		map -> states[i] = (State *) malloc(sizeof(State));
		map -> states[i] -> name = readState();
		map -> states[i] -> size = 0;
		map -> states[i] -> adjacents = NULL;
		readAdjacents(map -> states[i], i);
	}
}

void printMap(Map *map){
	int i, j;

	for(i = 0; i < map -> size; i++){
		printf("%s: ", map -> states[i] -> name);
		for(j = 0; j < map -> states[i] -> size; j++){
			printf("%s ", map -> states[i] -> adjacents[j]);
		}
		printf("\n");
	}
}

void freeMap(Map **map){
	int i, j;

	for(i = 0; i < (*map) -> size; i++){
		free((*map) -> states[i] -> name);
		for(j = 0; j < (*map) -> states[i] -> size; j++)
			free((*map) -> states[i] -> adjacents[j]);
		free((*map) -> states[i] -> adjacents);
		free((*map) -> states[i]);
	}

	free((*map) -> states);
	free(*map);
	*map = NULL;
}

int findStatePosition(Map *map, char *position){
	int i;

	for(i = 0; i < map -> size; i++){
		if(!memcmp(map -> states[i] -> name, position, strlen(position) + 1) && strlen(map -> states[i] -> name) == strlen(position)){
			break;
		}
	}

	return i;
}

void transformMap(Map *map, Graph *graph){
	int i, j;

	for(i = 0; i < map -> size; i++){
		for(j = 0; j < map -> states[i] -> size; j++){
			addEdge(graph, i, findStatePosition(map, map -> states[i] -> adjacents[j]), 0);
		}
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

int main(int argc, char *argv[]){

	Graph *graph;
	int *vector = NULL;
	Map *map;
	clock_t start, end;

	map = (Map *) malloc(sizeof(Map));

	scanf("%d", &(map -> size));

	graph = createGraph('D', map -> size, 0);

	readMap(map);
	printMap(map);
	transformMap(map, graph);

	printGraph(graph);

	vector = (int *) malloc(sizeof(int) * graph -> size);
	
	start = clock();
	backtracking(graph, vector);
	end = clock();

	printVector(vector, graph -> size);

	printf("Time consumed: %lf\n", (double) (end - start)/CLOCKS_PER_SEC);

	//FUNÇÃO SÓ PRA TESTAR OS RESULTADOS NAO ESQUECER DE TIRAR ELA DEPOIS 
	checkResult(graph, vector);

	freeGraph(&graph);
	freeMap(&map);
	free(vector);

	return 0;
}