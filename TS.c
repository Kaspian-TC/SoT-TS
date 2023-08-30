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

/* Function that helps mergesort */
static void copyintArray(Edge *a,Edge *b,int size)
{
	for(int i = 0;i<size;i++)
	{
		*(a+i) = *(b+i);
	}
	return;
}
/* Mergesort but for edge weights*/
static Edge* mergesort(Edge* input_array,const int size)
{
	/* if the size of the array is 1 then you can just return the array since it
	is already sorted */
	if(size<=1){ 
		return input_array;
	}
	else
	{
		// sorted_low takes the first half of the array and calls mergesort
		Edge sorted_low[LOW_SIZE];
		copyintArray(sorted_low,mergesort(input_array,LOW_SIZE),LOW_SIZE);

		// sorted_high takes the second half of the array and calls mergesort
		Edge sorted_high[HIGH_SIZE];
		copyintArray(sorted_high,mergesort(input_array+size/2,HIGH_SIZE),
		HIGH_SIZE); 
		int k = 0; //keeps track of the index for the sorted_low
		int j = 0; //keeps track of the index for the sorted_high
		int i;
		
		//i goes through the entire list
		for(i = 0;i<size && k<LOW_SIZE && j<HIGH_SIZE;i++) 
		{
			if(sorted_low[k].value < sorted_high[j].value){	
				/* if sorted_low has a smaller value, then add it to the array
				an increment k*/
				input_array[i] = sorted_low[k];
				k++;
			}
			else //if(sorted_low[k] >= sorted_high[j])
			{	/* if sorted_high has a smaller value (or an equal value), then 
				add it to the array an increment j*/
				input_array[i] = sorted_high[j];
				j++;
			}
		}
		// add remaining items to array
		while(j<HIGH_SIZE){ 
			input_array[i] = sorted_high[j];
			j++;
			i++;
		}
		while(k<LOW_SIZE){
			input_array[i] = sorted_low[k];
			k++;
			i++;
		}
		return input_array;
	}
}
/* Returns true if the edge can be added to both of the verticies */
static bool canAddNeighbour(Edge e,uint8_t * visitedRouteList, bool start_zero){
	// Makes sure that the edge is not already visited twice
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
/**
 * @brief Helper function for containsCycle
 * @param adjacency_list 
 * @param parent 
 * @param current_index 
 * @param ending_index 
 * @return true if it can find the end index from the start index, false 
 * otherwise
 */
static bool containsCycleHelper(int ** adjacency_list, int parent,
								int current_index,int ending_index){
	int child;
	//assigns child to the neighbour that is not the parent
	if(adjacency_list[current_index][ 0] == parent){
		child = adjacency_list[current_index][ 1];
	}
	else{
		child = adjacency_list[current_index][ 0];
	}
	//checks if there is no neighbour to the current index
	if(child == NO_NEIGHBOUR){
		return false;
	}
	//returns true if the child is the ending index
	else if(child == ending_index){
		return true;
	}
	//recursively calls the function on the child
	else{
		return containsCycleHelper(adjacency_list,current_index,
									child,ending_index);
	}
}
/**
 * @brief Checks to see if you can already get to the ending index from the 
 * starting index
 * @param adjacency_list 
 * @param start_index 
 * @param end_index 
 * @return true if it can find the end index from the start index, false 
 * otherwise
 */
static bool containsCycle(int ** adjacency_list, int start_index,int end_index){
	
	int child_index;
	//checks if the starting index is next to the ending index
	if(adjacency_list[start_index][0] == end_index ||
	   adjacency_list[start_index][1] == end_index){
		return true;
	}
	//checks if the starting index has no neighbours
	else if(adjacency_list[start_index][0] == NO_NEIGHBOUR &&
			adjacency_list[start_index][1] == NO_NEIGHBOUR){
		return false;
	}
	//checks if the starting index has exactly one neighbour
	else if((child_index = adjacency_list[start_index][0]) != NO_NEIGHBOUR ||
			(child_index = adjacency_list[start_index][1]) != NO_NEIGHBOUR){
		return containsCycleHelper(adjacency_list,start_index,
								  child_index,end_index);
	}
	/*else if((child_index = adjacency_list[starting_index][1]) != NO_NEIGHBOUR){
		return containsCycleHelper(adjacency_list,
		starting_index,child_index,ending_index);
	}*/
	printf("there is an error here\n");
	exit(1);
	return false;
}

/**
 * @brief Adds a neighbour to the current vertex
 * @param current_vertex 
 * @param neighbour 
 * @return true if successful, false otherwise
 */
static bool addNeighbour(int * current_vertex, int neighbour){
	if(current_vertex[0] == NO_NEIGHBOUR &&
	   current_vertex[1] != neighbour){
		current_vertex[0] = neighbour;
		return true;
	}
	else if(current_vertex[1] == NO_NEIGHBOUR &&
			current_vertex[0] != neighbour){
		current_vertex[1] = neighbour;
		return true;
	}
	else{
		fprintf(stderr,"Error: Cannot add neighbour\n");
		return false;
	}
}

/**
 * @brief Calculate the minimum length path for the given tsp.
 * @param tsp 
 * @param n 
 * @param start_zero 
 * @param min_cost 
 * @return A two dimensional array that represents the adjacency list of the
 * minimum path.  
 */
int ** findMinRoute(float ** tsp,int n,bool start_zero,void * min_cost)
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
	
	int path_index = 0;
	int amount_visited = 0;
	int** adjacency_list = malloc(sizeof(int*) * n);
	for (int i = 0; i < n; i++) {
		adjacency_list[i] = malloc(sizeof(int) * 2);
		adjacency_list[i][0] = NO_NEIGHBOUR;
		adjacency_list[i][1] = NO_NEIGHBOUR;
	}
	/* Sort the edges from smallest to largest */
	mergesort(edges,n*n);
	/* Keep adding edges to path (unless there is a cycle) */
	/* Adds verticies to the path with minimum length edges */
	for(int edge_index = 0; edge_index<n*n && path_index < n;edge_index++){
		Edge current_edge = edges[edge_index];
		if(current_edge.value != -1 && 
		canAddNeighbour(current_edge,visitedRouteList,start_zero) &&
		!containsCycle(adjacency_list,current_edge.v1,current_edge.v2)){
			//make the verticies neighbours
			addNeighbour(adjacency_list[current_edge.v1],current_edge.v2);
			addNeighbour(adjacency_list[current_edge.v2],current_edge.v1);
			//add the edge to the path
			path_edges[path_index] = current_edge;
			path_index++;
			visitedRouteList[current_edge.v1]++;
			visitedRouteList[current_edge.v2]++;
			
			amount_visited+=2;
		}
	}
	//Calculate the sum of edge lengths
	float sum = 0;
	for(int i = 0; i<n-1;i++){
		sum += path_edges[i].value;
	}
	if(min_cost != NULL){
		*((float *)min_cost) = sum;
	}
	free(path_edges);
	free(edges);
	free (visitedRouteList);
	return adjacency_list;
}