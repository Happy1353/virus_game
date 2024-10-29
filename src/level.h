#pragma once
#include<vector>
#include<cmath>

#include <SFML/Graphics.hpp>

enum class Cell {
	kEmpty = 0, kCross, kZero
};

class Level {
public:
	Level(size_t width, size_t height);
	Cell Get(size_t x, size_t y);
	void Put(size_t x, size_t y, Cell e);
	void Render(sf::RenderWindow& window) const;
	std::pair<int, int> MapPixelToMatrixCoords(sf::RenderWindow& window, sf::Vector2i pixel) const;
	bool TestVictory(Cell symbol) const;
	bool TestDraw() const;

public:
	static void DrawCross(sf::RenderWindow& window, const sf::Vector2f position);
	static void DrawZero(sf::RenderWindow& window, const sf::Vector2f position);

private:
	void DrawShapeInCell(sf::RenderWindow& window, Cell shape, size_t i, size_t j) const;
	bool TestRow(Cell symbol, size_t i, size_t j) const;
	bool TestColumn(Cell symbol, size_t i, size_t j) const;
	bool TestRightDiagonal(Cell symbol, size_t i, size_t j) const;
	bool TestLeftDiagonal(Cell symbol, size_t i, size_t j) const;

public:
	static const sf::Vector2f board_pos_;
	static const float cell_size_;
	static const float cell_padding_;
	static const float shape_thickness_;

private:
	std::vector<std::vector<Cell>> matrix_;
	size_t width_;
	size_t height_;

	std::vector<sf::Vertex> board_border_{};
	std::vector<sf::Vertex> board_rows_{};
	std::vector<sf::Vertex> board_columns_{};
	
	int victory_goal_ = 5;
};
