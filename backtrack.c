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
int compareMRV(const void *a, const void *b){
	Possibilities *x = (Possibilities *) a;
	Possibilities *y = (Possibilities *) b;
	return x -> size - y -> size;
}

int compareOrder(const void *a, const void *b){
	Possibilities *x = (Possibilities *) a;
	Possibilities *y = (Possibilities *) b;
	return x -> size - y -> size == 0 ? y -> degree - x -> degree : x -> size - y -> size;
}

//minimum remaining values heuristic
int minimumRemainingValues(Graph *graph, int *vector, int n, Possibilities *p, HEURISTIC flag){
	Possibilities *aux = (Possibilities *) malloc(sizeof(Possibilities) * n);
	int index;
	int i;
	int (*compare)(const void *, const void *) = &compareMRV;

	//copy all data from p to aux and sort it
	memcpy(aux, p, sizeof(Possibilities) * n);

	//if using order to determinate to choose between equal possibilities states
	if(flag >= ORDER)
		compare = &compareOrder;

	qsort(aux, n, sizeof(Possibilities), compare);

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
int selectUnassignedVariable(Graph *graph, int *vector, int n, Possibilities *p, HEURISTIC flag){
	int i;

	//if we are using MRV heuristic, let it select the state to color
	if(flag >= MRV)
		return minimumRemainingValues(graph, vector, n, p, flag);

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

void freePossibilities(Possibilities *p, int size){
	int i;

	for(i = 0; i < size; i++){
		free(p[i].colors);
		free(p[i].restrictions);
	}
	
	free(p);
}

void remakePossibilities(Possibilities *p, int *vector, int color, int size){
	int i;

	for(i = 0; i < size; i++){
		if(vector[i] != 0){
			p[i].colors[color] = 0;
			p[i].size--;
		}
	}
}

//forward checking heuristic
int forwardChecking(Graph *graph, int *states, int color, int vertex, Possibilities *p){
    ListNode *current = graph -> vector[vertex] -> head -> next;
    int *vector = NULL;

    //vector to keep all the possibilities values changed
    vector = (int *) calloc(graph -> size, sizeof(int));

    current = graph -> vector[vertex] -> head -> next;

 	//decrement the possibilities of colors for each adjacent state to the state we are coloring
    while(current != NULL){

    	//if the adjacent state doesn't already have a restriction to the color "color", add the restriction
    	if(!hasRestriction(p[current -> dest].colors, color)){
    		vector[current -> dest] = 1;
        	p[current -> dest].colors[color] = 1;
        	p[current -> dest].size--;
    	}
		
		//if someone receives a 0 possibility, cancel the assignment operation
	    if(states[current -> dest] != -1 && p[current -> dest].size == 0) {
	    	remakePossibilities(p, vector, color, graph -> size);
    		free(vector);
	        return FALSE;
	    }
    
        current = current -> next;
    }

    memcpy(p[vertex].restrictions, vector, sizeof(int) * graph -> size);
    free(vector);
 	
    return TRUE;
}

//checks if the assigned color respect the restrictions
int evaluateValue(Graph *graph, int *states, int color, int vertex, Possibilities *p, HEURISTIC flag){
	ListNode *current = graph -> vector[vertex] -> head -> next;

	//state doesn't have any boundaries
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

	//if we are using forward checking, let it decide to return either TRUE of FALSE
 	if(flag >= FORWARD_CHECKING)
 		return forwardChecking(graph, states, color, vertex, p);

 	//color is ok for this state
	return TRUE;
}

//assigns a color to a stata
void addValue(int *vector, int vertex, int color){
	vector[vertex] = color;
}

//removes a color assigned to a state
void removeValue(int size, int *states, int vertex, int color, Possibilities *p, HEURISTIC flag){
	states[vertex] = -1;

	//if using forward checking, remake the possibilities array
	if(flag >= FORWARD_CHECKING)
   		remakePossibilities(p, p[vertex].restrictions, color, size);
}

int _backtracking(Graph *graph, int *states, Possibilities *p, HEURISTIC flag, int *assignments){
	//gets out of the backtracking if the color assignment is complete
	if(assignmentComplete(states, graph -> size))
		return TRUE;	

	int i;
	//selects a statae with no color assigned
	int vertex = selectUnassignedVariable(graph, states, graph -> size, p, flag);

	//for each color available to paint
	for(i = 0; i < COLORS; i++){

		//checks if the restrictions are ok for painting this state with the color i
		if(evaluateValue(graph, states, i, vertex, p, flag)){
			//if yes, add the color to the states colors array
			addValue(states, vertex, i);
			(*assignments)++;

			//do the same for another state
			if(_backtracking(graph, states, p, flag, assignments))
				return TRUE;

			//if some state called from the recursive function above couldn't be painted,
			//remove the state painted before it, and try to paint it with another color
			removeValue(graph -> size, states, vertex, i, p, flag);
		}
	}

	//couldn't find a color to paint the current state, return and try a different color to the state painted before
	return FALSE;
}

//initializes a vector of "possibilities" for each state
void initializePossibilities(Graph *graph, Possibilities **p, HEURISTIC flag){
	int i;

	(*p) = (Possibilities *) malloc(sizeof(Possibilities) * graph -> size);

	for(i = 0; i < graph -> size; i++){
		(*p)[i].index = i;
		(*p)[i].restrictions = (int *) calloc(graph -> size, sizeof(int));
		(*p)[i].colors = (int *) calloc(COLORS, sizeof(int));
		(*p)[i].size = 4;
		(*p)[i].degree = 0;
	}

	//initialize all vertex degrees
	if(flag >= ORDER){
		for(i = 0; i < graph -> size; i++){
			ListNode *current = graph -> vector[i] -> head -> next;

			while(current != NULL){
				(*p)[i].degree++;
				current = current -> next;
			}
		}
	}
}	

int backtracking(Graph *graph, int **states, HEURISTIC flag){
    Possibilities *aux = NULL;
    int assignments;
    
    //setting up vector to keep the color assigned to the states
    (*states) = (int *) malloc(sizeof(int) * graph -> size);
    memset((*states), -1, graph -> size * sizeof(int));

    //initialize other auxiliar variables if doing forward checking
    if(flag >= FORWARD_CHECKING)
    	initializePossibilities(graph, &aux, flag);
    
    _backtracking(graph, (*states), aux, flag, &assignments);
 	
 	if(flag >= FORWARD_CHECKING)
    	freePossibilities(aux, graph -> size);
 
    return assignments;
}