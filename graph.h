#ifndef _graphList_H_
#define _graphList_H_

#include "graph.h"
#include "graphList.h"

ListGraph* createListGraph(int, int, int);
int addVertexListGraph(ListGraph *);
int printListGraph(ListGraph *);
int addEdgeListGraph(ListGraph*, int, int, int);
int printAdjListGraph(ListGraph *, int);
int removeEdgeListGraph(ListGraph*, int, int);
void freeListGraph(ListGraph **);
int getLighterEdgeListGraph(ListGraph *);

ListGraph* getTransposedListGraph(ListGraph *);
int breadthFirstSearchListGraph(ListGraph *, int, int);
int depthFirstSearchListGraph(ListGraph *, int, int, BYTE, ProcessDFS, int *);

MatrixGraph *toMatrix(ListGraph *);

int topologicalOrdenationListGraph(ListGraph *, int *);

ListGraph *kruskalMSTListGraph(ListGraph *);
ListGraph *primMSTListGraph(ListGraph *);
int printSumListGraph(ListGraph *);
int dijkstraShortestPathListGraph(ListGraph *, int, int);

#endif

