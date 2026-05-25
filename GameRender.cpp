#include "Game.h"

void Game::render() {
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
    drawUI();
}

void Game::drawUI() {
    float panelX = static_cast<float>(window_.getSize().x) - menuWidth_;

    sf::Text goldText(font_, "Gold: " + std::to_string(gold_), 26);
    goldText.setFillColor(sf::Color::Yellow);
    goldText.setPosition({panelX + 20.f, 20.f});
    window_.draw(goldText);

    sf::Text levelText(font_, "LEVEL " + std::to_string(currentLevel_), 42);
    levelText.setFillColor(sf::Color::White);

    sf::FloatRect bounds = levelText.getLocalBounds();
    levelText.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    });

    float mapWidth = static_cast<float>(window_.getSize().x) - menuWidth_;
    levelText.setPosition({mapWidth / 2.f, 40.f});

    window_.draw(levelText);
}