#include "Game.h"
#include "Enemy.h"
#include "Tower.h"
#include "Projectile.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <optional>
#include <iostream>

// Game constructor.
// Creates the window, sets default values,
// defines the enemy path, sets up the drag-and-drop tower menu,
// loads map textures, and starts the first wave.
Game::Game()
    : window_(sf::VideoMode({1280, 720}), "Tower Defense"),
      money_(200), baseHP_(10), wave_(0), toSpawn_(0),
      tileSize_(64.f), cols_(1280 / 64), rows_(720 / 64) {
    // Seed random generator.
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Define the enemy path on the tile grid.
    path_ = {
        {0,5},{1,5},{2,5},{3,5},{4,5},{5,5},{6,5},{7,5},
        {7,6},{7,7},{7,8},
        {8,8},{9,8},{10,8},{11,8},{12,8},{13,8},{14,8},
        {15,8},{16,8},{16,7},{16,6},{16,5},{16,4},
        {16,3},{16,2},{16,1},{16,0}
    };

    // Load grass texture.
    if (!grassTexture_.loadFromFile("C:\\Users\\spaul\\Desktop\\grass.png")) {
        std::cerr << "Could not load grass.png\n";
    } else {
        std::cout << "grass loaded: "
                  << grassTexture_.getSize().x << "x"
                  << grassTexture_.getSize().y << "\n";
        grassTexture_.setRepeated(true);
    }

    // Load dirt texture.
    if (!dirtTexture_.loadFromFile("C:\\Users\\spaul\\Desktop\\dirt4.png.preview.jpg")) {
        std::cerr << "Could not load dirt4.png.preview.jpg\n";
    } else {
        std::cout << "dirt loaded: "
                  << dirtTexture_.getSize().x << "x"
                  << dirtTexture_.getSize().y << "\n";
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
    // Prepare side menu and drag preview.
    setupTowerMenu();

    // Start the first wave.
    nextWave();
}
// Main game loop.
void Game::run() {
    while (window_.isOpen()) {
        // Poll all events from the window.
        while (const std::optional<sf::Event> event = window_.pollEvent()) {
            // Close event.
            if (event->is<sf::Event::Closed>()) {
                window_.close();
            }

            // Mouse button pressed -> start dragging if user clicked a tower button.
            if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mb->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos(
                        static_cast<float>(mb->position.x),
                        static_cast<float>(mb->position.y)
                    );

                   if (catapultButton_.getGlobalBounds().contains(mousePos)) {
                        draggedTower_ = TowerType::Catapult;
                        isDraggingTower_ = true;
                        dragPreview_.setTexture(&catapultTexture_, true);
                        dragPreview_.setFillColor(sf::Color(255, 255, 255, 180));
                        updateDragPreview(mousePos);
                    }
                    else if (cannonButton_.getGlobalBounds().contains(mousePos)) {
                        draggedTower_ = TowerType::CannonTower;
                        isDraggingTower_ = true;
                        dragPreview_.setTexture(&cannonTexture_, true);
                        dragPreview_.setFillColor(sf::Color(255, 255, 255, 180));
                        updateDragPreview(mousePos);
                    }
                    else if (tankButton_.getGlobalBounds().contains(mousePos)) {
                        draggedTower_ = TowerType::Tank;
                        isDraggingTower_ = true;
                        dragPreview_.setTexture(&tankTexture_, true);
                        dragPreview_.setFillColor(sf::Color(255, 255, 255, 180));
                        updateDragPreview(mousePos);
                    }
                }
            }

            // Mouse moved -> move preview while dragging.
            if (const auto* mm = event->getIf<sf::Event::MouseMoved>()) {
                sf::Vector2f mousePos(
                    static_cast<float>(mm->position.x),
                    static_cast<float>(mm->position.y)
                );

                if (isDraggingTower_) {
                    updateDragPreview(mousePos);
                }
            }

            // Mouse released -> try to place dragged tower.
            if (const auto* mr = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mr->button == sf::Mouse::Button::Left && isDraggingTower_) {
                    sf::Vector2f mousePos(
                        static_cast<float>(mr->position.x),
                        static_cast<float>(mr->position.y)
                    );

                    int col = static_cast<int>(mousePos.x / tileSize_);
                    int row = static_cast<int>(mousePos.y / tileSize_);

                    // Do not place towers inside the side menu.
                    if (mousePos.x < static_cast<float>(window_.getSize().x) - menuWidth_) {
                        if (canPlaceTower(col, row)) {
                            placeDraggedTower(col, row);
                        }
                    }

                    // End dragging state.
                    isDraggingTower_ = false;
                    draggedTower_ = TowerType::None;
                }
            }
        }

        // Delta time for smooth updates.
        float dt = deltaClock_.restart().asSeconds();

        update(dt);
        render();
    }
}

