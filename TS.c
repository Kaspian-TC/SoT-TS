#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#define NO_NEIGHBOUR (-1)
// Function to find the minimum cost path for all the paths
typedef struct edge
{
	float value;
	int v1;
	int v2;
}Edge;
#define LOW_SIZE (size/2)
#define HIGH_SIZE (size-size/2)

static void copyintArray(Edge *a,Edge *b,int size)
{
	for(int i = 0;i<size;i++)
	{
		*(a+i) = *(b+i);
	}
	return;
}
Edge* mergesort(Edge* input_array,const int size)
{
	if(size<=1){ // if the size of the array is 1 then you can just return the array since it is already sorted
		return input_array;
	}
	else
	{
		Edge sorted_low[LOW_SIZE];
		copyintArray(sorted_low,mergesort(input_array,LOW_SIZE),LOW_SIZE);// sorted_low takes the first half of the array and calls mergesort
		Edge sorted_high[HIGH_SIZE];
		copyintArray(sorted_high,mergesort(input_array+size/2,HIGH_SIZE),HIGH_SIZE); // sorted_high takes the second half of the array and calls mergesort
		int k = 0; //keeps track of the index for the sorted_low
		int j = 0; //keeps track of the index for the sorted_high
		int i;
		for(i = 0;i<size && k<LOW_SIZE && j<HIGH_SIZE;i++) //i goes through the entire list
		{
			if(sorted_low[k].value < sorted_high[j].value) 
			{	// if sorted_low has a smaller value, then add it to the array an increment k
				// copyEdge(input_array + i, sorted_low+k);
				input_array[i] = sorted_low[k];
				k++;
			}
			else //if(sorted_low[k] >= sorted_high[j])
			{	// if sorted_high has a smaller value (or an equal value), then add it to the array an increment j
				// copyEdge(input_array + i, sorted_high +j);
				input_array[i] = sorted_high[j];
				j++;
			}
		}
		while(j<HIGH_SIZE){ // add remaining items to array
			// copyEdge(input_array + i, sorted_high +j);
			input_array[i] = sorted_high[j];
			j++;
			i++;
		}
		while(k<LOW_SIZE){
			// copyEdge(input_array + i, sorted_low+k);
			input_array[i] = sorted_low[k];
			k++;
			i++;
		}
		return input_array;
	}
}
static bool canAddNeighbour(Edge e,uint8_t * visitedRouteList, bool start_zero){
	if(visitedRouteList[e.v1] != 2 && visitedRouteList[e.v2] != 2)
	{
		if(start_zero){
			if(e.v1 == 0){
				return visitedRouteList[e.v1] == 0;
			}
			else if(e.v2 == 0){
				return visitedRouteList[e.v2] == 0;
			}
		}
		return true;
	}
	return false;
}
static bool containsCycleHelper(int ** adjacency_list, int parent,int current_index,int ending_index,int n){
	int child;
	if(adjacency_list[current_index][ 0] == parent){
		child = adjacency_list[current_index][ 1];
	}
	else{
		child = adjacency_list[current_index][ 0];
	}
	if(child == NO_NEIGHBOUR){
		return false;
	}
	else if(child == ending_index){
		return true;
	}
	else{
		return containsCycleHelper(adjacency_list,current_index,child,ending_index,n);
	}
}
static bool containsCycle(int ** adjacency_list, int starting_index,int ending_index,int n){
	
	int child_index;
	if(adjacency_list[starting_index][0] == ending_index || adjacency_list[starting_index][1] == ending_index){
		return true;
	}
	else if(adjacency_list[starting_index][0] == NO_NEIGHBOUR && adjacency_list[starting_index][1] == NO_NEIGHBOUR){
		return false;
	}
	else if((child_index = adjacency_list[starting_index][0]) != NO_NEIGHBOUR){
		return containsCycleHelper(adjacency_list,starting_index,child_index,ending_index,n);
	}
	else if((child_index = adjacency_list[starting_index][1]) != NO_NEIGHBOUR){
		return containsCycleHelper(adjacency_list,starting_index,child_index,ending_index,n);
	}
	printf("there is an error here\n");
	exit(1);
	return false;
}
static bool addNeighbour(int * current_vertex, int neighbour){
	if(current_vertex[0] == NO_NEIGHBOUR &&
				neighbour!=current_vertex[1])
	{
		current_vertex[0] = neighbour;
		return true;
	}
	else if(current_vertex[1] == NO_NEIGHBOUR &&
		neighbour!=current_vertex[0])
	{
		current_vertex[1] = neighbour;
		return true;
	}
	else{
		return false;
	}
}
int ** findMinRoute(float ** tsp,int n,bool start_zero)
{
	uint8_t * visitedRouteList = calloc(sizeof (uint8_t), n);
	
	/* Create a list of all edges */
	Edge * edges = malloc(sizeof(Edge) *n*n);
	for(int i = 0;i<n;i++){
		for(int j = 0;j<n;j++){
			int index = i*n+j;
			
			edges[index].v1 = i;
			edges[index].v2 = j;
			edges[index].value = tsp[i][j];
		}
	}
	Edge * path_edges = malloc(sizeof(Edge) * n);
	/* Sort the edges from smallest to largest */
	
	/* Keep adding edges to path (unless there is a cycle) */
	int j = 0;
	int amount_visited = 0;
	int** adjacency_list = malloc(sizeof(int*) * n);
	for (int i = 0; i < n; i++) {
		adjacency_list[i] = malloc(sizeof(int) * 2);
		adjacency_list[i][0] = NO_NEIGHBOUR;
		adjacency_list[i][1] = NO_NEIGHBOUR;
	}
	mergesort(edges,n*n);
	for(int i = 0; i<n*n && j < n;i++){
		if(edges[i].value != -1 && 
		canAddNeighbour(edges[i],visitedRouteList,start_zero) &&
		!containsCycle(adjacency_list,edges[i].v1,edges[i].v2,20)){
			int * current_vertex = adjacency_list[edges[i].v1];
			addNeighbour(current_vertex,edges[i].v2);
			current_vertex = adjacency_list[edges[i].v2];
			addNeighbour(current_vertex,edges[i].v1);
			
			path_edges[j] = edges[i];
			j++;
			visitedRouteList[edges[i].v1]++;
			visitedRouteList[edges[i].v2]++;
			
			amount_visited+=2;
		}
	}
	float sum = 0;
	for(int i = 0; i<n-1;i++){
		sum += path_edges[i].value;
	}
	
	// printf("Minimum Cost is : ");
	// printf("%f\n",sum);
	// for (int i = 0; i < n; i++) {
		// printf("%d: %d %d\n",i,adjacency_list[i][0],adjacency_list[i][1]);
	// }
	free(path_edges);
	free(edges);
	free (visitedRouteList);
	return adjacency_list;
}
// int main()
// {
    // Input Matrix
    // float tsp[4][4] = { { -1, 10, 15, 20 },
                                 // { 10, -1, 35, 25 },
                                 // { 15, 35, -1, 30 },
                                 // { 20, 25, 30, -1 } };
    // findMinRoute(tsp[0],4);
// }