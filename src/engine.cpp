#include <iostream>

#include <SFML/Graphics.hpp>

#include "engine.h"

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

    const unsigned int ww = 1920u;
    const unsigned int wh = 1080u;
    window_.create({ ww, wh }, "CMake SFML Project", sf::Style::Default, settings);
    window_.setFramerateLimit(100);

    sf::View view = window_.getView();
    view.setCenter(sf::Vector2f{ 0.0, 0.0 });
    window_.setView(view);
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
            window_.setView(sf::View(visibleArea));
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i pixel(event.mouseButton.x, event.mouseButton.y);
            }
        }
    }
}


void Engine::Render() {
    window_.clear();
    window_.display();
}

void Engine::Shutdown() {
}