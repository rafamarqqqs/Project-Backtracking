#ifndef _MAPS_H_
#define _MAPS_H_

#include "graph.h"

typedef struct state {
	char *name;
	char **adjacents;
	int size;
} State;

typedef struct map {
	State **states;
	int size;
} Map;


void readMap(Map *);
void printMap(Map *);
void transformMap(Map *, Graph *);
void freeMap(Map **);

#endif