#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "heap.h"
#include "graph.h"
#include "graphMatrix.h"
#include "queue.h"

#define WHITE 0
#define BLACK -3
#define GRAY -2

#define INFINITE 1000000000

int addVertexListGraph(ListGraph *graph){					//adds a vertex to the graph
	if(!graph)
		return -1;
	graph -> vector = realloc(graph -> vector, sizeof(List*) * (graph -> size + 1));
	graph -> vector[graph -> size] = createList();
	(graph -> size)++;
	return 1;
}

int addEdgeListGraph(ListGraph *graph, int src, int dest, int weight){
	if(!graph || src < 0 || dest < 0 || dest >= graph -> size || src >= graph -> size)
	    return -1;

	addListNode(graph -> vector[src], src, dest, weight, NULL);

	if(!(graph -> is_digraph))
		addListNode(graph -> vector[dest], dest, src, weight, NULL);

	return 1;
}

ListGraph* createListGraph(int is_digraph, int vertex, int edges){
	    if(vertex <= 0 || edges < 0)
			return NULL;

	    int i;
	    int src, dest, weight;

	    ListGraph *newListGraph = (ListGraph *) malloc(sizeof(ListGraph));
	    newListGraph -> vector = (List **) malloc(sizeof(List*) * vertex);
	    newListGraph -> size = vertex;
	    newListGraph -> is_digraph = is_digraph;

	    for(i = 0; i < vertex; i++)
	    	newListGraph -> vector[i] = createList();

	    for(i = 0; i < edges; i++){
	    	scanf("%d %d %d", &src, &dest, &weight);
	    	//printf("Adding edge %d %d %d\n", src, dest, weight);
	    	//scanf("%d %d", &src, &dest);
	    	addEdgeListGraph(newListGraph, src, dest, weight);
	    	//addEdgeListGraph(newListGraph, src, dest, 0);
	    }

     return newListGraph;
}

int printListGraph(ListGraph *graph){
	if(!graph)
	    return -1;
        
	int i;

	for(i = 0; i < graph -> size; i++){
	    	printf("%d.", i);
	    	printList(graph -> vector[i]);
	}

	return 1;
}

int printAdjListGraph(ListGraph *graph, int id){
	if(!graph)
		return -1;

	printf("%d.", id);
	printList(graph -> vector[id]);
	return 1;
}

void freeListGraph(ListGraph **graph){
	if(!(*graph))
		return;
	
	int i;

	for(i = 0; i < (*graph) -> size; i++)
		freeList(&((*graph) -> vector[i]));

	free((*graph) -> vector);
	free(*graph);
	*graph = NULL;
}

int removeEdgeListGraph(ListGraph *graph, int src, int dest){
	if(!graph)
		return -1;

	removeListNode(graph -> vector[src], dest);

	if(!graph -> is_digraph)
		removeListNode(graph -> vector[dest], src);

	return 1;
}

ListGraph *getTransposedListGraph(ListGraph *graph){
	if(!graph || !graph -> is_digraph)
		return NULL;
	
	int i, j, weight;
	ListGraph *newGraph = createListGraph(graph -> is_digraph, graph -> size, 0);

	for(i = 0; i < graph -> size; i++){
		for(j = 0; j < graph -> size; j++){
			if((weight = searchNode(graph -> vector[j], i)), weight != -2)
				addEdgeListGraph(newGraph, i, j, weight);
		}
	}

	printListGraph(newGraph);

	return newGraph;
}

int getLighterEdgeListGraph(ListGraph *graph){
	if(!graph)
		return -1;

	int i, aux, smaller = -3;

	for(i = 0; i < graph -> size; i++){
		if((aux = getSmaller(graph -> vector[i])), (aux < smaller || smaller == -3) && aux != -2)
			smaller = aux;
	}

	return smaller;
}

