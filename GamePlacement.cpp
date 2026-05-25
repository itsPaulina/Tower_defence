#include "Game.h"
#include "Tower.h"

bool Game::isPathTile(int col, int row) const {
    for (const auto& t : path_) {
        if (t.x == col && t.y == row) {
            return true;
        }
    }
    return false;
}

bool Game::isTileOccupied(int col, int row) const {
    for (const auto& t : occupiedTiles_) {
        if (t.x == col && t.y == row) {
            return true;
        }
    }
    return false;
}

sf::Vector2f Game::tileCenter(int col, int row) const {
    return {
        col * tileSize_ + tileSize_ / 2.f,
        row * tileSize_ + tileSize_ / 2.f
    };
}

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

void Game::placeDraggedTower(int col, int row) {
    int cost = 0;
    TowerType draggedTower = towerMenu_.getDraggedTower();

    switch (draggedTower) {
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

    switch (draggedTower) {
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