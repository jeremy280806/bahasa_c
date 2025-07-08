#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define MAX_VERTICES 255

int parent[MAX_VERTICES];

struct Edge{
	int source;
	int destination;
};

struct Graph{
	int verticesCount;
	int edgesCount;
	struct Edge* edge;
};

struct Graph* createGraph(int verticesCount, int edgesCount){
	struct Graph* newGraph = (Graph*)malloc(sizeof(Graph));
	newGraph->verticesCount = verticesCount;
	newGraph->edgesCount  = edgesCount;
	newGraph->edge = (Edge*)malloc(sizeof(Edge) * edgesCount);
	return newGraph;
}

void makeSet(int verticesCount){
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

bool join(struct Graph* graph){
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

bool isCyclic(struct Graph* graph){
	makeSet(graph->verticesCount);
	return join(graph); 
}

int main(){
	struct Graph *graph = createGraph(4,4);
	graph->edge[0].source = 0;
	graph->edge[0].destination = 2;
	
	graph->edge[1].source = 2;
	graph->edge[1].destination = 1;
	
	graph->edge[2].source = 1;
	graph->edge[2].destination = 3;
	
	graph->edge[3].source = 2;
	graph->edge[3].destination = 4;
	
	if(isCyclic(graph)){
		printf("This graph is cyclic!\n");
	}
	else{
		printf("This graph is not cyclic\n"); 
	}
	
	free(graph->edge);
	free(graph); 
	return 0;
	
}