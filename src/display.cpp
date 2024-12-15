#include "display.h"

#include <iostream>

#include <SFML/Graphics.hpp>

#include "common.h"

Display::Display(sf::RenderWindow &window, DisplaySettings settings) : window_{window},
																	   settings_{settings},
																	   cross_rect1_{sf::Vector2f{settings_.shape_thickness, std::sqrtf(2) * (settings_.cell_size - 2 * settings_.cell_padding)}},
																	   cross_rect2_{sf::Vector2f{settings_.shape_thickness, std::sqrtf(2) * (settings_.cell_size - 2 * settings_.cell_padding)}},
																	   zero_circ_{settings_.cell_size / 2 - settings_.cell_padding}
{

	sf::Vector2f point_first = settings_.board_position;
	sf::Vector2f point_second = settings_.board_position + sf::Vector2f{0.f, settings_.cell_size * settings_.board_height};
	board_columns_.push_back(point_first);
	board_columns_.back().color = settings_.board_color;
	board_columns_.push_back(point_second);
	board_columns_.back().color = settings_.board_color;

	for (size_t i = 0; i < settings_.board_width; ++i)
	{
		point_first += sf::Vector2f{settings_.cell_size, 0.f};
		point_second += sf::Vector2f{settings_.cell_size, 0.f};
		board_columns_.push_back(point_first);
		board_columns_.back().color = settings_.board_color;
		board_columns_.push_back(point_second);
		board_columns_.back().color = settings_.board_color;
	}

	point_first = settings_.board_position;
	point_second = settings_.board_position + sf::Vector2f{settings_.cell_size * settings_.board_width, 0.f};
	board_rows_.push_back(point_first);
	board_rows_.back().color = settings_.board_color;
	board_rows_.push_back(point_second);
	board_rows_.back().color = settings_.board_color;

	for (size_t i = 0; i < settings_.board_height; ++i)
	{
		point_first += sf::Vector2f{0.f, settings_.cell_size};
		point_second += sf::Vector2f{0.f, settings_.cell_size};
		board_rows_.push_back(point_first);
		board_rows_.back().color = settings_.board_color;
		board_rows_.push_back(point_second);
		board_rows_.back().color = settings_.board_color;
	}

	cross_rect1_.setOrigin(settings_.shape_thickness / 2, 0);
	cross_rect1_.setRotation(-45.f);
	cross_rect1_.setFillColor(settings_.cross_color);

	cross_rect2_.setOrigin(settings_.shape_thickness / 2, 0);
	cross_rect2_.setRotation(45.f);
	cross_rect2_.setFillColor(settings_.cross_color);

	zero_circ_.setOutlineThickness(-settings_.shape_thickness);
	zero_circ_.setFillColor(sf::Color::Transparent);
	zero_circ_.setOutlineColor(settings_.zero_color);
}

const DisplaySettings &Display::ReadSettings() const
{
	return settings_;
}

void Display::DrawBoard() const
{
	window_.draw(&board_columns_[0], board_columns_.size(), sf::Lines);
	window_.draw(&board_rows_[0], board_rows_.size(), sf::Lines);
}

bool Display::MapPixelToMatrixCoords(sf::Vector2i pixel, std::pair<size_t, size_t> &mcoord) const
{
	sf::Vector2f board_relative_pixel = window_.mapPixelToCoords(pixel) - settings_.board_position;
	int i = std::floor(board_relative_pixel.x / settings_.cell_size);
	int j = std::floor(board_relative_pixel.y / settings_.cell_size);

	if (i < 0 || i >= settings_.board_width ||
		j < 0 || j >= settings_.board_height)
	{
		return false;
	}

	mcoord = {i, j};
	return true;
}

void Display::DrawShapeInCell(Cell shape, size_t i, size_t j) const
{
	sf::Vector2f board_pixel_pos{i * settings_.cell_size + settings_.cell_padding, j * settings_.cell_size + settings_.cell_padding};
	board_pixel_pos += settings_.board_position;
	DrawShape(shape, board_pixel_pos);
}

void Display::DrawShape(Cell shape, sf::Vector2f position) const
{
	switch (shape)
	{
	case Cell::kCross:
		DrawCross(position);
		break;
	case Cell::kZero:
		DrawZero(position);
		break;
	case Cell::kCrossDead:
		DrawCrossDead(position);
		break;
	case Cell::kZeroDead:
		DrawZeroDead(position);
		break;
	}
}

void Display::DrawCross(sf::Vector2f position) const
{
	cross_rect1_.setPosition(position);
	cross_rect2_.setPosition(position + sf::Vector2f{settings_.cell_size - 2 * settings_.cell_padding, 0.f});
	window_.draw(cross_rect1_);
	window_.draw(cross_rect2_);
}

void Display::DrawZero(sf::Vector2f position) const
{
	zero_circ_.setPosition(position);
	window_.draw(zero_circ_);
}

void Display::DrawCrossDead(sf::Vector2f position) const
{
	sf::RectangleShape background(sf::Vector2f(settings_.cell_size - 1, settings_.cell_size - 1));
	background.setPosition(position + sf::Vector2f{-2, -2});
	background.setFillColor(sf::Color::Blue);

	window_.draw(background);

	cross_rect1_.setPosition(position);
	cross_rect2_.setPosition(position + sf::Vector2f{settings_.cell_size - 2 * settings_.cell_padding, 0.f});
	window_.draw(cross_rect1_);
	window_.draw(cross_rect2_);
}

void Display::DrawZeroDead(sf::Vector2f position) const
{
	sf::RectangleShape background(sf::Vector2f(settings_.cell_size - 1, settings_.cell_size - 1));
	background.setPosition(position + sf::Vector2f{-2, -2});
	background.setFillColor(sf::Color::Red);

	window_.draw(background);

	zero_circ_.setPosition(position);
	window_.draw(zero_circ_);
}
