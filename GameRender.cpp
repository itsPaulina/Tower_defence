#include "Game.h"

void Game::render() {
    window_.clear(sf::Color(20, 20, 20));

    float mapWidth = static_cast<float>(window_.getSize().x) - menuWidth_;
    float mapHeight = static_cast<float>(window_.getSize().y);

    if (grassTexture_.getSize().x > 0 && grassTexture_.getSize().y > 0) {
        sf::Sprite grassSprite(grassTexture_);
        grassSprite.setTextureRect(sf::IntRect(
            {0, 0},
            {static_cast<int>(mapWidth), static_cast<int>(mapHeight)}
        ));
        grassSprite.setColor(sf::Color(170, 170, 170));
        grassSprite.setPosition({0.f, 0.f});
        window_.draw(grassSprite);
    } else {
        sf::RectangleShape fallback({mapWidth, mapHeight});
        fallback.setPosition({0.f, 0.f});
        fallback.setFillColor(sf::Color(60, 120, 60));
        window_.draw(fallback);
    }

    for (int r = 0; r < rows_; ++r) {
        for (int c = 0; c < cols_; ++c) {
            if (!isPathTile(c, r)) {
                continue;
            }

            sf::Vector2f pos{
                c * tileSize_,
                r * tileSize_
            };

            if (dirtTexture_.getSize().x > 0 && dirtTexture_.getSize().y > 0) {
                sf::Sprite dirtSprite(dirtTexture_);
                dirtSprite.setTextureRect(sf::IntRect(
                    {0, 0},
                    {static_cast<int>(tileSize_), static_cast<int>(tileSize_)}
                ));
                dirtSprite.setPosition(pos);
                window_.draw(dirtSprite);
            } else {
                sf::RectangleShape fallback({tileSize_, tileSize_});
                fallback.setPosition(pos);
                fallback.setFillColor(sf::Color(160, 110, 60));
                window_.draw(fallback);
            }
        }
    }

    for (const auto& obj : objects_) {
        if (obj->isActive()) {
            obj->draw(window_);
        }
    }

    towerMenu_.draw(window_);
    // drawUI();

    window_.display();
}

void Game::drawUI() {
    sf::RectangleShape bar({1280.f, 40.f});
    bar.setFillColor(sf::Color(0, 0, 0, 180));
    bar.setPosition({0.f, 0.f});
    window_.draw(bar);
}