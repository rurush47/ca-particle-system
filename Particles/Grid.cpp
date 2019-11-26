#include "Grid.h"
#include <algorithm>

Grid::Grid(const int& x, const int& y, const float& cellSize, const Vector3& position)
{
	m_sizeX = x;
	m_sizeY = y;
	
	m_cellSize = cellSize;
	m_position = Vector3Add(position, Vector3{ cellSize, 0, cellSize });
	
	for (int i = 0; i < x; i++)
	{
		std::vector<Cell> row;
		grid.push_back(row);
		
		for (int j = 0; j < x; j++)
		{
			Coords coords{ i, j };
			grid[i].emplace_back(coords, m_position.x + i*m_cellSize, m_position.z + j * m_cellSize, cellSize);
		}
	}
}

void Grid::render()
{
	for (std::vector<Cell>& cells : grid)
	{
		for (Cell& cell : cells)
		{
			cell.render();
		}
	}
}

Coords Grid::getCoordAtPosition(Vector3 clickPos)
{
	const int xCoord = static_cast<int>(abs(clickPos.x - m_position.x) / m_cellSize);
	const int yCoord = static_cast<int>(abs(clickPos.z - m_position.z) / m_cellSize);
	return Coords{ xCoord, yCoord};
}

void Grid::onLeftClick(const Coords& coords)
{
	if(inGrid(coords))
	{
		grid[coords.x][coords.y].onLeftClick();
	}
}

void Grid::onRightClick(const Coords& coords)
{
	if (inGrid(coords))
	{
		grid[coords.x][coords.y].onRightClick();
	}
}

bool Grid::inGrid(const int& x, const int& y)
{
	return x >= 0 &&
		x < m_sizeX &&
		y >= 0 &&
		y < m_sizeY;
}


bool Grid::inGrid(const Coords& coords)
{
	return inGrid(coords.x, coords.y);
}

float Grid::calculateH(Cell* start, Cell* destination)
{
	return sqrt(
		(start->x - destination->x) * (start->x - destination->x) + 
		(start->y - destination->y) * ((start->y - destination->y)));
}

float Grid::calculateDistance(Cell* start, Cell* destination)
{
	return sqrt(
		(start->x - destination->x) * (start->x - destination->x) +
		(start->y - destination->y) * ((start->y - destination->y)));
}

std::vector<Cell*> Grid::getNeighbor(const Coords& coords)
{
	std::vector<Cell*> neighbors;
	
	for (int k = -1; k < 2; ++k)
	{
		for (int j = -1; j < 2; ++j)
		{
			if(k == 0 && j == 0)
			{
				continue;
			}
			
			Coords nCoords{ k + coords.x, j + coords.y };
			if(isCellValid(nCoords))
			{
				neighbors.push_back(&grid[nCoords.x][nCoords.y]);
			}
		}
	}

	return neighbors;
}

std::vector<Cell*> Grid::reconstructPath(Cell* cell)
{
	std::vector<Cell*> path;
	path.push_back(cell);
	
	while (cell->cameFrom != nullptr)
	{
		path.push_back(cell->cameFrom);
		cell = cell->cameFrom;
	}
	
	return path;
}

bool compareCost(Cell* c1, Cell* c2)
{
	return (c1->fCost < c2->fCost);
}

std::vector<Cell*> Grid::aStar(Coords& start, Coords& end)
{
	std::vector<Cell*> emptyVector;
	if (!isCellValid(end.x, end.y) || !isCellValid(start.x, start.y))
	{
		return emptyVector;
	}

	if(end.x == start.x && end.y == start.y)
	{
		return emptyVector;
	}

	Cell* startCell = &grid[start.x][start.y];
	Cell* destinationCell = &grid[end.x][end.y];
	startCell->gCost = 0;
	startCell->fCost = calculateH(startCell, destinationCell);
	
	std::vector<Cell*> openSet;
	openSet.push_back(startCell);
	
	while (!openSet.empty())
	{
		auto current = std::min_element(openSet.begin(), openSet.end(), compareCost);
		
		Cell* currentCell = *current;
		
		if(currentCell == destinationCell)
		{
			return reconstructPath(currentCell);
		}

		openSet.erase(current);

		auto neighbours = getNeighbor(currentCell->coords);
		for (Cell* neighbor : neighbours)
		{
			float tentativeGCost = currentCell->gCost + calculateDistance(currentCell, neighbor);
			if(tentativeGCost < neighbor->gCost)
			{
				neighbor->cameFrom = currentCell;
				neighbor->gCost = tentativeGCost;
				neighbor->fCost = neighbor->gCost + calculateH(neighbor, destinationCell);

				//neighbor node not found
				if (std::find(openSet.begin(), openSet.end(), neighbor) == openSet.end())
				{
					openSet.push_back(neighbor);
				}
			}
		}
	}
	
	return emptyVector;
}

bool Grid::isCellValid(const Coords& coords)
{
	return isCellValid(coords.x, coords.y);
}

bool Grid::isCellValid(const int& xPos, const int& yPos)
{
	return inGrid(xPos, yPos) && !grid[xPos][yPos].obstacle;
}

void Grid::reset()
{
	for (int i = 0; i < m_sizeX; ++i)
	{
		for (int j = 0; j < m_sizeY; ++j)
		{
			Cell* cell = &grid[i][j];
			cell->fCost = std::numeric_limits<float>::max();
			cell->fCost = std::numeric_limits<float>::max();
			cell->fCost = std::numeric_limits<float>::max();
			cell->obstacle = false;
			cell->setColor(GRAY);
		}
	}
}

Cell* Grid::getCell(const Coords& coords)
{
	if(inGrid(coords))
	{
		return &grid[coords.x][coords.y];
	}
	return nullptr;
}
