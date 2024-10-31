#pragma once
#include <SFML/Graphics.hpp>

#include "common.h"
#include "display.h"
#include "game_logic.h"

class Engine {
public:
	sf::RenderWindow& GetWindow();

	void LoadResources();
	void CreateWindow();
	void InitializeLogic();
	void InitializeGraphics();
	void UserInput();
	void Render();
	
private:
	sf::Font standard_font_;
	sf::RenderWindow window_;
	std::shared_ptr<GameLogic> game_logic_;
	std::shared_ptr<Display> display_;
	bool game_over_ = false;
	Cell winner_ = Cell::kEmpty;
	int outcome_counter_[3] = { 0, 0, 0 };
};
