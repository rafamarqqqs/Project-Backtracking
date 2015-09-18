#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "backtrack.h"

#define TRUE 1
#define FALSE 0

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

int forwardChecking(Graph *graph, int vertex, int *vector){
    ListNode *current = graph -> vector[vertex] -> head -> next;
    int *aux = NULL;
 
    aux = (int *) malloc(sizeof(int) * graph -> size);
 
    memcpy(aux, vector, graph -> size * sizeof(int));
 
    while(current != NULL){
        aux[current -> dest]--;
	
	    if(aux[current -> dest] == 0) {
	        free(aux);
	        return FALSE;
	    }
    
        current = current -> next;
    }
 
    memcpy(vector, aux, graph -> size * sizeof(int));
    free(aux);

    return TRUE;
}

int evaluateValue(Graph *graph, int *states, int value, int vertex, int *forwardCheckingVector, HEURISTIC flag){
	ListNode *current = graph -> vector[vertex] -> head -> next;

	//state doesn't have any boundaries
	if(current == NULL)
		return TRUE;

	//checks if the state can be painted with the color i
	//see if there any edge with value equals i
	while(current != NULL){
		//if the adjacent state has the same color, return false
		if(states[current -> dest] == value)
			return FALSE;

		current = current -> next;
	}

 	if(flag >= FORWARD_CHECKING)
 		forwardChecking(graph, vertex, forwardCheckingVector);

	return TRUE;
}

void addValue(int *vector, int vertex, int color){
	vector[vertex] = color;
}

void removeValue(int *vector, int vertex){
	vector[vertex] = -1;
}

int _backtracking(Graph *graph, int *states, int *forwardCheckingVector, HEURISTIC flag){
	if(assignmentComplete(states, graph -> size))
		return TRUE;	

	int i;
	int result;
	int vertex = selectUnassignedVariable(states, graph -> size);

	for(i = 0; i < COLORS; i++){

		if(evaluateValue(graph, states, i, vertex, forwardCheckingVector, flag)){
			addValue(states, vertex, i);
			
			result = _backtracking(graph, states, forwardCheckingVector, flag);

			if(result)
				return TRUE;

			removeValue(states, vertex);
		}
	}

	return FALSE;
}

void initializeForwardChecking(int **vector, int size){
	(*vector) = (int *) malloc(sizeof(int) * size);
	memset((*vector), 4, size * sizeof(int));
}

int backtracking(Graph *graph, int **states, HEURISTIC flag){
    int *aux = NULL;
    int result;
    
    (*states) = (int *) malloc(sizeof(int) * graph -> size);
    memset((*states), -1, graph -> size * sizeof(int));

    if(flag >= FORWARD_CHECKING)
    	initializeForwardChecking(&aux, graph -> size);
    
    result = _backtracking(graph, (*states), aux, flag);
 
 	if(aux)
    	free(aux);
 
    return result;
}