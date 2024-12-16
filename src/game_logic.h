#pragma once
#include <vector>
#include <map>

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
	bool MakeFirstAndSecondTurn(Cell isTurn, size_t x, size_t y);
	bool CanEat(Cell isTurn, size_t x, size_t y) const;
	Cell WhichTurn() const;
	Cell WhitchDead(size_t x, size_t y) const;
	bool TestVictoryConditions(Cell node) const;
	bool TestTurnIsOff() const;
	void ResetCurrentAction();
	void ResetGame();

private:
	bool TestRow(Cell &outcome, size_t i, size_t j) const;
	bool TestColumn(Cell &outcome, size_t i, size_t j) const;
	bool TestRightDiagonal(Cell &outcome, size_t i, size_t j) const;
	bool TestLeftDiagonal(Cell &outcome, size_t i, size_t j) const;
	bool TestNearCells(Cell isTurn, size_t i, size_t j, bool testWinner) const;
	bool TestIsCainLive(Cell isTurn, Cell deadNode, size_t i, size_t j, std::map<std::pair<size_t, size_t>, bool> &visited) const;

private:
	std::vector<std::vector<Cell>> matrix_;
	size_t width_;
	size_t height_;
	int victory_goal_;
	Cell first_turn_;
	bool turn_;
	int current_actions_;
	const int actions_per_turn_ = 3;
};