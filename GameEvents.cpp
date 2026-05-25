#include "Game.h"
#include <optional>

void Game::handleEvents() {
    while (const std::optional<sf::Event> event = window_.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window_.close();
        }
         if (inMenu_) {
            continue;
        }

        if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
            handleMousePressed(*mb);
        }

        if (const auto* mm = event->getIf<sf::Event::MouseMoved>()) {
            handleMouseMoved(*mm);
        }

        if (const auto* mr = event->getIf<sf::Event::MouseButtonReleased>()) {
            handleMouseReleased(*mr);
        }
    }
}

void Game::handleMousePressed(const sf::Event::MouseButtonPressed& mb) {
    if (mb.button != sf::Mouse::Button::Left) {
        return;
    }

    sf::Vector2f mousePos(
        static_cast<float>(mb.position.x),
        static_cast<float>(mb.position.y)
    );

    if (towerMenu_.clickedCatapult(mousePos)) {
        towerMenu_.startDragging(TowerType::Catapult);
    }
    else if (towerMenu_.clickedCannon(mousePos)) {
        towerMenu_.startDragging(TowerType::CannonTower);
    }
    else if (towerMenu_.clickedTank(mousePos)) {
        towerMenu_.startDragging(TowerType::Tank);
    }
}

void Game::handleMouseMoved(const sf::Event::MouseMoved& mm) {
    if (!towerMenu_.isDragging()) {
        return;
    }

    sf::Vector2f mousePos(
        static_cast<float>(mm.position.x),
        static_cast<float>(mm.position.y)
    );

    int col = static_cast<int>(mousePos.x / tileSize_);
    int row = static_cast<int>(mousePos.y / tileSize_);

    bool canPlace = false;

    if (mousePos.x < static_cast<float>(window_.getSize().x) - menuWidth_) {
        canPlace = canPlaceTower(col, row);
    }

    towerMenu_.updateDragPreview(
        mousePos,
        tileSize_,
        menuWidth_,
        window_,
        canPlace
    );
}

void Game::handleMouseReleased(const sf::Event::MouseButtonReleased& mr) {
    if (mr.button != sf::Mouse::Button::Left || !towerMenu_.isDragging()) {
        return;
    }

    sf::Vector2f mousePos(
        static_cast<float>(mr.position.x),
        static_cast<float>(mr.position.y)
    );

    int col = static_cast<int>(mousePos.x / tileSize_);
    int row = static_cast<int>(mousePos.y / tileSize_);

    if (mousePos.x < static_cast<float>(window_.getSize().x) - menuWidth_) {
        if (canPlaceTower(col, row)) {
            placeDraggedTower(col, row);
        }
    }

    towerMenu_.stopDragging();
}