#pragma once

#include "raylib.h"
#include <limits>

typedef struct Coords
{
	int x;
	int y;
};

class Cell
{
public:
	Cell(const Coords& coords, const float& x, const float& y, const float& width);
	Coords coords;
	float x;
	float y;
	float width;
	float gCost = std::numeric_limits<float>::max();
	float hCost = std::numeric_limits<float>::max();
	float fCost = std::numeric_limits<float>::max();
	Cell* cameFrom = nullptr;
	bool obstacle = false;
	Color cellColor = GRAY;
	
	void render();
	void onLeftClick();
	void onRightClick();
	void setColor(const Color& color);
};

inline bool operator < (const Cell& l, const Cell& r)
{
	return l.fCost < r.fCost;
}

inline bool operator == (const Cell& l, const Cell& r)
{
	return l.x == r.x && l.y == r.y;
}
