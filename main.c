#include "Includes.h"

#define SCALE			48

#define GRIDX_DEFAULT		30
#define GRIDY_DEFAULT		16
#define NUMBOMBS_DEFAULT	70

uint GRIDX, GRIDY, NUMBOMBS;

uint remainingBox, bombCounter, timer;

typedef enum{MARK_NONE, MARK_FLAG, MARK_QUESTION}Marker;

typedef struct{
	bool bomb;
	bool clear;
	Marker mark;
	uint number;
}Box;

Box **grid = NULL;

#include "Print.h"
#include "Draw.h"

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

uint getAdjacent(uint x, uint y)
{
	printf("getting adjacent at grid (%u, %u)...\n", x, y);
	uint count = 0;
	if(grid[x][y].bomb)
		return count;
	for(uint yoff = y>0 ? y-1 : y; yoff < GRIDY && yoff <= y+1; yoff++){
		for(uint xoff = x>0 ? x-1 : x; xoff < GRIDX && xoff <= x+1; xoff++){
			printf("checking (%u, %u)\n", xoff, yoff);
			events();
			if(xoff != x || yoff != y)
				count += grid[xoff][yoff].bomb;
		}
	}
	printf(" found %u\n", count);
	return count;
}

void initGrid(void)
{
	setFontSize(SCALE-SCALE/8);
	grid = malloc(sizeof(Box*) * GRIDX);
	for(uint x = 0; x < GRIDX; ++x) {
		grid[x] = malloc(sizeof(Box) * GRIDY);
		memset(grid[x], 0, sizeof(Box) * GRIDY);
	}
	printf("Adding freeGrid to atexit()\n");
	atexit(freeGrid);

	// picks random x,y values until found box that is not bomb, then sets it to bomb until NUMBOMBS has been reached
	for(uint i = 0; i < NUMBOMBS; ++i){
		uint x = 0, y = 0;
		do {
			x = rand() % GRIDX;
			y = rand() % GRIDY;
		} while(grid[x][y].bomb);
		grid[x][y].bomb = true;
	}

	printGrid();

	for(uint y = 0; y < GRIDY; y++){
		for(uint x = 0; x < GRIDX; x++){
			grid[x][y].number = getAdjacent(x,y);
		}
	}
}

void loose()
{
	printf("You lost x_x\n");
	exit(0);
}

void clearAdjacentNoNums(uint x, uint y)
{
	if(y>0)
		grid[x][y-1].clear |= !grid[x][y-1].bomb && !grid[x][y-1].number;
	if(y<GRIDY-1)
		grid[x][y+1].clear |= !grid[x][y+1].bomb && !grid[x][y+1].number;
	if(x>0)
		grid[x-1][y].clear |= !grid[x-1][y].bomb && !grid[x-1][y].number;
	if(x<GRIDX-1)
		grid[x+1][y].clear |= !grid[x+1][y].bomb && !grid[x+1][y].number;
}

// returns true if box with no number is cleared
bool clearAround(uint x, uint y)
{
	bool ret = false;
	for(uint yoff = y>0 ? y-1 : y; yoff < GRIDY && yoff <= y+1; yoff++){
		for(uint xoff = x>0 ? x-1 : x; xoff < GRIDX && xoff <= x+1; xoff++){
			ret |=  grid[xoff][yoff].number == 0 && !grid[xoff][yoff].clear;
			grid[xoff][yoff].clear |= grid[xoff][yoff].mark != MARK_FLAG;
		}
	}
	return ret;
}

void fill()
{
	uint clears;
	do{
		clears = 0;
		for(uint y = 0; y < GRIDY; y++){
			for(uint x = 0; x < GRIDX; x++){
				if(grid[x][y].clear && !grid[x][y].bomb && !grid[x][y].number){
					clears += clearAround(x, y);
				}
			}
		}
	}while(clears);
}

bool win()
{
	for(uint x = 0; x < GRIDX; x++){
		for(uint y = 0; y < GRIDY; y++){
			if(grid[x][y].bomb && grid[x][y].mark != MARK_FLAG)
				return false;
			if(grid[x][y].mark == MARK_FLAG && !grid[x][y].bomb)
				return false;
		}
	}
}

void clickL(uint xpos, uint ypos)
{
	uint x = xpos/SCALE;
	uint y = ypos/SCALE;
	if(x >= GRIDX || y >= GRIDY)
		return;
	if(grid[x][y].clear || grid[x][y].mark == MARK_FLAG)
		return;
	if(grid[x][y].bomb)
		loose();
	grid[x][y].clear = true;
	clearAdjacentNoNums(x, y);
	fill();
	if(win())
		printf("You win!\n");
}

void clickR(uint xpos, uint ypos)
{
	uint x = xpos/SCALE;
	uint y = ypos/SCALE;
	if(x >= GRIDX || y >= GRIDY)
		return;
	if(grid[x][y].clear)
		return;
	grid[x][y].mark = (grid[x][y].mark+1)%3;
}

int main(int argc, char *argv[])
{
	switch (argc) {
	case 4:
		GRIDX = strToInt(argv[1]);
		GRIDY = strToInt(argv[2]);
		NUMBOMBS = strToInt(argv[3]);
		break;
	case 3:
		GRIDX = strToInt(argv[1]);
		GRIDY = strToInt(argv[2]);
		NUMBOMBS = NUMBOMBS_DEFAULT;
		break;
	case 2:
		GRIDX = GRIDX_DEFAULT;
		GRIDY = GRIDY_DEFAULT;
		NUMBOMBS = strToInt(argv[1]);
		break;
	case 1:
	default:
		GRIDX = GRIDX_DEFAULT;
		GRIDY = GRIDY_DEFAULT;
		NUMBOMBS = NUMBOMBS_DEFAULT;
		break;
	}
	gfx_init(GRIDX*SCALE, GRIDY*SCALE+SCALE);
	initGrid();
	printGrid();
	putc('\n', stdout);
	printNumbers();
	while(1) {
		events();
		drawGrid();
		draw();
	}
	return 0;
}
