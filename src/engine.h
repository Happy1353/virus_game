#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "common.h"
#include "display.h"
#include "game_logic.h"

class Engine
{
public:
	sf::RenderWindow &GetWindow();

	void LoadResources();
	void CreateWindow();
	void InitializeLogic();
	void InitializeGraphics();
	void UserInput();
	void UserInputMenu(sf::Event event);
	void Render();

private:
	sf::Font standard_font_;
	sf::SoundBuffer sbuf_cross_click_;
	sf::SoundBuffer sbuf_zero_click_;
	sf::Sound sound_cross_click_;
	sf::Sound sound_zero_click_;
	sf::RenderWindow window_;
	std::shared_ptr<GameLogic> game_logic_;
	std::shared_ptr<Display> display_;
	bool game_over_ = false;
	Cell winner_ = Cell::kEmpty;
	GameState game_state_ = GameState::Menu;
	int outcome_counter_[3] = {0, 0, 0};
};
