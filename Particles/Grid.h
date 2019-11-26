#pragma once
#include "Cell.h"
#include "raymath.h"
#include <vector>

class Grid
{
public:	
	Grid(const int& x, const int& y, const float& cellSize, const Vector3& position);
	std::vector<std::vector<Cell>> grid;

	void render();
	Coords getCoordAtPosition(Vector3 clickPos);
	void onLeftClick(const Coords& coords);
	void onRightClick(const Coords& coords);
	bool inGrid(const int& x, const int& y);
	bool inGrid(const Coords& coords);
	float calculateH(Cell* start, Cell* destination);
	float calculateDistance(Cell* start, Cell* destination);
	std::vector<Cell*> getNeighbor(const Coords& coords);
	std::vector<Cell*> reconstructPath(Cell* cell);
	std::vector<Cell*> aStar(Coords& start, Coords& end);
	bool isCellValid(const Coords& coords);
	bool isCellValid(const int& xPos, const int& yPos);
	void reset();
	Cell* getCell(const Coords& coords);

private:
	Vector3 m_position;
	int m_sizeX;
	int m_sizeY;
	float m_cellSize;
};

