#pragma once

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
};

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
