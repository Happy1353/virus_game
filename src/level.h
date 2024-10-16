#pragma once
#include<vector>

#include <SFML/Graphics.hpp>

enum class Cell {
	kEmpty = 0, kCross, kZero
};

class Level {
public:
	Level(size_t width, size_t height)
		: width_{ width }, height_{ height } {
		matrix_.resize(width);
		for (auto &row : matrix_) {
			row.resize(height);
		}

		sf::Vector2f point_first = board_top_left;
		sf::Vector2f point_second = board_top_left + sf::Vector2f{ 0.f, cell_height*height };
		board_columns.push_back(point_first);
		board_columns.push_back(point_second);

		for (size_t i = 0; i < width; ++i) {
			point_first += sf::Vector2f{ cell_width, 0.f };
			point_second += sf::Vector2f{ cell_width, 0.f };
			board_columns.push_back(point_first);
			board_columns.push_back(point_second);
		}

		point_first = board_top_left;
		point_second = board_top_left + sf::Vector2f{ cell_width * width, 0.f };
		board_rows.push_back(point_first);
		board_rows.push_back(point_second);

		for (size_t i = 0; i < height; ++i) {
			point_first += sf::Vector2f{ 0.f, cell_height };
			point_second += sf::Vector2f{ 0.f, cell_height };
			board_rows.push_back(point_first);
			board_rows.push_back(point_second);
		}

	}

	Cell Get(size_t x, size_t y) const {
		return matrix_[x][y];
	}

	void Put(size_t x, size_t y, Cell e) {
		matrix_[x][y] = e;
	}

	size_t Width() const {
		return width_;
	}

	size_t Height() const {
		return height_;
	}

	void Render(sf::RenderWindow &window) const {
		window.draw(&board_columns[0], board_columns.size(), sf::Lines);
		window.draw(&board_rows[0], board_rows.size(), sf::Lines);
	}

private:
	std::vector<std::vector<Cell>> matrix_;
	size_t width_;
	size_t height_;

	const float cell_width = 80.f;
	const float cell_height = 80.f;
	const sf::Vector2f board_top_left{ 500.f, 100.f };
	std::vector<sf::Vertex> board_border{};
	std::vector<sf::Vertex> board_rows{};
	std::vector<sf::Vertex> board_columns{};
};