int breadthFirstSearchListGraph(ListGraph *graph, int start, int searchedVertex, int *p, int *d){	
	if(!graph)
		return -1;

	int *color = (int *) malloc(sizeof(int) * (graph -> size));					//vetor de cores
	int i, u, v;
	p = (int *) malloc(sizeof(int) * (graph -> size));				//vetor de predecessores
	d = (int *) malloc(sizeof(int) * (graph -> size));						//vetor de distancia

	for(i = 0; i < graph -> size; i++){
		p[i] = -1;
		color[i] = WHITE;
		d[i] = 0;
	}

	u = start;

	Queue *queue = createQueue();														
	enqueue(queue, u);									
	color[u] = BLACK;
	d[u] = 0;
	p[u] = -2;

	while(!isEmpty(queue)){								
		u = dequeue(queue);							

			List *listAux = graph -> vector[u];
			ListNode *current = listAux -> head -> next;

			while(current != NULL){ 					
				v = current -> dest;

				if(color[v] == WHITE){				
					color[v] = GRAY;
					d[v] = d[u] + 1;
					p[v] = u;
					enqueue(queue, v);
				}

				current = current -> next;

			}
		color[u] = BLACK;
	}

	buildPath(p, searchedVertex, start);
	freeQueue(&queue);								
	free(color);
	free(p);
	free(d);

	return 1; 									
}



int _depthFirstSearchVisitListGraph(ListGraph *graph, int u, int searchedVertex, int *d, int *color, int *p, 
									int *f, int *visitTime, BYTE condition, ProcessDFS callback, int *vector){

	color[u] = GRAY;		

	d[u] = ++(*visitTime);

	//condição para para a dfs caso o uso da dfs esteja sendo usada para buscar o caminho de um vértice a outro
	if(condition && callback(graph -> size, u, searchedVertex, p, d, f, vector))
		return 1;

	ListNode *current = graph -> vector[u] -> head -> next;

	while(current){
		int v = current -> dest;

		if(color[v] == WHITE){
			p[v] = u;
			if(_depthFirstSearchVisitListGraph(graph, v, searchedVertex, d, color, p, f, visitTime, condition, callback, vector))
				return 1;
		}

		current = current -> next;
	}

	color[u] = BLACK;	
	f[u] = ++(*visitTime);

	return 0;
}

int depthFirstSearchListGraph(ListGraph *graph, int start, int searchedVertex, BYTE condition, ProcessDFS callback, int *vector){
	if(!graph)
		return -1;
	
	int *color = (int *) malloc(sizeof(int) * graph -> size);
	int i;
	int visitTime = 0;
	
	int *p = (int *) malloc(sizeof(int) * graph -> size);
	int *d = (int *) malloc(sizeof(int) * graph -> size);
	int *f = (int *) malloc(sizeof(int) * graph -> size);

	for(i = 0; i < graph -> size; i++){
		p[i] = -1;
		color[i] = WHITE;
		d[i] = f[i] = 0;
	}

	//roda a dfsVisit para cada vértice do grafo a partir de start
	for(i = start; i < graph -> size; i++){
		if(color[i] == WHITE){
			p[i] = -2;
			if(_depthFirstSearchVisitListGraph(graph, i, searchedVertex, d, color, p, f, &visitTime, condition, callback, vector) && i == start){
				break;
			}
		}
	}

	//roda a dfs para os vertices restantes de 0 à start
	if(i == graph -> size){
		for(i = 0; i < start; i++){
			if(color[i] == WHITE){
				p[i] = -2;
				_depthFirstSearchVisitListGraph(graph, i, searchedVertex, d, color, p, f, &visitTime, condition, callback, vector);
			}
		}
	}

	//se condition for 1 quer dizer que a dfs foi usada para buscar o caminho de um vértice ao outro
	//caso contrário a dfs foi usada para ordenação topológica, então rodamos a função que foi passada para copiar o vetor f para um outro vetor
	//que será usado na ordenação topológica
	if(condition)
		buildPath(p, searchedVertex, start);
	else
		callback(graph -> size, 0, 0, p, d, f, vector);

	free(color);
	free(f);
	free(d);
	free(p);

	return 1;
}

