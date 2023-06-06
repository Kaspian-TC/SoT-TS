#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "TS.h"
#define _N_ 10

// char * island_locations[_N_] = {"q17","w21","b9","p19","o11","r12","i8","m10","o4","L14"};
bool zero_start = false;
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
	if(adjacency_list[current_index][ 0] == parent){
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
	// else if(child == ending_index){
		// printf("%s ",island_locations[child]);
		// return;
	// }
	else{
		// printf("%s ",island_locations[child]);
		return printPath(adjacency_list,island_locations,current_index,child);
	}
}

int main(int argc, char ** argv)
{
	// bool zero_start = false;
	int n = argc-1;
	if(n<2){
		fprintf(stderr,"Needs at least 2 locations");
		exit(1);
	}
	
	char ** island_locations_2 = (argv + 1);
	// if(strcmp(island_locations_2[0],"true")){
		// zero_start = true;
		// island_locations_2++;
		// n--;
	// }
	// else if(strcmp(island_locations_2[0],"false")){
		// zero_start = false;
		// island_locations_2++;
		// n--;
	// }
	for(int i = 0; i<n;i++){
		printf("%s ",island_locations_2[i]);
	}
	printf("\n");
	
	Island islands[_N_];
	Island * islands_2 = malloc(sizeof(Island)*n);
	locationsToIslands(island_locations_2,islands_2,n);
	float island_distances[_N_][_N_];
	float ** island_distances_2 = malloc(sizeof(float*) * n);
	for(int i = 0; i< n;i++){
		island_distances_2[i] = malloc(sizeof(float)*n);
	}
	
	// calculateIslandDistances(islands,island_distances[0],n);
	calculateIslandDistances2(islands_2,island_distances_2,n);
	for(int i = 0; i<n;i++){
		printf("%d:",i);
		for(int j = 0; j<n;j++){ 
			printf("%f ",island_distances_2[i][j]);
		}
		printf("\n");
	}
	int ** adjacency_list = findMinRoute(island_distances_2,n,zero_start);
	
	for (int i = 0; i < n; i++) {
		printf("%s: ",island_locations_2[i]);
		if(adjacency_list[i][0] != -1){
			printf("%s ", island_locations_2[adjacency_list[i][0]]);
		}
		if(adjacency_list[i][1] != -1){
			printf("%s", island_locations_2[adjacency_list[i][1]]);
		}
		printf("\n");
	}
	printf("\n");
	printf("%s ",island_locations_2[0]);
	printPath(adjacency_list,island_locations_2,0,adjacency_list[0][0]);
	printf("\n");
	printf("finished running\n");
	for(int i = 0; i<n;i++){
		free(adjacency_list[i]);
	}
	free(adjacency_list);
	return 0;
}