#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Enemy.h"

Game::Game()
    : window_(sf::VideoMode({1280, 720}), "Tower Defense"),
      gold_(60),
      tileSize_(64.f),
      cols_(1280 / 64),
      rows_(720 / 64) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

   path_ = {
    {0,4},{1,4},{2,4},{3,4},{4,4},{5,4},{6,4},{7,4},{8,4},
    {8,5},{8,6},{8,7},
    {7,7},{6,7},{5,7},{4,7},{3,7},
    {3,8},{3,9},
    {4,9},{5,9},{6,9},{7,9},{8,9},{9,9},{10,9},{11,9},{12,9},{13,9},{14,9},
    {14,8},{14,7},{14,6},{14,5}
};

    if (!grassTexture_.loadFromFile("C:\\Users\\spaul\\Desktop\\grass.png")) {
        std::cerr << "Could not load grass.png\n";
    } else {
        grassTexture_.setRepeated(true);
    }

    if (!castleTexture_.loadFromFile("C:\\Users\\spaul\\Desktop\\tower.png")) {
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
}

void Game::updateMenuColors() {
    easyButton_.setFillColor(sf::Color(180, 180, 180));
    mediumButton_.setFillColor(sf::Color(180, 180, 180));
    hardButton_.setFillColor(sf::Color(180, 180, 180));

    if (selectedDifficulty_ == Difficulty::Easy) {
        easyButton_.setFillColor(sf::Color(100, 220, 100));
    }
    else if (selectedDifficulty_ == Difficulty::Medium) {
        mediumButton_.setFillColor(sf::Color(255, 220, 100));
    }
    else if (selectedDifficulty_ == Difficulty::Hard) {
        hardButton_.setFillColor(sf::Color(220, 100, 100));
    }
}

void Game::handleMenuClick(sf::Vector2f mousePos) {
    if (easyButton_.getGlobalBounds().contains(mousePos)) {
        selectedDifficulty_ = Difficulty::Easy;
        updateMenuColors();
    }
    else if (mediumButton_.getGlobalBounds().contains(mousePos)) {
        selectedDifficulty_ = Difficulty::Medium;
        updateMenuColors();
    }
    else if (hardButton_.getGlobalBounds().contains(mousePos)) {
        selectedDifficulty_ = Difficulty::Hard;
        updateMenuColors();
    }
    else if (startButton_.getGlobalBounds().contains(mousePos)) {
        applyDifficultySettings();
        startLevel(1);
        state_ = GameState::Playing;
        deltaClock_.restart();
    }
}

void Game::applyDifficultySettings() {
    if (selectedDifficulty_ == Difficulty::Easy) {
        gold_ = 250;
        spawnInterval_ = 1.2f;
        castleMaxHP_ = 10;
        castleHP_ = 10;
    }
    else if (selectedDifficulty_ == Difficulty::Medium) {
        gold_ = 200;
        spawnInterval_ = 0.9f;
        castleMaxHP_ = 8;
        castleHP_ = 8;
    }
    else if (selectedDifficulty_ == Difficulty::Hard) {
        gold_ = 150;
        spawnInterval_ = 0.7f;
        castleMaxHP_ = 6;
        castleHP_ = 6;
    }
}

void Game::drawMenu() {
    window_.draw(*menuTitle_);
    window_.draw(easyButton_);
    window_.draw(mediumButton_);
    window_.draw(hardButton_);
    window_.draw(startButton_);
    window_.draw(*easyText_);
    window_.draw(*mediumText_);
    window_.draw(*hardText_);
    window_.draw(*startText_);
}

