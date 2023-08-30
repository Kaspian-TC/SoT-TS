#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "TS.h"
#define _N_ 10

// q17 w21 b9 p19 o11 r12 i8 m10 o4 L14

typedef struct island
{
	char * name;
	int x;
	int y;
}Island;

/**
 * @brief Simple function to convert a char to uppercase
 * @param c 
 * @return 
 */
static char toUpperC(char c){
	return (c >= 'a' && c <= 'z') ? c - 0x20 : c; 
}
/**
 * @brief Takes the the list of coords and populates a list of islands for
 * simpler manipulation
 * @param locations 
 * @param islands 
 * @param n 
 */
static void locationsToIslands(char ** locations, Island * islands, int n)
{
	for(int i = 0; i< n;i++)
	{
		islands[i].name = locations[i];
		islands[i].x = toUpperC(locations[i][0])-'A';
		char *ptr;
		islands[i].y = strtol(locations[i]+1, &ptr, 10);
	}
	return;
}

/**
 * @brief Calcualtes all the distances between the coordinates and assigns them
 * to island_distances, which has been allocated on the heap.
 * @param islands 
 * @param island_distances 
 * @param n 
 */
static void calculateIslandDistances(Island * islands,
										 float ** island_distances, const int n)
{
	for(int i = 0; i< n; i++){
		for(int j = 0; j< n; j++){
			//thank god for hypotf
			island_distances[i][j] = hypotf(
				islands[i].x - islands[j].x,islands[i].y - islands[j].y);

			if(island_distances[i][j] == 0){
				island_distances[i][j] = -1;
			}
		}
	}
	return;
}

/**
 * @brief Prints out the path in the adjacency list
 * @param adjacency_list 
 * @param island_locations 
 * @param parent 
 * @param cur_index 
 */
static void printPath(int ** adjacency_list,char ** island_locations,
					  int parent,int cur_index){
	int child;
	if(adjacency_list[cur_index][0] == parent){
		child = adjacency_list[cur_index][ 1];
	}
	else{
		child = adjacency_list[cur_index][ 0];
	}
	printf("%s ",island_locations[cur_index]);
	if(child == -1){
		return;
	}
	else{
		return printPath(adjacency_list,island_locations,cur_index,child);
	}
}

/**
 * @brief Given an adjacency list, find the beginning of the path
 * @param adjacency_list 
 * @param n 
 * @return index of adjacency list that is the start of the path. If no start
 * can be found, return -1
 */
static int findStartIndex(int ** adjacency_list, int n) {
	for(int i = 0; i < n; i++){
		if(adjacency_list[i][0] == -1 || adjacency_list[i][1] == -1){
			return i;
		}
	}
	return -1;
}
/**
 * @brief Takes in a list of island locations and exits if any of them are of 
 * invalid format.
 * @param island_locations 
 * @param n 
 */
static void checkValidInput(char ** island_locations, int n){
	for(int i = 0; i<n;i++){
		char * cur_str = island_locations[i];
		if(strlen(cur_str)<2 || strlen(cur_str)>3){
			fprintf(stderr,
			"Too many or too few characters inputted"
			" for island coordinate: %s\n",cur_str);
			exit(1);	
		}
		else if(toUpperC(cur_str[0]) < 'A' || toUpperC(cur_str[0]) > 'Z'){
			fprintf(stderr,"First character must be for [a-z] or [A-Z]: %s\n",
			cur_str);
			exit(1);
		}
		else if(cur_str[1] < '0' || cur_str[1] > '9'){
			fprintf(stderr,"Second character must be for a digit: %s\n",
			cur_str);
			exit(1);
		}
		else if(strlen(cur_str) == 3 &&
			   (cur_str[2] < '0' || cur_str[2] > '9')){
			fprintf(stderr,"Third character must be for a digit: %s\n",
			cur_str);
			exit(1);
		}
	}
}

/**
 * @brief Checks to see if there are enough inputs to run the program. Exits 
 * otherwise
 * @param n 
 * @param argv 
 */
void checkEnoughInputs(int n, char ** argv){
	if(n <= 1 ||
	(n == 2 && (strcmp(argv[1],"true") == 0 || strcmp(argv[1],"false") == 0)
	)){
		fprintf(stderr,"Needs at least two island inputs");
		exit(1);
	}
	return;
}
int main(int argc, char ** argv)
{
	bool zero_start = false;
	int n = argc-1;
	checkEnoughInputs(n,argv);
	char ** island_locations = &argv[1];
	if(strcmp(island_locations[0],"true") == 0){
		zero_start = true;
		island_locations++;
		n--;
	}
	else if(strcmp(island_locations[0],"false") == 0){
		zero_start = false;
		island_locations++;
		n--;
	}
	checkValidInput(island_locations,n);
	
	Island * islands_2 = malloc(sizeof(Island)*n);
	locationsToIslands(island_locations,islands_2,n);
	float ** island_distances_2 = malloc(sizeof(float*) * n);
	for(int i = 0; i< n;i++){
		island_distances_2[i] = malloc(sizeof(float)*n);
	}
	
	calculateIslandDistances(islands_2,island_distances_2,n);
	float min_sum;
	int ** adjacency_list = findMinRoute(island_distances_2,n,
										 zero_start,&min_sum);
	printf("the best path is:\n");
	int current_index;
	int start_index = 0;
	if(!zero_start){
		start_index = findStartIndex(adjacency_list,n);
		if(start_index == -1)
			exit(1);
	}
	printf("%s ",island_locations[start_index]);
	current_index = adjacency_list[start_index][0] == -1 ?
		adjacency_list[start_index][1]:
		adjacency_list[start_index][0];
	printPath(adjacency_list,island_locations,start_index,current_index);
	
	printf("\nWith a total length of %f map tiles\n", min_sum);
	for(int i = 0; i<n;i++){
		free(adjacency_list[i]);
	}
	free(adjacency_list);
	return 0;
}