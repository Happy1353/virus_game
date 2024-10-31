#include "engine.h"

#include <cassert>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "common.h"

sf::RenderWindow& Engine::GetWindow() {
    return window_;
}

void Engine::LoadResources() {
    // If you're working on Linux put path to standard font:
    if (!standard_font_.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        std::cout << "Error loading standard font" << std::endl;
    }
}

void Engine::CreateWindow() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    const unsigned int ww = 1300u;
    const unsigned int wh = 1000u;
    window_.create({ ww, wh }, "CMake SFML Project", sf::Style::Titlebar | sf::Style::Close, settings);
    window_.setFramerateLimit(100);
}

void Engine::InitializeLogic() {
    assert(!game_logic_);

    game_logic_ = std::make_shared<GameLogic>(10, 10, 5, Cell::kCross);
}

void Engine::InitializeGraphics() {
    assert(!display_);
    assert(game_logic_);

    DisplaySettings settings;
    settings.board_width = game_logic_->GetWidth();
    settings.board_height = game_logic_->GetHeight();
    settings.board_position = sf::Vector2f{ 250.f, 100.f };
    settings.cell_size = 80.f;
    settings.cell_padding = 5.f;
    settings.shape_thickness = 5.f;
    settings.board_color = sf::Color::Black;
    settings.cross_color = sf::Color::Red;
    settings.zero_color = sf::Color::Blue;

    display_ = std::make_shared<Display>(window_, settings);
}

void Engine::UserInput() {
    for (auto event = sf::Event{}; window_.pollEvent(event);) {
        if (event.type == sf::Event::Closed) {
            window_.close();
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if(event.mouseButton.button == sf::Mouse::Left && !game_over_) {
                sf::Vector2i pixel(event.mouseButton.x, event.mouseButton.y);
                std::pair<size_t, size_t> mcoord;
                if (display_->MapPixelToMatrixCoords(pixel, mcoord) &&
                    game_logic_->GetCell(mcoord.first, mcoord.second) == Cell::kEmpty) {
                    game_logic_->MakeTurn(mcoord.first, mcoord.second);
                    game_over_ = game_logic_->TestVictoryConditions(winner_);
                    if (game_over_) {
                        outcome_counter_[(size_t)winner_]++;
                    }
                }
            }
            if (event.mouseButton.button == sf::Mouse::Right && game_over_) {
                game_over_ = false;
                game_logic_->ResetGame();
            }
        }
    }
}


void Engine::Render() {
    window_.clear(sf::Color::White);

    if (game_over_) {
        sf::Text game_over_text;
        game_over_text.setFont(standard_font_);
        game_over_text.setCharacterSize(60);
        game_over_text.setFillColor(sf::Color::Black);
        game_over_text.setStyle(sf::Text::Bold);
        game_over_text.setPosition(sf::Vector2f{ 250.f, 10.f });

        switch(winner_) {
        case Cell::kCross:
            game_over_text.setString("Crosses Win");
            break;
        case Cell::kZero:
            game_over_text.setString("Zeroes Win");
            break;
        case Cell::kEmpty:
            game_over_text.setString("Stalemate");
            break;
        }

        window_.draw(game_over_text);

        game_over_text.setCharacterSize(30);
        game_over_text.setPosition(sf::Vector2f{ 410.f, 930.f });
        game_over_text.setString("Press right mouse button to continue...");

        window_.draw(game_over_text);
    }

    sf::Text counter_text;
    counter_text.setFont(standard_font_);
    counter_text.setCharacterSize(60);
    counter_text.setFillColor(sf::Color::Black);
    counter_text.setStyle(sf::Text::Bold);
    counter_text.setPosition(sf::Vector2f{ 1050.f, 10.f });
    counter_text.setString(
        std::to_string(outcome_counter_[0]) + " / " +
        std::to_string(outcome_counter_[1]) + " / " +
        std::to_string(outcome_counter_[2]));
    window_.draw(counter_text);

    display_->DrawShape(game_logic_->WhichTurn(), sf::Vector2f{ 10.f, 10.f });

    display_->DrawBoard();
    for (size_t i = 0; i < game_logic_->GetHeight(); ++i) {
    	for (size_t j = 0; j < game_logic_->GetWidth(); ++j) {
    		display_->DrawShapeInCell(game_logic_->GetCell(i,j), i, j);
    	}
    }

    window_.display();
}
