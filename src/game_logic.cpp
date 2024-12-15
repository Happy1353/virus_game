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
		PutCell(x, y, isTurn);
		turn_ = !turn_;
	}
}

bool GameLogic::MakeFirstAndSecondTurn(Cell isTurn, size_t x, size_t y)
{
	if (matrix_.at(0).at(0) == Cell::kEmpty || matrix_.at(9).at(9) == Cell::kEmpty)
	{
		if (turn_ && x == 0 && y == 0)
		{
			PutCell(0, 0, isTurn);
			turn_ = !turn_;
		}
		else if (!turn_ && x == 9 && y == 9)
		{
			PutCell(9, 9, isTurn);
			turn_ = !turn_;
		}
		return false;
	}

	return true;
}

Cell GameLogic::WhichTurn() const
{
	return turn_ ? Cell::kCross : Cell::kZero;
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
		}
	}
	return false;
}