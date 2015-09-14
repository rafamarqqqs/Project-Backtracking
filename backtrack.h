#ifndef _BACKTRACK_H_
#define _BACKTRACK_H_

#include "graph.h"

#define VERMELHO 0
#define AZUL 1
#define VERDE 2
#define AMARELO 3
#define COLORS 4

#define PICK_COLOR(color) ((color == VERMELHO ? "VERMELHO" : \
(color == AZUL ? "AZUL" : (color == VERDE ? "VERDE" : (color == AMARELO ? "AMARELO" : "UNASSIGNED")))))\

int backtracking(Graph *, int **);

#endif