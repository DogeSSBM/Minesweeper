#pragma once
void drawBox(uint x, uint y)
{
	const static Color numColor[] = {
		{0x00, 0x00, 0xAA, 0xFF},
		{0x00, 0x52, 0x00, 0xFF},
		{0xAA, 0x00, 0x00, 0xFF},
		{0x00, 0x00, 0x52, 0xFF},
		{0x52, 0x00, 0x00, 0xFF},
		{0x00, 0x52, 0x52, 0xFF},
		{0x00, 0x00, 0x00, 0xFF},
		{0x52, 0x52, 0x52, 0xFF},
	};
	char numStr[] = "n";
	if(grid[x][y].clear){
		setColor(GREY2);
		fillSquare(x*SCALE, y*SCALE, SCALE);
		setColor(GREY3);
		drawVLine(x*SCALE, y*SCALE, SCALE);
		drawHLine(x*SCALE, y*SCALE, SCALE);

		if(grid[x][y].number > 0){
			setFontColor(numColor[grid[x][y].number-1]);
			intToStr(grid[x][y].number, numStr);
			drawText(x*SCALE+SCALE/4, y*SCALE+SCALE/10, numStr);
		}

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

	switch (grid[x][y].mark) {
	default:
	case MARK_NONE:
		return;
		break;
	case MARK_FLAG:
		setFontColor(numColor[2]);
		drawText(x*SCALE+SCALE/6, y*SCALE-SCALE/8, ">");
		setFontColor(BLACK);
		drawText(x*SCALE, y*SCALE, "|");
		return;
		break;
	case MARK_QUESTION:
		setFontColor(BLACK);
		drawText(x*SCALE+SCALE/4, y*SCALE+SCALE/10, "?");
		return;
		break;
	}
}

void drawGrid()
{
	for(uint y = 0; y < GRIDY; ++y){
		for(uint x = 0; x < GRIDX; ++x){
			drawBox(x, y);
		}
	}
}
