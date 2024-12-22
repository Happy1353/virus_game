#pragma once
#include <vector>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "common.h"

struct DisplaySettings
{
	size_t board_width;
	size_t board_height;
	sf::Vector2f board_position;
	float cell_size;
	float cell_padding;
	float shape_thickness;
	sf::Color board_color;
	sf::Color cross_color;
	sf::Color zero_color;
};

class Display
{
public:
	Display(sf::RenderWindow &window, DisplaySettings settings);
	const DisplaySettings &ReadSettings() const;
	void DrawBoard() const;
	void DrawMenu(sf::Font font);
	void DrawShapeInCell(Cell shape, size_t i, size_t j) const;
	void DrawShape(Cell shape, sf::Vector2f position) const;
	void DrawCross(sf::Vector2f position) const;
	void DrawZero(sf::Vector2f position) const;
	void DrawCrossDead(sf::Vector2f position) const;
	void DrawZeroDead(sf::Vector2f position) const;
	bool MapPixelToMatrixCoords(sf::Vector2i pixel, std::pair<size_t, size_t> &mcoord) const;
	GameType DetectButtonClick(sf::Vector2i pixel) const;

private:
	sf::RenderWindow &window_;
	DisplaySettings settings_;
	std::vector<sf::Vertex> board_border_;
	std::vector<sf::Vertex> board_rows_;
	std::vector<sf::Vertex> board_columns_;
	mutable sf::RectangleShape cross_rect1_;
	mutable sf::RectangleShape cross_rect2_;
	mutable sf::CircleShape zero_circ_;
	std::vector<sf::FloatRect> buttons_;
};