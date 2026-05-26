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
    struct WaveData {
        int slimes;
        int goblins;
        int wolves;
        int towerLimit;
    };

    sf::RenderWindow window_;
    bool inMenu_ = true;
    sf::Font font_;

    int baseHP_;
    int gold_ = 0;
    int currentLevel_ = 1;
    int maxLevels_ = 5;
    int towersPlaced_ = 0;
    int towerLimit_ = 2;
    int enemiesToSpawn_ = 0;

    int slimesLeftToSpawn_ = 0;
    int goblinsLeftToSpawn_ = 0;
    int wolvesLeftToSpawn_ = 0;

    float spawnTimer_ = 0.f;
    float spawnInterval_ = 1.f;

    bool levelInProgress_ = false;
    bool gameOver_ = false;
    bool victory_ = false;

    float tileSize_;
    int cols_;
    int rows_;
    float menuWidth_ = 180.f;

    sf::Clock deltaClock_;

    std::vector<std::unique_ptr<GameObject>> objects_;
    std::vector<sf::Vector2i> path_;
    std::vector<sf::Vector2i> occupiedTiles_;
    std::vector<WaveData> waves_;

    sf::Texture grassTexture_;
    sf::Texture dirtTexture_;
    sf::Texture catapultTexture_;
    sf::Texture cannonTexture_;
    sf::Texture tankTexture_;

    TowerMenu towerMenu_;

    void setupMenu();
    void updateMenu();
    void drawMenu();

    void handleEvents();
    void handleMousePressed(const sf::Event::MouseButtonPressed& mb);
    void handleMouseMoved(const sf::Event::MouseMoved& mm);
    void handleMouseReleased(const sf::Event::MouseButtonReleased& mr);

    void update(float dt);
    void render();
    void drawUI();

    void setupWaves();
    void startLevel(int level);
    void spawnNextEnemy();
    void checkLevelFinished();
    int wave_ = 1;
    int toSpawn_ = 0;

    bool isPathTile(int col, int row) const;
    bool isTileOccupied(int col, int row) const;
    sf::Vector2f tileCenter(int col, int row) const;
    bool canPlaceTower(int col, int row) const;
    void placeDraggedTower(int col, int row);
};