#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

/*
Disjoint Sets
Sebuah kota memiliki 5 pulau (bernomor 0, 1, 2, 3) dan 4 sisi:

Jembatan 1: Menghubungkan pulau 0 dan 1.
Jembatan 2: Menghubungkan pulau 1 dan 2.
Jembatan 3: Menghubungkan pulau 2 dan 3.
Jembatan 4: Menghubungkan pulau 3 dan 1.
Pertanyaan: Apakah jembatan-jembatan ini membentuk siklus? Jika ya, jembatan mana yang menyebabkan siklus?
*/

#define MAX_VERTICES 255

int parent[MAX_VERTICES];

struct Edge{
	int source;
	int destination;
};

struct Graph{
	int verticeCount;
	int edgesCount;
	struct Edge *edge;
};

struct Graph *createGraph(int verticesCount, int edgesCount){
	struct Graph *newGraph = (struct Graph*)malloc(sizeof(struct Graph));
	newGraph->verticeCount = verticesCount;
	newGraph->edgesCount = edgesCount;
	newGraph->edge = (struct Edge*)malloc(sizeof(struct Edge) * edgesCount);
	return newGraph; 
}

void MakeSet(int verticesCount){
	for(int i = 0; i < verticesCount; i++){
		parent[i] = i;
	}
}

int findParent(int vertex){
	if(parent[vertex] == vertex){
		return vertex;
	}
	return findParent(parent[vertex]);
}

bool join(struct Graph *graph){
	for(int i = 0; i < graph->edgesCount; i++){
		int sourceParent = findParent(graph->edge[i].source);
		int destinationParent = findParent(graph->edge[i].destination);
		
		if(sourceParent == destinationParent){
			return true;
		}
		parent[sourceParent] = destinationParent;
	}
	return false;
}

bool isCyclic(struct Graph *graph){
	MakeSet(graph->verticeCount);
	return join(graph); 
}

int main(){
	struct Graph *graph = createGraph(5, 4);
	
	graph->edge[0].source = 0;
	graph->edge[0].destination = 1;
	
	graph->edge[1].source = 1;
	graph->edge[1].destination = 2;
	
	graph->edge[2].source = 2;
	graph->edge[2].destination = 3;
	
	graph->edge[3].source = 3;
	graph->edge[3].destination = 1;
	
	if(isCyclic(graph)){
		printf("This graph is cyclic!\n");
	}
	else{
		printf("This graph is not cyclic!\n");
	}
	
	free(graph->edge);
	free(graph);
	
	return 0;  
}