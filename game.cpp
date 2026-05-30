#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <iostream>



Game::Game()
    : window_(sf::VideoMode({1280, 720}), "Tower Defense"),
      gold_(60),
      tileSize_(64.f),
      cols_(1280 / 64),
      rows_(720 / 64) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

   path_ = {
    {0,3},{1,3},{2,3},{3,3},{4,3},{5,3},{6,3},{7,3},{8,3},
    {8,4},{8,5},{8,6},
    {7,6},{6,6},{5,6},{4,6},{3,6},
    {3,7},{3,8},{3,9},
    {4,9},{5,9},{6,9},{7,9},{8,9},{9,9},{10,9},{11,9},{12,9},{13,9},{14,9},
    {14,8},{14,7},{14,6},{14,5},{14,4}
};

    if (!grassTexture_.loadFromFile("photos/grass.png")) {
        std::cerr << "Could not load grass.png\n";
    } else {
        grassTexture_.setRepeated(true);
    }

    if (!castleTexture_.loadFromFile("photos/tower.png")) {
    std::cerr << "Could not load tower.png\n";
}
else {
        castleSprite_ = std::make_unique<sf::Sprite>(castleTexture_);

    sf::Vector2f scale(
        180.f / static_cast<float>(castleTexture_.getSize().x),
        180.f / static_cast<float>(castleTexture_.getSize().y)
    );
    castleSprite_->setScale(scale);

    sf::Vector2f origin(
        static_cast<float>(castleTexture_.getSize().x) * 0.5f,
        static_cast<float>(castleTexture_.getSize().y) * 0.5f
    );
    castleSprite_->setOrigin(origin);

        sf::Vector2i lastTile = path_.back();
    sf::Vector2f castlePos = tileCenter(lastTile.x, lastTile.y - 1);
    castleSprite_->setPosition(castlePos);
}

    if (!dirtTexture_.loadFromFile("photos/dirt4.png.preview.jpg")) {
        std::cerr << "Could not load dirt texture\n";
    } else {
        dirtTexture_.setRepeated(true);
    }

    if (!catapultTexture_.loadFromFile("photos/catapult.png")) {
        std::cerr << "Could not load catapult.png\n";
    }

    if (!cannonTexture_.loadFromFile("photos/cannon.png")) {
        std::cerr << "Could not load cannon.png\n";
    }

    if (!tankTexture_.loadFromFile("photos/tank.png")) {
        std::cerr << "Could not load tank.png\n";
    }

    towerMenu_.setup(window_, catapultTexture_, cannonTexture_, tankTexture_);

    setupMenu();
    setupMainMenu();
    setupWaves();

    if (!backgroundMusic_.openFromFile("photos/music.ogg")) {
    std::cerr << "Could not load background.ogg\n";
} else {
    backgroundMusic_.setLooping(true);
    backgroundMusic_.setVolume(50.f);
    backgroundMusic_.play();
}
}


void Game::run() {
    while (window_.isOpen()) {
        handleEvents();

        float dt = deltaClock_.restart().asSeconds();

        if (state_ == GameState::MainMenu) {
            window_.clear(sf::Color(40, 40, 60));
            drawMainMenu();
            window_.display();
            continue;
        }

        if (state_ == GameState::DifficultyMenu) {
            window_.clear(sf::Color(40, 40, 60));
            drawMenu();
            window_.display();
            continue;
        }

        update(dt);

        window_.clear();
        render();
        window_.display();
    }
}

