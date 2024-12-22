#pragma once

enum class Cell
{
	kEmpty = 0,
	kCross,
	kZero,
	kCrossDead,
	kZeroDead
};

enum class GameState
{
	Menu,
	Playing,
	GameOver
};

enum class GameType
{
	Local = 0,
	Computer,
	Online,
	Unknown
};