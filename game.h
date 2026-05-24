#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "GameObject.h"
#include "Enemy.h"
#include "Tower.h"
#include "Projectile.h"

class Game {
public:
    Game();
    void run();

private:
    // Core game loop methods
    void update(float dt);
    void render();

    // Enemy / wave logic
    void spawnEnemy();
    void nextWave();

    // Collision handling
    void handleCollisions();

    // Tile helpers
    bool isPathTile(int col, int row) const;
    bool isTileOccupied(int col, int row) const;
    sf::Vector2f tileCenter(int col, int row) const;

    // Drag and drop tower placement
    bool canPlaceTower(int col, int row) const;
    void placeDraggedTower(int col, int row);

    // UI / menu helpers
    void drawUI();
    void setupTowerMenu();
    void updateDragPreview(sf::Vector2f mousePos);
    void drawTowerMenu();

    
private:
    // Main game window
    sf::RenderWindow window_;

    // Delta time clock
    sf::Clock deltaClock_;

    // Enemy spawn timer
    sf::Clock spawnClock_;

    // All active game objects
    std::vector<std::unique_ptr<GameObject>> objects_;

    // Enemy path on the map
    std::vector<sf::Vector2i> path_;

    // Occupied tower tiles
    std::vector<sf::Vector2i> occupiedTiles_;

    // Game state
    int money_;
    int baseHP_;
    int wave_;
    int toSpawn_;

    // Grid settings
    float tileSize_;
    int cols_;
    int rows_;

    // Tower drag-and-drop type
    enum class TowerType {
        None,
        Catapult,
        CannonTower,
        Tank
    };

    // Drag-and-drop state
    TowerType draggedTower_ = TowerType::None;
    bool isDraggingTower_ = false;

    // Right-side menu settings
    float menuWidth_ = 180.f;

    // Menu background and buttons
    sf::Texture grassTexture_;
    sf::Texture dirtTexture_;
    sf::Texture catapultTexture_;
    sf::Texture cannonTexture_;
    sf::Texture tankTexture_;
    sf::RectangleShape menuBackground_;
    sf::RectangleShape cannonButton_;
    sf::RectangleShape catapultButton_;
    sf::RectangleShape tankButton_;
    // Preview shown while dragging a tower
    sf::RectangleShape dragPreview_;
};