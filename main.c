#include "Includes.h"

#define SCALE			48

#define GRIDX_DEFAULT		30
#define GRIDY_DEFAULT		16
#define NUMBOMBS_DEFAULT	99

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

void printGrid(void)
{
	for(uint y = 0; y < GRIDY; ++y){
		for(uint x = 0; x < GRIDX; ++x){
			putc(grid[x][y].bomb ? 'B' : '-', stdout);
			putc(' ', stdout);
		}
		putc('\n', stdout);
	}
	putc('\n', stdout);
}

void initGrid(void)
{
	grid = malloc(sizeof(Box*) * GRIDX);
	for(uint x = 0; x < GRIDX; ++x) {
		grid[x] = malloc(sizeof(Box) * GRIDY);
	}
	printf("Adding freeGrid to atexit()\n");
	atexit(freeGrid);
	for(uint y = 0; y < GRIDY; y++){ // set default values
		for(uint x = 0; x < GRIDX; x++){
			grid[x][y].bomb = false;
			grid[x][y].clear = false;
			grid[x][y].mark = MARK_NONE;
			grid[x][y].number = 0;
		}
	}
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

void printNumbers(void)
{
	for(uint y = 0; y < GRIDY; ++y){
		for(uint x = 0; x < GRIDX; ++x){
			if(grid[x][y].bomb){
				putc('B', stdout);
				goto escape;
			}
			if(grid[x][y].number != 0){
				putc(intToChar(grid[x][y].number), stdout);
				goto escape;
			}
			putc('-', stdout);
			escape:
			putc(' ', stdout);
		}
		putc('\n', stdout);
	}
}

void drawBox(uint x, uint y)
{
	if(grid[x][y].clear){
		setColor(GREY2);
		fillSquare(x*SCALE, y*SCALE, SCALE);
		setColor(GREY3);
		drawVLine(x*SCALE, y*SCALE, SCALE);
		drawHLine(x*SCALE, y*SCALE, SCALE);
		return;
	}
	setColor(GREY2);
	fillSquare(x*SCALE, y*SCALE, SCALE);

	setColor(GREY3);
	drawHLine(x*SCALE,		(y+1)*SCALE-1,	SCALE-1);
	drawVLine((x+1)*SCALE-1,	y*SCALE, 		SCALE-1);
	drawHLine(x*SCALE+1,		(y+1)*SCALE-2,	SCALE-2);
	drawVLine((x+1)*SCALE-2,	y*SCALE+1, 		SCALE-2);

	setColor(GREY1);
	drawVLine(x*SCALE, y*SCALE, SCALE);
	drawHLine(x*SCALE, y*SCALE, SCALE);
	drawVLine(x*SCALE+1, y*SCALE+1, SCALE-2);
	drawHLine(x*SCALE+1, y*SCALE+1, SCALE-2);
}

void drawGrid()
{
	for(uint y = 0; y < GRIDY; ++y){
		for(uint x = 0; x < GRIDX; ++x){
			drawBox(x, y);
		}
	}
}

void loose()
{
	printf("You lost x_x\n");
	exit(0);
}

void onClick(uint xpos, uint ypos)
{
	uint x = xpos/SCALE;
	uint y = ypos/SCALE;
	if(x >= GRIDX || y >= GRIDY)
		return;
	if(grid[x][y].clear || grid[x][y].mark == MARK_FLAG)
		return;
	if(grid[x][y].bomb)
		loose();
	else
		grid[x][y].clear = true;
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
		drawGrid();
		draw();
		events();
	}
	return 0;
}