MatrixGraph* toMatrix(ListGraph *graph){
	if(!graph)
		return NULL;

	int i;
	MatrixGraph *newGraph = createMatrixGraph(graph -> is_digraph, graph -> size, 0);

	ListNode *current;

	for(i = 0; i < graph -> size; i++){
		current = graph -> vector[i] -> head -> next;

		while(current){
			addEdgeMatrixGraph(newGraph, i, current -> dest, current -> weight);
			current = current -> next;
		}
	}

	freeListGraph(&graph);

	return newGraph;
}

int topologicalOrdenationListGraph(ListGraph *graph, int *vector){
	if(!graph)
		return -1;
	else if(!graph -> is_digraph)
		return 0;
	
	//a função topologicalProcess está no graph.c, assim como a compareOrdTop
	//porque elas são usadas no graphMatrix também
	int i;

	//vetor de struct auxiliar para ordenar o vetor f
	TopOrdIdx *auxVector = (TopOrdIdx *) malloc(sizeof(TopOrdIdx) * graph -> size);

	//roda a dfs para o grafo, com a função topologicalProcess, que copia o vetor "f" no vetor "vector"
	//para ser usado nessa função, após o término da dfs
	depthFirstSearchListGraph(graph, 0, graph -> size - 1, 0, topologicalProcess, vector);

	//copiar o vetor "f", que foi copiado para o vetor "vector" no conteúdo do vetor auxiliar
	//e position recebe o índice de cada um dos valores do vetor
	for(i = 0; i < graph -> size; i++){
		auxVector[i].content = vector[i];
		auxVector[i].position = i;
	}

	//ordena o vetor de struct auxiliar de acordo com compareOrdTop (compara o campo content de cada uma das structs)
	qsort(auxVector, graph -> size, sizeof(TopOrdIdx), compareOrdTop);

	//ao final do processo, o campo position terá a ordenação topológica correta
	for(i = 0; i < graph -> size; i++)
		vector[i] = auxVector[i].position;

	free(auxVector);

	return 1;
}

//Algoritmo de MST de Kruskal feito em um grafo representado por lista
ListGraph *kruskalMSTListGraph(ListGraph *graph){
	if(!graph)
		return NULL;

	int i, u, v;
	ListGraph *newGraph = NULL;
	SetNode **vertexSet = NULL;
	List *edges = NULL;
	ListNode *current = NULL;

	newGraph = createListGraph(graph -> is_digraph, graph -> size, 0);						//inicializa um novo grafo
	edges = createList();																	//lista de arestas
	vertexSet = (SetNode **) malloc(sizeof(SetNode *) * graph -> size);						//sets para cada vértice

	//inicializa os sets para cada vértice
	for(i = 0; i < graph -> size; i++){
		vertexSet[i] = createSetNode(i);
		makeSet(vertexSet[i]);
	}
	
	for(i = 0; i < graph -> size; i++){														//coloca as arestas em uma lista ordenada
		current = graph -> vector[i] -> head -> next;

		while(current){
			addListNode(edges, current -> src, current -> dest, current -> weight, compareMST);
			current = current -> next;
		}
	}

	current = edges -> head -> next;

	//passa por todas as arestas presentes no vetor de arestas
	while(current){
		
		u = current -> src;
		v = current -> dest;

		//se dois vértices não pertencerem ao mesmo set, adicionamos a aresta entre eles ao novo grafo e unimos os seus sets
		if(findSet(vertexSet[u]) != findSet(vertexSet[v])){									//se u e v não pertencem à mesma árvore
			addEdgeListGraph(newGraph, u, v, current -> weight);							//adiciona a aresta no novo grafo
			unite(vertexSet[u], vertexSet[v]);												//une dois sets
		}
		
		current = current -> next;
	}
	
	freeList(&edges);

	for(i = 0; i < graph -> size; i++)
		free(vertexSet[i]);

	free(vertexSet);

	return newGraph;								//retorna o grafo cujas arestas formam uma MST
}

