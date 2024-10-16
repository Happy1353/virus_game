#pragma once
#include <SFML/Graphics.hpp>

#include "level.h"

class Engine {
public:
	sf::RenderWindow& GetWindow();

	void LoadResources();
	void CreateWindow();
	void InitializeLevel();
	void UserInput();
	void Render();
	void Shutdown();

private:
	sf::Font standard_font_;
	sf::RenderWindow window_;
	Level level_{ 10,10 };
};
