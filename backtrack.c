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

int backtracking(Graph *graph, int **vector){
	(*vector) = (int *) malloc(sizeof(int) * graph -> size);
	memset((*vector), -1, graph -> size * sizeof(int));
	return _backtracking(graph, (*vector));
}