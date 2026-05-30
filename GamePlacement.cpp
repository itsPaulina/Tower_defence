#include "Game.h"   // Includes the Game class declaration.
#include "Tower.h"  // Includes tower classes like Catapult, CannonTower, and Tank.


bool Game::isPathTile(int col, int row) const {
    for (const auto& t : path_) { // Check every tile in the enemy path.
        if (t.x == col && t.y == row) { // Compare given tile coordinates with current path tile.
            return true; // This tile is part of the path.
        }
    }
    return false; // Tile was not found in the path.
}


bool Game::isTileOccupied(int col, int row) const {
    for (const auto& t : occupiedTiles_) { // Check every already occupied tile.
        if (t.x == col && t.y == row) { // Compare coordinates.
            return true; // Tile is already occupied by a tower.
        }
    }
    return false; // Tile is free.
}


sf::Vector2f Game::tileCenter(int col, int row) const {
    return {
        col * tileSize_ + tileSize_ / 2.f, // X coordinate of the tile center.
        row * tileSize_ + tileSize_ / 2.f  // Y coordinate of the tile center.
    };
}


bool Game::canPlaceTower(int col, int row) const {
    if (col < 0 || col >= cols_ || row < 0 || row >= rows_) { // Check if tile is outside the map.
        return false;
    }

    if (isPathTile(col, row)) { // Prevent placing towers on the enemy path.
        return false;
    }

    if (isTileOccupied(col, row)) { // Prevent placing towers on occupied tiles.
        return false;
    }

    return true; // Tile is valid for tower placement.
}


void Game::placeDraggedTower(int col, int row) {
    int cost = 0; // Will store the price of the selected tower.
    TowerType draggedTower = towerMenu_.getDraggedTower(); // Get the currently dragged tower type.

    switch (draggedTower) { // Set tower cost based on tower type.
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
        return; // Stop if tower type is invalid.
    }

    if (gold_ < cost) { // Check if player has enough gold.
        return; // Do not place tower if gold is insufficient.
    }

    sf::Vector2f pos = tileCenter(col, row); // Get center position of selected tile.

    switch (draggedTower) { // Create the correct tower type.
    case TowerType::Catapult: {
        auto tower = std::make_unique<Catapult>(pos); // Create catapult tower.
        tower->setTexture(&catapultTexture_);         // Assign catapult texture.
        objects_.push_back(std::move(tower));         // Add tower to game objects.
        break;
    }
    case TowerType::CannonTower: {
        auto tower = std::make_unique<CannonTower>(pos); // Create cannon tower.
        tower->setTexture(&cannonTexture_);              // Assign cannon texture.
        objects_.push_back(std::move(tower));            // Add tower to game objects.
        break;
    }
    case TowerType::Tank: {
        auto tower = std::make_unique<Tank>(pos); // Create tank tower.
        tower->setTexture(&tankTexture_);         // Assign tank texture.
        objects_.push_back(std::move(tower));     // Add tower to game objects.
        break;
    }
    default:
        return; // Stop if tower type is invalid.
    }

    gold_ -= cost; // Deduct gold after successful placement.
    towersPlaced_++; // Increase number of placed towers.
    occupiedTiles_.push_back({col, row}); // Mark tile as occupied.
}