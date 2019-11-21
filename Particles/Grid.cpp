#include "Grid.h"

Grid::Grid(const int& x, const int& y, const float& cellSize, const Vector3& position)
{
	m_cellSize = cellSize;
	m_position = Vector3Add(position, Vector3{ cellSize, 0, cellSize });
	
	for (int i = 0; i < x; i++)
	{
		std::vector<Cell> row;
		grid.push_back(row);
		
		for (int j = 0; j < x; j++)
		{
			grid[i].emplace_back(m_position.x + i*m_cellSize, m_position.z + j * m_cellSize, cellSize);
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
