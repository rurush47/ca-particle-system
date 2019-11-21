#include "Cell.h"

Cell::Cell(const float& posX, const float& posY, const float& cellWidth)
{
	x = posX;
	y = posY;
	width = cellWidth;
}

void Cell::render()
{
	DrawCube(Vector3{x, -width, y}, width, width, width, GREEN);
	DrawCubeWires(Vector3{ x, -width, y }, width, width, width, RED);
}