// Update all game logic.
void Game::update(float dt) {
    // Fixed spawn interval without config.
    float spawnInterval = 1.5f;

    // Spawn enemies one by one.
    if (toSpawn_ > 0 && spawnClock_.getElapsedTime().asSeconds() >= spawnInterval) {
        spawnEnemy();
        spawnClock_.restart();
        --toSpawn_;
    }

    // Update all active objects.
    for (auto& obj : objects_) {
        if (obj->isActive()) {
            obj->update(dt);
        }
    }

    // Let towers attack.
    for (auto& obj : objects_) {
        Tower* tower = dynamic_cast<Tower*>(obj.get());
        if (tower && tower->isActive()) {
            tower->attack(objects_);
        }
    }

    // Damage base when an enemy reaches the goal.
    for (auto& obj : objects_) {
        Enemy* e = dynamic_cast<Enemy*>(obj.get());
        if (e && e->isActive() && e->reachedGoal()) {
            e->deactivate();
            --baseHP_;
        }
    }

    // Handle collisions.
    handleCollisions();

    // Reward inactive enemies.
    // WARNING: This may also reward enemies that reached the goal.
    for (auto& obj : objects_) {
        Enemy* e = dynamic_cast<Enemy*>(obj.get());
        if (e && !e->isActive()) {
            money_ += e->getReward();
        }
    }

    // Remove inactive objects.
    objects_.erase(
        std::remove_if(objects_.begin(), objects_.end(),
            [](const std::unique_ptr<GameObject>& o) { return !o->isActive(); }),
        objects_.end());
}

// Spawn one random enemy type.
void Game::spawnEnemy() {
    int type = std::rand() % 3;

    if (type == 0) {
        objects_.push_back(std::make_unique<Slime>(path_, tileSize_));
    }
    else if (type == 1) {
        objects_.push_back(std::make_unique<Goblin>(path_, tileSize_));
    }
    else {
        objects_.push_back(std::make_unique<Wolf>(path_, tileSize_));
    }
}

// Handle projectile-enemy collisions.
void Game::handleCollisions() {
    for (auto& projObj : objects_) {
        Projectile* p = dynamic_cast<Projectile*>(projObj.get());
        if (!p || !p->isActive()) {
            continue;
        }

        for (auto& enemyObj : objects_) {
            Enemy* e = dynamic_cast<Enemy*>(enemyObj.get());
            if (!e || !e->isActive()) {
                continue;
            }

            if (p->getBounds().findIntersection(e->getBounds())) {
                e->takeDamage(p->getDamage());
                p->hitTarget();
                break;
            }
        }
    }
}

// Start the next wave.
void Game::nextWave() {
    ++wave_;

    // Fixed wave size without config.
    int base = 5;
    toSpawn_ += base + wave_ * 2;
}
void Game::render() {
    // Clear the window.
    window_.clear(sf::Color(20, 20, 20));

    // Compute playable map area width (without the right-side menu).
    float mapWidth = static_cast<float>(window_.getSize().x) - menuWidth_;
    float mapHeight = static_cast<float>(rows_) * tileSize_;

    // Draw one large repeated grass texture over the whole map.
    if (grassTexture_.getSize().x > 0 && grassTexture_.getSize().y > 0) {
        sf::Sprite grassSprite(grassTexture_);

        grassSprite.setTextureRect(sf::IntRect(
            {0, 0},
            {
                static_cast<int>(mapWidth),
                static_cast<int>(mapHeight)
            }
        ));

        // Slightly darken the grass to reduce the neon look.
        grassSprite.setColor(sf::Color(170, 170, 170));

        grassSprite.setPosition({0.f, 0.f});
        window_.draw(grassSprite);
    }
    else {
        sf::RectangleShape fallback({mapWidth, mapHeight});
        fallback.setPosition({0.f, 0.f});
        fallback.setFillColor(sf::Color(60, 120, 60));
        window_.draw(fallback);
    }

    // Draw dirt on path tiles only.
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
                    {
                        static_cast<int>(tileSize_),
                        static_cast<int>(tileSize_)
                    }
                ));

                dirtSprite.setPosition(pos);
                window_.draw(dirtSprite);
            }
            else {
                sf::RectangleShape fallback({tileSize_, tileSize_});
                fallback.setPosition(pos);
                fallback.setFillColor(sf::Color(160, 110, 60));
                window_.draw(fallback);
            }
        }
    }

    // Draw active game objects.
    for (const auto& obj : objects_) {
        if (obj->isActive()) {
            obj->draw(window_);
        }
    }

    // Draw menu and UI.
    drawTowerMenu();
    drawUI();

    // Show the final frame.
    window_.display();
}
// Draw simple top UI bar.
void Game::drawUI() {
    sf::RectangleShape bar({1280.f, 40.f});
    bar.setFillColor(sf::Color(0, 0, 0, 180));
    bar.setPosition({0.f, 0.f});
    window_.draw(bar);
}

// Check if tile belongs to the path.
bool Game::isPathTile(int col, int row) const {
    for (const auto& t : path_) {
        if (t.x == col && t.y == row) {
            return true;
        }
    }
    return false;
}

// Check if tile already contains a tower.
bool Game::isTileOccupied(int col, int row) const {
    for (const auto& t : occupiedTiles_) {
        if (t.x == col && t.y == row) {
            return true;
        }
    }
    return false;
}

