#pragma once
#include "Cell.h"
#include "raymath.h"
#include <vector>

typedef struct Coords
{
	int x;
	int y;
};

class Grid
{
public:	
	Grid(const int& x, const int& y, const float& cellSize, const Vector3& position);
	std::vector<std::vector<Cell>> grid;

	void render();
	Coords getCoordAtPosition(Vector3 clickPos);
	
private:
	Vector3 m_position;
	int m_sizeX;
	int m_sizeY;
	float m_cellSize;
};

