#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Enemy.h"



Game::Game()
    : window_(sf::VideoMode({1280, 720}), "Tower Defense"),
      gold_(60),
      baseHP_(10),
      tileSize_(64.f),
      cols_(1280 / 64),
      rows_(720 / 64)  {
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

    setupMenu();
    setupWaves();
    startLevel(1);
}
void Game::setupMenu() {
    if (!font_.openFromFile("C:\\Users\\spaul\\Desktop\\Inter_18pt-BlackItalic.ttf")) {
        std::cerr << "Could not load Inter_18pt-BlackItalic.ttf\n";
    }
}

void Game::updateMenu() {
    sf::Text startText(font_, "Start the game", 34);
    startText.setPosition({500.f, 320.f});

    sf::Text exitText(font_, "Exit the game", 34);
    exitText.setPosition({515.f, 390.f});

    sf::Vector2f mousePos = window_.mapPixelToCoords(sf::Mouse::getPosition(window_));

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (startText.getGlobalBounds().contains(mousePos)) {
            inMenu_ = false;
            deltaClock_.restart();
            return;
        }

        if (exitText.getGlobalBounds().contains(mousePos)) {
            window_.close();
            return;
        }
    }
}

void Game::drawMenu() {
    sf::Text titleText(font_, "Tower Defense", 54);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition({430.f, 170.f});

    sf::Text startText(font_, "Start the game", 34);
    startText.setFillColor(sf::Color::Green);
    startText.setPosition({500.f, 320.f});

    sf::Text exitText(font_, "Exit the game", 34);
    exitText.setFillColor(sf::Color::Red);
    exitText.setPosition({515.f, 390.f});

    window_.draw(titleText);
    window_.draw(startText);
    window_.draw(exitText);
}

void Game::run() {
    while (window_.isOpen()) {
        handleEvents();

        float dt = deltaClock_.restart().asSeconds();

        if (inMenu_) {
            updateMenu();

            window_.clear(sf::Color::Black);
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
void Game::setupWaves() {
    waves_ = {
        {3, 0, 0, 2},
        {4, 1, 0, 3},
        {5, 2, 0, 4},
        {6, 2, 1, 5},
        {8, 3, 2, 6}
    };
}

void Game::startLevel(int level) {
    if (level > maxLevels_) {
        victory_ = true;
        levelInProgress_ = false;
        return;
    }

    currentLevel_ = level;

    const WaveData& wave = waves_[level - 1];
    slimesLeftToSpawn_ = wave.slimes;
    goblinsLeftToSpawn_ = wave.goblins;
    wolvesLeftToSpawn_ = wave.wolves;

    enemiesToSpawn_ = wave.slimes + wave.goblins + wave.wolves;
    towerLimit_ = wave.towerLimit;

    towersPlaced_ = 0;
    spawnTimer_ = 0.f;
    levelInProgress_ = true;
}

void Game::spawnNextEnemy() {
    if (slimesLeftToSpawn_ > 0) {
        objects_.push_back(std::make_unique<Slime>(path_, tileSize_));
        slimesLeftToSpawn_--;
        enemiesToSpawn_--;
        return;
    }

    if (goblinsLeftToSpawn_ > 0) {
        objects_.push_back(std::make_unique<Goblin>(path_, tileSize_));
        goblinsLeftToSpawn_--;
        enemiesToSpawn_--;
        return;
    }

    if (wolvesLeftToSpawn_ > 0) {
        objects_.push_back(std::make_unique<Wolf>(path_, tileSize_));
        wolvesLeftToSpawn_--;
        enemiesToSpawn_--;
        return;
    }
}

void Game::checkLevelFinished() {
    bool anyEnemyAlive = false;

    for (const auto& obj : objects_) {
        // jeśli masz klasę Enemy dziedziczącą po GameObject
        // to ten fragment będzie dobry:
        Enemy* enemy = dynamic_cast<Enemy*>(obj.get());
        if (enemy && enemy->isActive()) {
            anyEnemyAlive = true;
            break;
        }
    }

    if (levelInProgress_ && enemiesToSpawn_ == 0 && !anyEnemyAlive) {
        levelInProgress_ = false;
        startLevel(currentLevel_ + 1);
    }
}