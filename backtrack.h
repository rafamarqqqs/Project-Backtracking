#ifndef _BACKTRACK_H_
#define _BACKTRACK_H_

#include "graph.h"

#define TRUE 1
#define FALSE 0

#define VERMELHO 0
#define AZUL 1
#define VERDE 2
#define AMARELO 3
#define COLORS 4

#define STANDARD 0
#define FORWARD_CHECKING 1
#define MRV 2
#define ORDER 3

typedef struct possibilities {
	int *colors;
	int size;
	int index;
} Possibilities;
 
typedef unsigned int HEURISTIC;

#define PICK_COLOR(color) ((color == VERMELHO ? "VERMELHO" : \
(color == AZUL ? "AZUL" : (color == VERDE ? "VERDE" : (color == AMARELO ? "AMARELO" : "UNASSIGNED")))))\

int backtracking(Graph *, int **, HEURISTIC flag);

#endif