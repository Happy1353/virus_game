#pragma once
#include <vector>

#include "common.h"

class GameLogic
{
public:
	GameLogic(size_t width, size_t height, int goal, Cell first_turn);
	size_t GetWidth() const;
	size_t GetHeight() const;
	Cell GetCell(size_t x, size_t y) const;
	void PutCell(size_t x, size_t y, Cell e);
	void MakeTurn(size_t x, size_t y);
	Cell WhichTurn() const;
	bool TestVictoryConditions(Cell &outcome) const;
	void ResetGame();

private:
	bool TestRow(Cell &outcome, size_t i, size_t j) const;
	bool TestColumn(Cell &outcome, size_t i, size_t j) const;
	bool TestRightDiagonal(Cell &outcome, size_t i, size_t j) const;
	bool TestLeftDiagonal(Cell &outcome, size_t i, size_t j) const;

private:
	std::vector<std::vector<Cell>> matrix_;
	size_t width_;
	size_t height_;
	int victory_goal_;
	Cell first_turn_;
	bool turn_;
};