#include "Includes.h"

#define GRIDX 20
#define GRIDY 20
#define NUMBOMBS 50

typedef struct{
	bool isCleared;
	bool isFlagged;
	bool isBomb;
	uint adjacentBombs;	// never a zero because it will be clear if zero
}Box;

Box **grid = NULL;

void freeGrid(void)
{
	if(grid == NULL)
	return;

	for(uint x = 0; x < GRIDX; ++x) {
		if(grid[x] != NULL)
			free(grid[x]);
	}
	free(grid);
}

void initGrid(void)
{
	grid = malloc(sizeof(Box*) * GRIDX);
	for(uint x = 0; x < GRIDX; ++x) {
		grid[x] = malloc(sizeof(Box) * GRIDY);
	}
	printf("Adding freeGrid to atexit()\n");
	atexit(freeGrid);
	for(uint y = 0; y < GRIDY; ++y){ // set default values
		for(uint x = 0; x < GRIDX; ++x){
			grid[x][y].isCleared = false;
			grid[x][y].isFlagged = false;
			grid[x][y].isBomb = false;
			grid[x][y].adjacentBombs = 0;
		}
	}
	// picks random x,y values until found box that is not bomb, then sets it to bomb until NUMBOMBS has been reached
	for(uint i = 0; i < NUMBOMBS; ++i){
		uint x = 0, y = 0;
		do {
			x = rand() % GRIDX;
			y = rand() % GRIDY;
		} while(grid[x][y].isBomb);
		grid[x][y].isBomb = true;
	}
}

void printGrid(void)
{
	for(uint y = 0; y < GRIDY; ++y){
		for(uint x = 0; x < GRIDX; ++x){
			putc(grid[x][y].isBomb ? 'B' : '-', stdout);
			putc(' ', stdout);
		}
		putc('\n', stdout);
	}
}

int main(int argc, char *argv[])
{
	gfx_init(800, 600);
	initGrid();
	printGrid();
	while(1) {
		events();
		delay(5);
	}
	return 0;
}