// Get the center of a tile in world coordinates.
sf::Vector2f Game::tileCenter(int col, int row) const {
    return {
        col * tileSize_ + tileSize_ / 2.f,
        row * tileSize_ + tileSize_ / 2.f
    };
}

// Check if a tower can be placed on the given tile.
bool Game::canPlaceTower(int col, int row) const {
    if (col < 0 || col >= cols_ || row < 0 || row >= rows_) {
        return false;
    }

    if (isPathTile(col, row)) {
        return false;
    }

    if (isTileOccupied(col, row)) {
        return false;
    }

    return true;
}

// Place the currently dragged tower.
void Game::placeDraggedTower(int col, int row) {
    int cost = 0;

    switch (draggedTower_) {
    case TowerType::Catapult:
        cost = 40;
        break;
    case TowerType::CannonTower:
        cost = 60;
        break;
    case TowerType::Tank:
        cost = 80;
        break;
    default:
        return;
    }

    if (money_ < cost) {
        return;
    }

    sf::Vector2f pos = tileCenter(col, row);

    switch (draggedTower_) {
    case TowerType::Catapult: {
        auto tower = std::make_unique<Catapult>(pos);
        tower->setTexture(&catapultTexture_);
        objects_.push_back(std::move(tower));
        break;
    }
    case TowerType::CannonTower: {
        auto tower = std::make_unique<CannonTower>(pos);
        tower->setTexture(&cannonTexture_);
        objects_.push_back(std::move(tower));
        break;
    }
    case TowerType::Tank: {
        auto tower = std::make_unique<Tank>(pos);
        tower->setTexture(&tankTexture_);
        objects_.push_back(std::move(tower));
        break;
    }
    default:
        return;
    }

    money_ -= cost;
    occupiedTiles_.push_back({col, row});
}

// Set up the right-side menu and drag preview.
void Game::setupTowerMenu() {
    const sf::Vector2f towerSize(64.f, 64.f);

    menuBackground_.setSize({menuWidth_, static_cast<float>(window_.getSize().y)});
    menuBackground_.setPosition({window_.getSize().x - menuWidth_, 0.f});
    menuBackground_.setFillColor(sf::Color(40, 40, 40));

    catapultButton_.setSize({80.f, 80.f});
    catapultButton_.setPosition({window_.getSize().x - menuWidth_ + 50.f, 20.f});
    catapultButton_.setTexture(&catapultTexture_, true);
    catapultButton_.setFillColor(sf::Color::White);

    cannonButton_.setSize({80.f, 80.f});
    cannonButton_.setPosition({window_.getSize().x - menuWidth_ + 50.f, 120.f});
    cannonButton_.setTexture(&cannonTexture_, true);
    cannonButton_.setFillColor(sf::Color::White);

    tankButton_.setSize({80.f, 80.f});
    tankButton_.setPosition({window_.getSize().x - menuWidth_ + 50.f, 220.f});
    tankButton_.setTexture(&tankTexture_, true);
    tankButton_.setFillColor(sf::Color::White);

    dragPreview_.setSize(towerSize);
    dragPreview_.setOrigin({towerSize.x / 2.f, towerSize.y / 2.f});
    dragPreview_.setFillColor(sf::Color(255, 255, 255, 180));
}
// Update drag preview position and color.
void Game::updateDragPreview(sf::Vector2f mousePos) {
    int col = static_cast<int>(mousePos.x / tileSize_);
    int row = static_cast<int>(mousePos.y / tileSize_);

    sf::Vector2f snappedPos = tileCenter(col, row);
    dragPreview_.setPosition(snappedPos);

    if (mousePos.x >= static_cast<float>(window_.getSize().x) - menuWidth_) {
        dragPreview_.setFillColor(sf::Color(255, 0, 0, 140));
        return;
    }

    if (canPlaceTower(col, row)) {
        switch (draggedTower_) {
        case TowerType::Catapult:
            dragPreview_.setTexture(&catapultTexture_, true);
            dragPreview_.setFillColor(sf::Color(255, 255, 255, 180));
            break;
        case TowerType::CannonTower:
            dragPreview_.setTexture(&cannonTexture_, true);
            dragPreview_.setFillColor(sf::Color(255, 255, 255, 180));
            break;
        case TowerType::Tank:
            dragPreview_.setTexture(&tankTexture_, true);
            dragPreview_.setFillColor(sf::Color(255, 255, 255, 180));
            break;
        default:
            dragPreview_.setFillColor(sf::Color(255, 255, 255, 180));
            break;
        }
    }
    else {
        dragPreview_.setFillColor(sf::Color(255, 0, 0, 140));
    }
}

// Draw menu buttons and preview.
void Game::drawTowerMenu() {
    window_.draw(menuBackground_);

    window_.draw(catapultButton_);
    window_.draw(cannonButton_);
    window_.draw(tankButton_);

    if (isDraggingTower_) {
        window_.draw(dragPreview_);
    }
}