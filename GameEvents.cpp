#include "Game.h"    // Includes the Game class declaration.
#include <optional>  // Needed for std::optional.


void Game::handleEvents() {
    while (const std::optional<sf::Event> event = window_.pollEvent()) { // Process all events currently in the event queue.
        if (event->is<sf::Event::Closed>()) { // Check if the user requested to close the window.
            window_.close(); // Close the game window.
        }

        if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) { // Check if this is a mouse button press event.
            handleMousePressed(*mb); // Pass the event to the mouse press handler.
        }

        if (state_ == GameState::Playing) { // Handle these events only during gameplay.
            if (const auto* mm = event->getIf<sf::Event::MouseMoved>()) { // Check if the mouse was moved.
                handleMouseMoved(*mm); // Update drag preview or other mouse-move logic.
            }

            if (const auto* mr = event->getIf<sf::Event::MouseButtonReleased>()) { // Check if a mouse button was released.
                handleMouseReleased(*mr); // Pass the event to the release handler.
            }
        }
    }
}


void Game::handleMousePressed(const sf::Event::MouseButtonPressed& mb) {
    if (mb.button != sf::Mouse::Button::Left) { // If the pressed button is not the left mouse button, ignore the event.
        return;
    }

    sf::Vector2f mousePos(
        static_cast<float>(mb.position.x), // Convert mouse X position to float.
        static_cast<float>(mb.position.y)  // Convert mouse Y position to float.
    );

    if (state_ == GameState::MainMenu) { // If currently in the main menu,
        handleMainMenuClick(mousePos);   // handle main menu click.
        return;
    }

    if (state_ == GameState::DifficultyMenu) { // If currently in the difficulty menu,
        handleMenuClick(mousePos);             // handle difficulty menu click.
        return;
    }

    if (state_ != GameState::Playing) { // Ignore clicks if the game is not in playing state.
        return;
    }

    if (towerMenu_.clickedCatapult(mousePos)) { // Check if the catapult icon was clicked.
        towerMenu_.startDragging(TowerType::Catapult); // Start dragging a catapult tower.
    }
    else if (towerMenu_.clickedCannon(mousePos)) { // Check if the cannon icon was clicked.
        towerMenu_.startDragging(TowerType::CannonTower); // Start dragging a cannon tower.
    }
    else if (towerMenu_.clickedTank(mousePos)) { // Check if the tank icon was clicked.
        towerMenu_.startDragging(TowerType::Tank); // Start dragging a tank tower.
    }
}


void Game::handleMouseMoved(const sf::Event::MouseMoved& mm) {
    if (!towerMenu_.isDragging()) { // Do nothing if no tower is currently being dragged.
        return;
    }

    sf::Vector2f mousePos(
        static_cast<float>(mm.position.x), // Convert mouse X to float.
        static_cast<float>(mm.position.y)  // Convert mouse Y to float.
    );

    int col = static_cast<int>(mousePos.x / tileSize_); // Convert X position to tile column.
    int row = static_cast<int>(mousePos.y / tileSize_); // Convert Y position to tile row.

    bool canPlace = false; // Default to not allowed.

    if (mousePos.x < static_cast<float>(window_.getSize().x) - menuWidth_) { // Check if mouse is inside the map area, not the side menu.
        canPlace = canPlaceTower(col, row); // Check whether tower placement is valid on this tile.
    }

    towerMenu_.updateDragPreview(
        mousePos,   // Current mouse position.
        tileSize_,  // Tile size used for snapping preview.
        menuWidth_, // Width of side menu.
        window_,    // Window used for bounds checking.
        canPlace    // Whether placement is allowed.
    );
}


void Game::handleMouseReleased(const sf::Event::MouseButtonReleased& mr) {
    if (mr.button != sf::Mouse::Button::Left || !towerMenu_.isDragging()) { // Ignore if it is not left mouse or if nothing is being dragged.
        return;
    }

    sf::Vector2f mousePos(
        static_cast<float>(mr.position.x), // Convert release X position to float.
        static_cast<float>(mr.position.y)  // Convert release Y position to float.
    );

    int col = static_cast<int>(mousePos.x / tileSize_); // Convert X position to tile column.
    int row = static_cast<int>(mousePos.y / tileSize_); // Convert Y position to tile row.

    if (mousePos.x < static_cast<float>(window_.getSize().x) - menuWidth_) { // Make sure release happened inside the playable area.
        if (canPlaceTower(col, row)) { // Check if tower can be placed here.
            placeDraggedTower(col, row); // Place the dragged tower on the selected tile.
        }
    }

    towerMenu_.stopDragging(); // Stop dragging no matter whether placement succeeded or not.
}