ListGraph *primMSTListGraph(ListGraph *graph){
	if(!graph)
		return NULL;

	int i;
	int *p, *k;
	ListGraph *newGraph = createListGraph(graph -> is_digraph, graph -> size, 0);
	
	SetNode **setVector = (SetNode **) malloc(sizeof(SetNode *) * graph -> size);
	SetNode *mainSet;

	Heap *heap = createHeap();

	ListNode *current = NULL;

	k = (int *) malloc(sizeof(int) * graph -> size);
	p = (int *) malloc(sizeof(int) * graph -> size);

	for(i = 0; i < graph -> size; i++){
		k[i] = INT_MAX;
		p[i] = -1;
	}

	//faz o set para ser o "corte"
	mainSet = createSetNode(-1);
	makeSet(mainSet);

	//um set para cada um dos elementos, colocar todos na heap
	for(i = 0; i < graph -> size; i++){
		setVector[i] = createSetNode(i);
		makeSet(setVector[i]);
		heapInsert(heap, i, INT_MAX);
	}
	
	k[0] = 0;
	setKey(heap, 0, 0);	//root começa com key = 0


	while(heap -> size > 0){
		HeapNode *node = extractMin(heap);

		if(p[node -> id] != -1){
			unite(mainSet, setVector[node -> id]);
			addEdgeListGraph(newGraph, node -> id, p[node -> id], k[node -> id]);
		}
		
		current = graph -> vector[node -> id] -> head -> next;

		while(current){
			if(findSet(setVector[current -> dest]) != findSet(mainSet) && current -> weight < k[current -> dest]){
				p[current -> dest] = current -> src;
				k[current -> dest] = current -> weight;
				setKey(heap, current -> dest, current -> weight);
			}

			current = current -> next;
		}

	}

	for(i = 0; i < graph -> size; i++)
		free(setVector[i]);

	freeHeap(&heap);
	free(mainSet);
	free(setVector);
	free(k);
	free(p);

	return newGraph;
}

int printSumListGraph(ListGraph *graph){
	if(!graph)
		return -1;

	int i, weight = 0;
	List **auxVector = (List **) malloc(sizeof(List *) * graph -> size);

	for(i = 0; i < graph -> size; i++)
		auxVector[i] = createList();

	for(i = 0; i < graph -> size; i++){
		ListNode *current = graph -> vector[i] -> head -> next;

		while(current){
			//utilizado para não somar duas vezes o peso de uma mesma aresta
			if(searchNode(auxVector[current -> dest], current -> src) == -2){
				addListNode(auxVector[i], current -> src, current -> dest, current -> weight, compareMST);
				weight += current -> weight;
			}			

			current = current -> next;
		}
	}

	for(i = 0; i < graph -> size; i++)
		freeList(&auxVector[i]);

	free(auxVector);
	
	return weight;
}

//processo de relaxamento das arestas
void relaxListGraph(Heap *heap, int *d, int *p, int u, int v, int w){
	if(d[v] > w + d[u]){
		//atualiza o valor do vértice na heap
		setKey(heap, v, w + d[u]);
		p[v] = u;
		d[v] = w + d[u];
	}
}

int dijkstraShortestPathListGraph(ListGraph *graph, int start, int end){
	if(!graph -> is_digraph)
		return -1;

	Heap *heap;
	int i, *p, *d, u, v, w;

	//vetores predecessor e "distancia"
	p = (int *) malloc(sizeof(int) * graph -> size);
	d = (int *) malloc(sizeof(int) * graph -> size);
	heap = createHeap();

	for(i = 0; i < graph -> size; i++){
		p[i] = -1;
		d[i] = INFINITE;
		heapInsert(heap, i, INFINITE);
	}

	//muda o peso no vértice inicial e o seu valor na heap
	d[start] = 0;
	setKey(heap, start, 0);

	while(!empty(heap)){
		HeapNode *node = extractMin(heap);
		
		u = node -> id;
		ListNode *current = graph -> vector[u] -> head -> next;

		//aplica o processo de relaxamento dos vértices para todos os vértices adjacentes a u
		while(current){
			v = current -> dest;
			w = current -> weight;

			relaxListGraph(heap, d, p, u, v, w);

			current = current -> next;
		}

		free(node);
	}

	//imprime o caminho
	buildPath(p, end, start);

	free(p);
	free(d);
	freeHeap(&heap);

	return 1;
}
