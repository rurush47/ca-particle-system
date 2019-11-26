#include "Cell.h"

Cell::Cell(const Coords& gridCoords, const float& posX, const float& posY, const float& cellWidth)
{
	coords = gridCoords;
	x = posX;
	y = posY;
	width = cellWidth;
}

void Cell::render()
{
	DrawCube(Vector3{x, -width, y}, width, width, width, cellColor);
	DrawCubeWires(Vector3{ x, -width, y }, width, width, width, cellColor);
}

void Cell::onLeftClick()
{
	obstacle = !obstacle;
	cellColor = obstacle ? Color(RED) : Color(GRAY);
}

void Cell::onRightClick()
{
	
}

void Cell::setColor(const Color& color)
{
	cellColor = color;
}