void Game::run() {
    while (window_.isOpen()) {
        handleEvents();

        float dt = deltaClock_.restart().asSeconds();

        if (state_ == GameState::Menu) {
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
        state_ = GameState::Victory;
        levelInProgress_ = false;
        return;
    }

    currentLevel_ = level;

    const WaveData& wave = waves_[level - 1];

    int slimeBonus = 0;
    int goblinBonus = 0;
    int wolfBonus = 0;

    if (selectedDifficulty_ == Difficulty::Medium) {
        slimeBonus = 2;
        goblinBonus = 1;
        wolfBonus = 1;
    }
    else if (selectedDifficulty_ == Difficulty::Hard) {
        slimeBonus = 4;
        goblinBonus = 2;
        wolfBonus = 2;
    }

    slimesLeftToSpawn_ = wave.slimes + slimeBonus;
    goblinsLeftToSpawn_ = wave.goblins + goblinBonus;
    wolvesLeftToSpawn_ = wave.wolves + wolfBonus;

    enemiesToSpawn_ = slimesLeftToSpawn_ + goblinsLeftToSpawn_ + wolvesLeftToSpawn_;
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
        Enemy* enemy = dynamic_cast<Enemy*>(obj.get());
        if (enemy && enemy->isActive()) {
            anyEnemyAlive = true;
            break;
        }
    }

    if (levelInProgress_ && enemiesToSpawn_ == 0 && !anyEnemyAlive) {
        levelInProgress_ = false;
        betweenLevels_ = true;
        levelPauseTimer_ = levelPauseDuration_;
    }
}

void Game::setupMenu() {
    if (!font_.openFromFile("C:\\Users\\spaul\\Desktop\\Inter_18pt-BlackItalic.ttf")) {
        std::cerr << "Could not load Inter_18pt-BlackItalic.ttf\n";
    }

    const float windowWidth = static_cast<float>(window_.getSize().x);
    const float centerX = windowWidth / 2.f;

    const sf::Vector2f diffButtonSize(180.f, 60.f);
    const float gap = 30.f;

    const float totalWidth = diffButtonSize.x * 3.f + gap * 2.f;
    const float startX = centerX - totalWidth / 2.f;
    const float buttonsY = 280.f;

    menuTitle_ = std::make_unique<sf::Text>(font_, "Choose difficulty level", 40);
    {
        sf::FloatRect bounds = menuTitle_->getLocalBounds();
        menuTitle_->setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
        });
    }
    menuTitle_->setFillColor(sf::Color::White);
    menuTitle_->setPosition({centerX, 170.f});

    easyButton_.setSize(diffButtonSize);
    easyButton_.setPosition({startX, buttonsY});

    mediumButton_.setSize(diffButtonSize);
    mediumButton_.setPosition({startX + diffButtonSize.x + gap, buttonsY});

    hardButton_.setSize(diffButtonSize);
    hardButton_.setPosition({startX + 2.f * (diffButtonSize.x + gap), buttonsY});

    startButton_.setSize({220.f, 70.f});
    startButton_.setPosition({centerX - 110.f, 420.f});
    startButton_.setFillColor(sf::Color(70, 130, 70));

    easyText_ = std::make_unique<sf::Text>(font_, "Easy", 28);
    mediumText_ = std::make_unique<sf::Text>(font_, "Medium", 28);
    hardText_ = std::make_unique<sf::Text>(font_, "Hard", 28);
    startText_ = std::make_unique<sf::Text>(font_, "Start", 30);

    easyText_->setFillColor(sf::Color::Black);
    mediumText_->setFillColor(sf::Color::Black);
    hardText_->setFillColor(sf::Color::Black);
    startText_->setFillColor(sf::Color::White);

    auto centerTextInButton = [](sf::Text& text, const sf::RectangleShape& button) {
        sf::FloatRect textBounds = text.getLocalBounds();
        sf::FloatRect buttonBounds = button.getGlobalBounds();

        text.setOrigin({
            textBounds.position.x + textBounds.size.x / 2.f,
            textBounds.position.y + textBounds.size.y / 2.f
        });

        text.setPosition({
            buttonBounds.position.x + buttonBounds.size.x / 2.f,
            buttonBounds.position.y + buttonBounds.size.y / 2.f
        });
    };

    centerTextInButton(*easyText_, easyButton_);
    centerTextInButton(*mediumText_, mediumButton_);
    centerTextInButton(*hardText_, hardButton_);
    centerTextInButton(*startText_, startButton_);

    updateMenuColors();
}

