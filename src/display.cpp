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

void Display::DrawMenu()
{
	sf::Vector2u window_size = window_.getSize();

	float button_width = 300.f;
	float button_height = 50.f;
	float button_spacing = 20.f;

	float total_height = (button_height + button_spacing) * 3 - button_spacing;
	sf::Vector2f button_position(
		(window_size.x - button_width) / 2.f,
		(window_size.y - total_height) / 2.f);

	sf::Color button_color = sf::Color(100, 100, 200);
	sf::Color text_color = sf::Color::White;

	sf::Font font;
	if (!font.loadFromFile("../res/arial.ttf"))
	{
		std::cerr << "Failed to load font!" << std::endl;
		return;
	}

	std::vector<std::string> button_texts = {
		"Play local ",
		"Play with computer",
		"Play online"};

	buttons_.clear();

	for (size_t i = 0; i < button_texts.size(); ++i)
	{
		sf::RectangleShape button(sf::Vector2f(button_width, button_height));
		button.setPosition(button_position);
		button.setFillColor(button_color);

		sf::Text button_label;
		button_label.setFont(font);
		button_label.setString(button_texts[i]);
		button_label.setCharacterSize(20);
		button_label.setFillColor(text_color);

		sf::FloatRect text_bounds = button_label.getLocalBounds();
		button_label.setOrigin(text_bounds.left + text_bounds.width / 2.0f,
							   text_bounds.top + text_bounds.height / 2.0f);
		button_label.setPosition(
			button_position.x + button_width / 2.f,
			button_position.y + button_height / 2.f);

		window_.draw(button);
		window_.draw(button_label);

		buttons_.emplace_back(button.getGlobalBounds());

		button_position.y += button_height + button_spacing;
	}
}

GameType Display::DetectButtonClick(sf::Vector2i pixel) const
{
	sf::Vector2f world_pos = window_.mapPixelToCoords(pixel);

	for (size_t i = 0; i < buttons_.size(); ++i)
	{
		if (buttons_[i].contains(world_pos))
		{
			switch (i)
			{
			case 0:
				return GameType::Local;
			case 1:
				return GameType::Computer;
			case 2:
				return GameType::Online;
			default:
				break;
			}
		}
	}

	return GameType::Unknown;
}
