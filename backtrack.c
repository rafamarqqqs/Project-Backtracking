#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "backtrack.h"

//checks if the assignment is complete
int assignmentComplete(int *vector, int n){
	int i;

	for(i = 0; i < n; i++){
		if(vector[i] == -1)
			return FALSE;
	}

	return TRUE;
}

//compare function to compare the possibilities struct
int comparePossibilities(const void *a, const void *b){
	Possibilities *x = (Possibilities *) a;
	Possibilities *y = (Possibilities *) b;
	return x -> size - y -> size;
}

//minimum remaining values heuristic
int minimumRemainingValues(int *vector, int n, Possibilities *p){
	Possibilities *aux = (Possibilities *) malloc(sizeof(Possibilities) * n);
	int index;
	int i;
	printf("MRV...\n");

	//copy all data from p to aux and sort it
	memcpy(aux, p, sizeof(Possibilities) * n);
	qsort(aux, n, sizeof(Possibilities), comparePossibilities);

	//get the index of the first unassigned state
	for(i = 0; i < n; i++){
		if(vector[aux[i].index] == -1){
			index = aux[i].index;
			break;
		}
	}

	free(aux);

	return index;
}

//selects a variable to assign a color
int selectUnassignedVariable(int *vector, int n, Possibilities *p, HEURISTIC flag){
	int i;

	//if we are using MRV heuristic, let it select the state to color
	if(flag >= MRV)
		return minimumRemainingValues(vector, n, p);

	//otherwise just return the next available state
	for(i = 0; i < n; i++){
		if(vector[i] == -1)
			break;
	}

	return i;	
}

//checks if the state already has the restriction to the color assigned or not
int hasRestriction(int *colors, int color){
	return colors[color] != 0 ? TRUE : FALSE;
}

//forward checking heuristic
int forwardChecking(Graph *graph, int *states, int color, int vertex, Possibilities *p){
    ListNode *current = graph -> vector[vertex] -> head -> next;
    Possibilities *aux = NULL;
    int i, j;

    aux = (Possibilities *) malloc(sizeof(Possibilities) * graph -> size);

    //copy all data from p to an aux vector
    for(i = 0; i < graph -> size; i++){
    	aux[i].colors = (int *) malloc(sizeof(int) * COLORS);
    	
    	for(j = 0; j < COLORS; j++){
    		aux[i].colors[j] = p[i].colors[j];
    	}
    	
    	aux[i].size = p[i].size;
    	aux[i].index = p[i].index;
    }

    //memcpy(aux, p, graph -> size * sizeof(Possibilities));

 	//decrement the possibilities of colors for each adjacent state to the state we are coloring
    while(current != NULL){

    	if(!hasRestriction(aux[current -> dest].colors, color)){
        	aux[current -> dest].colors[color] = 1;
        	aux[current -> dest].size--;
    	}
		
		//if someone receives a 0 possibility, cancel the assignment operation
	    if(aux[current -> dest].size == 0) {
	        free(aux);
	        return FALSE;
	    }
    
        current = current -> next;
    }
 	
 	//otherwise, make the aux vector our new possibilities vector
    //memcpy(p, aux, graph -> size * sizeof(Possibilities));
    for(i = 0; i < graph -> size; i++){
    	for(j = 0; j < COLORS; j++){
    		aux[i].colors[j] = p[i].colors[j];
    	}
    	p[i].size = aux[i].size;
    	p[i].index = aux[i].index;
    }

    free(aux);

    return TRUE;
}

//checks if the assigned color respect the restrictions
int evaluateValue(Graph *graph, int *states, int color, int vertex, Possibilities *p, HEURISTIC flag){
	ListNode *current = graph -> vector[vertex] -> head -> next;

	//state doesn't have any boundaries
	//falta corrigir, não ta pegando
	if(current == NULL)
		return TRUE;

	//checks if the state can be painted with the color i
	//see if there any edge with color equals i
	while(current != NULL){
		//if the adjacent state has the same color, return false
		if(states[current -> dest] == color)
			return FALSE;

		current = current -> next;
	}

	//if we are using forward checking, use it before returning to the recursion
 	if(flag >= FORWARD_CHECKING)
 		return forwardChecking(graph, states, color, vertex, p);

	return TRUE;
}

void addValue(int *vector, int vertex, int color){
	vector[vertex] = color;
}

int checkRestriction(Graph *graph, int *states, int color, int vertex){
	ListNode *current = graph -> vector[vertex] -> head -> next;
	
	while(current != NULL){
		if(states[current -> dest] == color)
			return TRUE;
		current = current -> next;
	}

	return FALSE;
}

void removeValue(Graph *graph, int *states, int vertex, int color, Possibilities *p){
	ListNode *current = NULL;
	int i, j;

	states[vertex] = -1;

	for(i = 0; i < graph -> size; i++){
		for(j = 0; j < COLORS; j++)
			p[i].colors[j] = 0;
		p[i].size = 4;
		p[i].index = i;
	}

	for(i = 0; i < graph -> size; i++){
		current = graph -> vector[i] -> head -> next;

		while(current != NULL){

	    	if(states[current -> dest] != -1 && !hasRestriction(p[i].colors, states[current -> dest])){
	        	p[i].colors[states[current -> dest]] = 1;
	        	p[i].size--;
	    	}
	    
	        current = current -> next;
	    }
	}


}

int _backtracking(Graph *graph, int *states, Possibilities *p, HEURISTIC flag){
	if(assignmentComplete(states, graph -> size))
		return TRUE;	

	int i;
	int result;
	int vertex = selectUnassignedVariable(states, graph -> size, p, flag);
	printf("alive\n");
	for(i = 0; i < COLORS; i++){

		if(evaluateValue(graph, states, i, vertex, p, flag)){
			addValue(states, vertex, i);
			
			result = _backtracking(graph, states, p, flag);

			if(result){
				return TRUE;
			}

			removeValue(graph, states, vertex, i, p);
		}
	}

	return FALSE;
}

void initializePossibilities(Possibilities **p, int size){
	int i;

	(*p) = (Possibilities *) malloc(sizeof(Possibilities) * size);

	for(i = 0; i < size; i++){
		(*p)[i].colors = (int *) calloc(COLORS, sizeof(int));
		(*p)[i].size = 4;
		(*p)[i].index = i;
	}
}	

int backtracking(Graph *graph, int **states, HEURISTIC flag){
    Possibilities *aux = NULL;
    int result;
    
    (*states) = (int *) malloc(sizeof(int) * graph -> size);
    memset((*states), -1, graph -> size * sizeof(int));

    if(flag >= FORWARD_CHECKING)
    	initializePossibilities(&aux, graph -> size);
    
    result = _backtracking(graph, (*states), aux, flag);
 
 	if(aux){
    	free(aux);
 	}
 
    return result;
}