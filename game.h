#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "GameObject.h"
#include "TowerMenu.h"

class Game {
public:
    Game();
    void run();

private:
    sf::RenderWindow window_;

    int money_;
    int baseHP_;
    int wave_;
    int toSpawn_;

    float tileSize_;
    int cols_;
    int rows_;

    float menuWidth_ = 180.f;

    sf::Clock deltaClock_;
    sf::Clock spawnClock_;

    std::vector<std::unique_ptr<GameObject>> objects_;
    std::vector<sf::Vector2i> path_;
    std::vector<sf::Vector2i> occupiedTiles_;

    sf::Texture grassTexture_;
    sf::Texture dirtTexture_;
    sf::Texture catapultTexture_;
    sf::Texture cannonTexture_;
    sf::Texture tankTexture_;

    TowerMenu towerMenu_;

    void handleEvents();
    void handleMousePressed(const sf::Event::MouseButtonPressed& mb);
    void handleMouseMoved(const sf::Event::MouseMoved& mm);
    void handleMouseReleased(const sf::Event::MouseButtonReleased& mr);

    void update(float dt);
    void render();
    void drawUI();

    void spawnEnemy();
    void handleCollisions();
    void nextWave();

    bool isPathTile(int col, int row) const;
    bool isTileOccupied(int col, int row) const;
    sf::Vector2f tileCenter(int col, int row) const;
    bool canPlaceTower(int col, int row) const;
    void placeDraggedTower(int col, int row);
};