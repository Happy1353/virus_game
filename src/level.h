#pragma once
#include<vector>

enum class Cell {
	kEmpty = 0, kCross, kZero
};

class Level {
public:
	Level(size_t width, size_t height) {
		matrix_.resize(width);
		for (auto row : matrix_) {
			row.resize(height);
		}
	}

	Cell Get(size_t x, size_t y) {
		return matrix_[x][y];
	}

	void Put(size_t x, size_t y, Cell e) {
		matrix_[x][y] = e;
	}

	size_t Width() {
		return width_;
	}

	size_t Height() {
		return height_;
	}

private:
	std::vector<std::vector<Cell>> matrix_;
	size_t width_;
	size_t height_;
};
