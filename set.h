#ifndef _set_H_
#define _set_H_


typedef struct setnode{
	struct setnode *parent;
	int rank;					//usado em união pelo posto de 2 sets
	int id;
} SetNode;

void makeSet(SetNode*);
int findSet(SetNode*);			//compressão de caminho
int unite(SetNode*, SetNode*);
SetNode *createSetNode(int);


#endif