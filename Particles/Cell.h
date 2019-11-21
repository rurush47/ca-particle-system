#pragma once

#include "raylib.h"

class Cell
{
public:
	Cell(const float& x, const float& y, const float& width);
	float x;
	float y;
	float width;
	
	void render();
};

