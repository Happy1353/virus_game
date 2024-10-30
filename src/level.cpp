#include "level.h"

#include <iostream>

#include <SFML/Graphics.hpp>

const sf::Vector2f Level::board_pos_{ 250.f, 100.f };
const float Level::cell_size_ = 80.f;
const float Level::cell_padding_ = 5.f;
const float Level::shape_thickness_ = 5.f;

Level::Level(size_t width, size_t height) : width_{ width }, height_{ height } {
	matrix_.resize(width);
	for (auto& row : matrix_) {
		row.resize(height);
	}

	sf::Vector2f point_first = board_pos_;
	sf::Vector2f point_second = board_pos_ + sf::Vector2f{ 0.f, cell_size_ * height };
	board_columns_.push_back(point_first);
	board_columns_.back().color = sf::Color::Black;
	board_columns_.push_back(point_second);
	board_columns_.back().color = sf::Color::Black;

	for (size_t i = 0; i < width; ++i) {
		point_first += sf::Vector2f{ cell_size_, 0.f };
		point_second += sf::Vector2f{ cell_size_, 0.f };
		board_columns_.push_back(point_first);
		board_columns_.back().color = sf::Color::Black;
		board_columns_.push_back(point_second);
		board_columns_.back().color = sf::Color::Black;
	}

	point_first = board_pos_;
	point_second = board_pos_ + sf::Vector2f{ cell_size_ * width, 0.f };
	board_rows_.push_back(point_first);
	board_rows_.back().color = sf::Color::Black;
	board_rows_.push_back(point_second);
	board_rows_.back().color = sf::Color::Black;

	for (size_t i = 0; i < height; ++i) {
		point_first += sf::Vector2f{ 0.f, cell_size_ };
		point_second += sf::Vector2f{ 0.f, cell_size_ };
		board_rows_.push_back(point_first);
		board_rows_.back().color = sf::Color::Black;
		board_rows_.push_back(point_second);
		board_rows_.back().color = sf::Color::Black;
	}
}

Cell Level::Get(size_t x, size_t y) {
	if (x >= width_)
		return Cell::kEmpty;

	if (y >= height_)
		return Cell::kEmpty;

	return matrix_[x][y];
}

void Level::Put(size_t x, size_t y, Cell e) {
	if (x >= width_)
		return;

	if (y >= height_)
		return;

	matrix_[x][y] = e;
}

void Level::Render(sf::RenderWindow& window) const {
	window.draw(&board_columns_[0], board_columns_.size(), sf::Lines);
	window.draw(&board_rows_[0], board_rows_.size(), sf::Lines);

	for (size_t i = 0; i < width_; ++i) {
		for (size_t j = 0; j < width_; ++j) {
			DrawShapeInCell(window, matrix_[i][j], i, j);
		}
	}
}

std::pair<int, int> Level::MapPixelToMatrixCoords(sf::RenderWindow& window, sf::Vector2i pixel) const {
	sf::Vector2f coord = window.mapPixelToCoords(pixel) - board_pos_;
	int i = std::floor(coord.x / cell_size_);
	int j = std::floor(coord.y / cell_size_);

	if (i < 0 || i >= width_) {
		i = -1;
	}

	if (j < 0 || j >= height_) {
		j = -1;
	}

	return { i, j };
}

bool Level::TestVictory(Cell symbol) const {
	for (size_t i = 0; i < width_; ++i) {
		for (size_t j = 0; j < width_; ++j) {
			if (TestRow(symbol, i, j))
				return true;
			if (TestColumn(symbol, i, j))
				return true;
			if (TestRightDiagonal(symbol, i, j))
				return true;
			if (TestLeftDiagonal(symbol, i, j))
				return true;
		}
	}
	return false;
}

bool Level::TestDraw() const {
	for (size_t i = 0; i < width_; ++i) {
		for (size_t j = 0; j < width_; ++j) {
			if (matrix_[i][j] == Cell::kEmpty)
				return false;
		}
	}
	return true;
}

void Level::DrawCross(sf::RenderWindow& window, const sf::Vector2f position) {
	static const float dlen = std::sqrtf(2) * (cell_size_ - 2 * cell_padding_);
	static const sf::Vector2f shift{ cell_size_ - 2 * cell_padding_ , 0 };

	sf::RectangleShape r1(sf::Vector2f(shape_thickness_, dlen));
	r1.setOrigin(shape_thickness_ / 2, 0);
	r1.setPosition(position);
	r1.setRotation(-45.f);
	r1.setFillColor(sf::Color::Red);

	sf::RectangleShape r2(sf::Vector2f(4.f, 100.f));
	r2.setOrigin(shape_thickness_ / 2, 0);
	r2.setPosition(position + shift);
	r2.setRotation(45.f);
	r2.setFillColor(sf::Color::Red);

	window.draw(r1);
	window.draw(r2);
}

void Level::DrawZero(sf::RenderWindow& window, const sf::Vector2f position) {
	sf::CircleShape z(cell_size_ / 2 - cell_padding_);
	z.setPosition(position);
	z.setOutlineThickness(-shape_thickness_);
	z.setFillColor(sf::Color::Transparent);
	z.setOutlineColor(sf::Color::Blue);
	window.draw(z);
}

void Level::DrawShapeInCell(sf::RenderWindow& window, Cell shape, size_t i, size_t j) const {
	sf::Vector2f matrix_pos{ i * cell_size_ + cell_padding_, j * cell_size_ + cell_padding_ };
	switch (shape) {
	case Cell::kCross:
		DrawCross(window, board_pos_ + matrix_pos);
		break;
	case Cell::kZero:
		DrawZero(window, board_pos_ + matrix_pos);
		break;
	}
}

bool Level::TestRow(Cell symbol, size_t i, size_t j) const {
	for (size_t k = 0; k < victory_goal_; ++k) {
		if (i + k >= width_)
			return false;

		if (matrix_[i + k][j] != symbol)
			return false;
	}
	return true;
}

bool Level::TestColumn(Cell symbol, size_t i, size_t j) const {
	for (size_t k = 0; k < victory_goal_; ++k) {
		if (j + k >= height_)
			return false;

		if (matrix_[i][j + k] != symbol)
			return false;
	}
	return true;
}

bool Level::TestRightDiagonal(Cell symbol, size_t i, size_t j) const {
	for (size_t k = 0; k < victory_goal_; ++k) {
		if (i + k >= width_ || j + k >= height_)
			return false;

		if (matrix_[i + k][j + k] != symbol)
			return false;
	}
	return true;
}

bool Level::TestLeftDiagonal(Cell symbol, size_t i, size_t j) const {
	if (j >= height_)
		return false;

	for (size_t k = 0; k < victory_goal_; ++k) {
		if (i + k >= width_ || j < k)
			return false;

		if (matrix_[i + k][j - k] != symbol)
			return false;
	}
	return true;
}
