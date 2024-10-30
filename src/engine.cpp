#include "engine.h"

#include <iostream>

#include <SFML/Graphics.hpp>

sf::RenderWindow& Engine::GetWindow() {
    return window_;
}

void Engine::LoadResources() {
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

void Engine::InitializeLevel() {
}

void Engine::UserInput() {
    for (auto event = sf::Event{}; window_.pollEvent(event);) {
        if (event.type == sf::Event::Closed) {
            window_.close();
        }

        if (event.type == sf::Event::KeyPressed) {
        }


        if (event.type == sf::Event::Resized)
        {
            sf::Vector2f new_size_float{ float(event.size.width), float(event.size.height) };
            sf::FloatRect visibleArea(sf::Vector2f{ 0, 0 }, new_size_float);

            sf::Transform trans;
            trans.translate(- new_size_float / 2.0f);
            visibleArea = trans.transformRect(visibleArea);
            //window_.setView(sf::View(visibleArea));
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i pixel(event.mouseButton.x, event.mouseButton.y);
                auto c = level_.MapPixelToMatrixCoords(window_, pixel);
                if (c.first >= 0 && c.second >= 0 &&
                    level_.Get(c.first, c.second) == Cell::kEmpty) {
                   Cell current_symbol = Symbol(turn_);
                   level_.Put(c.first, c.second, current_symbol);
                   if (level_.TestVictory(current_symbol)) {
                       game_over_ = true;
                       winner_ = current_symbol;
                   }
                   if (level_.TestDraw()) {
                       game_over_ = true;
                       winner_ = Cell::kEmpty;
                   }
                   turn_ = !turn_;
                }
            }
        }
    }
}


void Engine::Render() {
    window_.clear(sf::Color::White);

    if (game_over_) {
        sf::Text text_ln1;
        text_ln1.setFont(standard_font_);
        text_ln1.setString("Game Over");
        text_ln1.setCharacterSize(100);
        text_ln1.setFillColor(sf::Color::Black);
        text_ln1.setStyle(sf::Text::Bold);
        text_ln1.setOrigin(225.f, 0.f);
        text_ln1.setPosition(sf::Vector2f{650.f, 300.f});

        sf::Text text_ln2;
        text_ln2.setFont(standard_font_);
        switch(winner_) {
        case Cell::kCross:
            text_ln2.setString("Crosses Win");
            text_ln2.setOrigin(250.f, 0.f);
            break;
        case Cell::kZero:
            text_ln2.setString("Zeroes Win");
            text_ln2.setOrigin(250.f, 0.f);
            break;
        case Cell::kEmpty:
            text_ln2.setString("DRAW");
            text_ln2.setOrigin(100.f, 0.f);
            break;
        }
        text_ln2.setCharacterSize(100);
        text_ln2.setFillColor(sf::Color::Black);
        text_ln2.setStyle(sf::Text::Bold);
        text_ln2.setPosition(sf::Vector2f{ 650.f, 420.f });

        window_.draw(text_ln1);
        window_.draw(text_ln2);
    }
    else {
        level_.Render(window_);
        if (turn_) {
            Level::DrawCross(window_, sf::Vector2f{ 10.f, 10.f });
        }
        else {
            Level::DrawZero(window_, sf::Vector2f{ 1290.f - Level::cell_size_, 10.f });
        }
    }
    window_.display();
}

void Engine::Shutdown() {
}

bool Engine::IsGameOver() const {
    return game_over_;
}

Cell Engine::Symbol(bool turn) {
    return turn ? Cell::kCross : Cell::kZero;
}
