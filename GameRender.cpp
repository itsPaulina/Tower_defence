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

    if (castleSprite_) {
        window_.draw(*castleSprite_);
    }

    for (const auto& obj : objects_) {
        if (obj->isActive()) {
            obj->draw(window_);
        }
    }

    if (castleSprite_) {
        drawCastleHP();
    }

    towerMenu_.draw(window_);
    drawUI();

   
    if (gameOver_) {
        sf::RectangleShape overlay({
            static_cast<float>(window_.getSize().x),
            static_cast<float>(window_.getSize().y)
        });
        overlay.setFillColor(sf::Color(0, 0, 0, 160));
        window_.draw(overlay);

        sf::Text loseText(font_, "Game Over", 68);
        loseText.setFillColor(sf::Color::Red);

        sf::FloatRect bounds = loseText.getLocalBounds();
        loseText.setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
        });

        loseText.setPosition({
            static_cast<float>(window_.getSize().x) / 2.f,
            static_cast<float>(window_.getSize().y) / 2.f
        });

        window_.draw(loseText);
    }

    if (victory_) {
        sf::RectangleShape overlay({
            static_cast<float>(window_.getSize().x),
            static_cast<float>(window_.getSize().y)
        });
        overlay.setFillColor(sf::Color(0, 0, 0, 160));
        window_.draw(overlay);

        sf::Text winText(font_, "You win!", 68);
        winText.setFillColor(sf::Color::Yellow);

        sf::FloatRect bounds = winText.getLocalBounds();
        winText.setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
        });

        winText.setPosition({
            static_cast<float>(window_.getSize().x) / 2.f,
            static_cast<float>(window_.getSize().y) / 2.f
        });

        window_.draw(winText);
    }
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

void Game::drawCastleHP() {
    if (!castleSprite_) {
        return;
    }

    float barWidth = 180.f;
    float barHeight = 14.f;

    float ratio = 0.f;
    if (castleMaxHP_ > 0) {
        ratio = static_cast<float>(castleHP_) / static_cast<float>(castleMaxHP_);
    }
    if (ratio < 0.f) ratio = 0.f;

    sf::Vector2f pos = castleSprite_->getPosition();

    sf::RectangleShape back({barWidth, barHeight});
    back.setFillColor(sf::Color(40, 40, 40));
    back.setPosition({pos.x - barWidth / 2.f, pos.y - 120.f});

    sf::RectangleShape front({barWidth * ratio, barHeight});
    front.setFillColor(sf::Color::Red);
    front.setPosition({pos.x - barWidth / 2.f, pos.y - 120.f});

    window_.draw(back);
    window_.draw(front);
}