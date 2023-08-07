#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "TS.h"
#define _N_ 10

// q17 w21 b9 p19 o11 r12 i8 m10 o4 L14

// char * island_locations[_N_] = {"q17","w21","b9","p19","o11","r12","i8","m10","o4","L14"};
// bool zero_start = false;
typedef struct island
{
	char * name;
	int x;
	int y;
}Island;

char toUpperC(char c){
	return (c >= 'a' && c <= 'z') ? c - 0x20 : c; 
}

/* Takes the the list of coords and populates a list of islands for simpler manipulation */
void locationsToIslands(char ** locations, Island * islands, int n)
{
	for(int i = 0; i< n;i++)
	{
		islands[i].name = locations[i];
		islands[i].x = toUpperC(locations[i][0])-'A';
		char *ptr;
		islands[i].y = strtol(locations[i]+1, &ptr, 10);
		// printf("x: %d,y: %d, name %s\n",islands[i].x,islands[i].y,islands[i].name);
	}
	return;
}
void calculateIslandDistances(Island * islands, float * island_distances, int n)
{
	for(int i = 0; i< n; i++){
		for(int j = 0; j< n; j++){
			island_distances[i*n + j] = hypotf(islands[i].x - islands[j].x, islands[i].y - islands[j].y);
			if(island_distances[i*n + j] == 0){
				island_distances[i*n + j] = -1;
			}
		}
	}
	return;
}
void calculateIslandDistances2(Island * islands, float ** island_distances, int n)
{
	for(int i = 0; i< n; i++){
		for(int j = 0; j< n; j++){
			island_distances[i][j] = hypotf(islands[i].x - islands[j].x, islands[i].y - islands[j].y);
			if(island_distances[i][j] == 0){
				island_distances[i][j] = -1;
			}
		}
	}
	return;
}

static void printPath(int ** adjacency_list,char ** island_locations ,int parent,int current_index){
	int child;
	if(adjacency_list[current_index][0] == parent){
		child = adjacency_list[current_index][ 1];
	}
	else{
		child = adjacency_list[current_index][ 0];
	}
	printf("%s ",island_locations[current_index]);
	if(child == -1){
		// printf("%s ",island_locations[child]);
		return;
	}
	else{
		// printf("%s ",island_locations[child]);
		return printPath(adjacency_list,island_locations,current_index,child);
	}
}

int findStartIndex(int ** adjacency_list, int n) {
	for(int i = 0; i < n; i++){
		if(adjacency_list[i][0] == -1 || adjacency_list[i][1] == -1){
			return i;
		}
	}
	return -1;
}

int main(int argc, char ** argv)
{
	bool zero_start = false;
	int n = argc-1;
	if(n<2){
		fprintf(stderr,"Needs at least 2 locations");
		exit(1);
	}
	
	char ** island_locations_2 = &argv[1];
	if(strcmp(island_locations_2[0],"true") == 0){
		zero_start = true;
		island_locations_2++;
		n--;
	}
	else if(strcmp(island_locations_2[0],"false") == 0){
		zero_start = false;
		island_locations_2++;
		n--;
	}
	/*for(int i = 0; i<n;i++){
		printf("%s ",island_locations_2[i]);
	}
	printf("\n");*/
	
	Island * islands_2 = malloc(sizeof(Island)*n);
	locationsToIslands(island_locations_2,islands_2,n);
	float ** island_distances_2 = malloc(sizeof(float*) * n);
	for(int i = 0; i< n;i++){
		island_distances_2[i] = malloc(sizeof(float)*n);
	}
	
	calculateIslandDistances2(islands_2,island_distances_2,n);
	/*for(int i = 0; i<n;i++){
		printf("%d:",i);
		for(int j = 0; j<n;j++){ 
			printf("%f ",island_distances_2[i][j]);
		}
		printf("\n");
	}*/
	float min_sum;
	int ** adjacency_list = findMinRoute(island_distances_2,n,zero_start,&min_sum);
	
	/*for (int i = 0; i < n; i++) {
		printf("%s: ",island_locations_2[i]);
		if(adjacency_list[i][0] != -1){
			printf("%s ", island_locations_2[adjacency_list[i][0]]);
		}
		if(adjacency_list[i][1] != -1){
			printf("%s", island_locations_2[adjacency_list[i][1]]);
		}
		printf("\n");
	}*/
	printf("the best path is:\n");
	int current_index;
	if(zero_start){
		printf("%s ",island_locations_2[0]);
		current_index = adjacency_list[0][0] == -1 ? adjacency_list[0][1] : adjacency_list[0][0];
		printPath(adjacency_list,island_locations_2,0,current_index);

	}
	else{
		int starting_index = findStartIndex(adjacency_list,n);
		if(starting_index == -1){
			return 1;
		}
		printf("%s ",island_locations_2[starting_index]);
		current_index = adjacency_list[starting_index][0] == -1 ? adjacency_list[starting_index][1] : adjacency_list[starting_index][0];
		printPath(adjacency_list,island_locations_2,starting_index,current_index);
		//finish this case
	}
	printf("\nWith a total length of %f map tiles", min_sum);
	for(int i = 0; i<n;i++){
		free(adjacency_list[i]);
	}
	free(adjacency_list);
	return 0;
}