#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

Game::Game()
    : window_(sf::VideoMode({1280, 720}), "Tower Defense"),
      money_(200), baseHP_(10), wave_(0), toSpawn_(0),
      tileSize_(64.f), cols_(1280 / 64), rows_(720 / 64) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    path_ = {
        {0,5},{1,5},{2,5},{3,5},{4,5},{5,5},{6,5},{7,5},
        {7,6},{7,7},{7,8},
        {8,8},{9,8},{10,8},{11,8},{12,8},{13,8},{14,8},
        {15,8},{16,8},{16,7},{16,6},{16,5},{16,4},
        {16,3},{16,2},{16,1},{16,0}
    };

    if (!grassTexture_.loadFromFile("C:\\Users\\spaul\\Desktop\\grass.png")) {
        std::cerr << "Could not load grass.png\n";
    } else {
        grassTexture_.setRepeated(true);
    }

    if (!dirtTexture_.loadFromFile("C:\\Users\\spaul\\Desktop\\dirt4.png.preview.jpg")) {
        std::cerr << "Could not load dirt texture\n";
    } else {
        dirtTexture_.setRepeated(true);
    }

    if (!catapultTexture_.loadFromFile("C:\\Users\\spaul\\Desktop\\catapult.png")) {
        std::cerr << "Could not load catapult.png\n";
    }

    if (!cannonTexture_.loadFromFile("C:\\Users\\spaul\\Desktop\\cannon.png")) {
        std::cerr << "Could not load cannon.png\n";
    }

    if (!tankTexture_.loadFromFile("C:\\Users\\spaul\\Desktop\\tank.png")) {
        std::cerr << "Could not load tank.png\n";
    }

    towerMenu_.setup(window_, catapultTexture_, cannonTexture_, tankTexture_);

    nextWave();
}

void Game::run() {
    while (window_.isOpen()) {
        handleEvents();

        float dt = deltaClock_.restart().asSeconds();
        update(dt);
        render();
    }
}