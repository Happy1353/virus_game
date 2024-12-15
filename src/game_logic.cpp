#include <iostream>
#include <cassert>
#include <stdexcept>

#include "game_logic.h"

GameLogic::GameLogic(size_t width, size_t height, int goal, Cell first_turn)
	: width_{width}, height_{height}, victory_goal_{goal}, first_turn_{first_turn}
{
	if (first_turn_ == Cell::kEmpty)
	{
		throw std::invalid_argument{"first turn argument is invalid"};
	}

	matrix_.resize(width_);
	for (auto &row : matrix_)
	{
		row.resize(height_);
	}

	ResetGame();
}

size_t GameLogic::GetWidth() const
{
	return width_;
}
size_t GameLogic::GetHeight() const
{
	return height_;
}

Cell GameLogic::GetCell(size_t x, size_t y) const
{
	return matrix_.at(x).at(y);
}

void GameLogic::PutCell(size_t x, size_t y, Cell e)
{
	matrix_.at(x).at(y) = e;
}

void GameLogic::MakeTurn(size_t x, size_t y)
{
	Cell isTurn = WhichTurn();

	if (MakeFirstAndSecondTurn(isTurn, x, y) && TestNearCells(isTurn, x, y))
	{
		if (matrix_.at(x).at(y) == Cell::kEmpty)
		{
			PutCell(x, y, isTurn);
			current_actions_++;
		}
		else if (CanEat(isTurn, x, y))
		{
			PutCell(x, y, WhitchDead(x, y));
			current_actions_++;
		}
	}

	if (current_actions_ >= actions_per_turn_)
	{
		turn_ = !turn_;
		current_actions_ = 0;
	}
}

bool GameLogic::MakeFirstAndSecondTurn(Cell isTurn, size_t x, size_t y)
{
	if (matrix_.at(0).at(0) == Cell::kEmpty || matrix_.at(9).at(9) == Cell::kEmpty)
	{
		if (turn_ && x == 0 && y == 0)
		{
			PutCell(0, 0, isTurn);
			current_actions_++;
			return false;
		}

		if (!turn_ && x == 9 && y == 9)
		{
			PutCell(9, 9, isTurn);
			current_actions_++;
			return false;
		}
	}

	return true;
}

Cell GameLogic::WhichTurn() const
{
	return turn_ ? Cell::kCross : Cell::kZero;
}

Cell GameLogic::WhitchDead(size_t x, size_t y) const
{
	return matrix_.at(x).at(y) == Cell::kCross ? Cell::kCrossDead : Cell::kZeroDead;
}

bool GameLogic::CanEat(Cell isTurn, size_t x, size_t y) const
{
	if (isTurn == Cell::kCross && matrix_.at(x).at(y) == Cell::kZero)
	{
		return true;
	}
	else if (isTurn == Cell::kZero && matrix_.at(x).at(y) == Cell::kCross)
	{
		return true;
	}
	return false;
}

bool GameLogic::TestVictoryConditions(Cell &outcome) const
{
	bool draw = true;
	for (size_t i = 0; i < width_; ++i)
	{
		for (size_t j = 0; j < width_; ++j)
		{
			if (matrix_[i][j] == Cell::kEmpty)
			{
				draw = false;
				continue;
			}

			if (TestRow(outcome, i, j))
				return true;
			if (TestColumn(outcome, i, j))
				return true;
			if (TestRightDiagonal(outcome, i, j))
				return true;
			if (TestLeftDiagonal(outcome, i, j))
				return true;
		}
	}

	if (draw)
	{
		outcome = Cell::kEmpty;
		return true;
	}

	return false;
}

void GameLogic::ResetGame()
{
	turn_ = first_turn_ == Cell::kCross;
	current_actions_ = 0;

	for (size_t i = 0; i < width_; ++i)
	{
		for (size_t j = 0; j < width_; ++j)
		{
			matrix_[i][j] = Cell::kEmpty;
		}
	}
}

bool GameLogic::TestRow(Cell &outcome, size_t i, size_t j) const
{
	Cell current = matrix_.at(i).at(j);
	assert(current != Cell::kEmpty);

	for (size_t k = 1; k < victory_goal_; ++k)
	{
		if (j + k >= width_)
			return false;

		if (matrix_[i][j + k] != current)
			return false;
	}

	outcome = current;
	return true;
}

bool GameLogic::TestColumn(Cell &outcome, size_t i, size_t j) const
{
	Cell current = matrix_.at(i).at(j);
	assert(current != Cell::kEmpty);

	for (size_t k = 1; k < victory_goal_; ++k)
	{
		if (i + k >= height_)
			return false;

		if (matrix_[i + k][j] != current)
			return false;
	}

	outcome = current;
	return true;
}

bool GameLogic::TestRightDiagonal(Cell &outcome, size_t i, size_t j) const
{
	Cell current = matrix_.at(i).at(j);
	assert(current != Cell::kEmpty);

	for (size_t k = 1; k < victory_goal_; ++k)
	{
		if (i + k >= height_ || j + k >= width_)
			return false;

		if (matrix_[i + k][j + k] != current)
			return false;
	}

	outcome = current;
	return true;
}

bool GameLogic::TestLeftDiagonal(Cell &outcome, size_t i, size_t j) const
{
	Cell current = matrix_.at(i).at(j);
	assert(current != Cell::kEmpty);

	for (size_t k = 1; k < victory_goal_; ++k)
	{
		if (i + k >= height_ || j < k)
			return false;

		if (matrix_[i + k][j - k] != current)
			return false;
	}

	outcome = current;
	return true;
}

// Check if cell in good position
bool GameLogic::TestNearCells(Cell e, size_t x, size_t y) const
{
	for (int dx = -1; dx <= 1; ++dx)
	{
		for (int dy = -1; dy <= 1; ++dy)
		{
			if (dx == 0 && dy == 0)
				continue;

			size_t nx = x + dx;
			size_t ny = y + dy;

			if (nx < width_ && ny < height_ && matrix_[nx][ny] == e)
			{
				return true;
			}

			Cell deadNode = e == Cell::kCross ? Cell::kZeroDead : Cell::kCrossDead;
			if (matrix_[nx][ny] == deadNode)
			{
				std::map<std::pair<size_t, size_t>, bool> visited;

				if (TestIsCainLive(e, deadNode, nx, ny, visited))
				{
					return true;
				}
			}
		}
	}
	return false;
}

// Check if cell near live chain
bool GameLogic::TestIsCainLive(Cell node, Cell deadNode, size_t i, size_t j, std::map<std::pair<size_t, size_t>, bool> &visited) const
{
	if (visited[{i, j}] || i > 9 || j > 9 || i < 0 || j < 0 || (matrix_.at(i).at(j) != node && matrix_.at(i).at(j) != deadNode))
	{
		return false;
	}

	visited[{i, j}] = true;

	if (matrix_[i][j] == node)
	{
		return true;
	}

	static const std::vector<std::pair<int, int>> directions = {
		{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

	for (const auto &[dx, dy] : directions)
	{
		size_t ni = i + dx;
		size_t nj = j + dy;

		if (TestIsCainLive(node, deadNode, ni, nj, visited))
		{
			return true;
		}
	}

	return false;
}
