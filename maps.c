#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "maps.h"


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

	if(size != 1){
		state -> adjacents[state -> size][size - 1] = '\0';
		state -> size++;
	}
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