#ifndef _BACKTRACK_H_
#define _BACKTRACK_H_

#include "graph.h"

#define RED 0
#define BLUE 1
#define GREEN 2
#define YELLOW 3
#define COLORS 4

#define PICK_COLOR(color) ((color == RED ? "RED" : \
(color == BLUE ? "BLUE" : (color == GREEN ? "GREEN" : (color == YELLOW ? "YELLOW" : "UNASSIGNED")))))\

int backtracking(Graph *, int **);

#endif