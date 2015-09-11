#include <stdlib.h>
#include <stdio.h>
#include "set.h"

SetNode *createSetNode(int n){
	SetNode *newNode = (SetNode *) malloc(sizeof(SetNode));
	newNode -> id = n;
	newNode -> rank = 0;
	newNode -> parent = NULL;
	return newNode;
}

void makeSet(SetNode *root){
	root -> parent = root;
}

SetNode *findSetAux(SetNode *node){
	if(node -> parent != node)
		node -> parent = findSetAux(node -> parent);	//heurística de compressão de caminho
	return node -> parent;
}

int findSet(SetNode *node){
	if(!node)
		return -1;
	return findSetAux(node) -> id;
}

int link(SetNode *a, SetNode *b){						//heurística de união pelo posto

	if(a -> rank < b -> rank)
		a -> parent = b -> parent;
	else{
		b -> parent = a -> parent;
			if(a -> rank == b -> rank)
				b -> rank++;
	}

	return 1;
}

int unite(SetNode *a, SetNode *b){
	if(!a || !b)
		return -1;

	link(findSetAux(a -> parent), findSetAux(b -> parent));

	return 1;
}