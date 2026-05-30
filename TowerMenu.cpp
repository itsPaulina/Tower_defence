#include "TowerMenu.h" // Gives access to the TowerMenu class declaration.
#include <iostream>    // Allows printing error messages with std::cerr.


// Default constructor for the tower menu.
// No special setup is needed here because most initialization
// is done later in the setup() function.
TowerMenu::TowerMenu() {
}


// Prepares the tower menu after the window and textures are available.
// This function stores texture pointers, loads the font,
// creates the menu background, places the tower buttons,
// and configures the drag preview object.
void TowerMenu::setup(const sf::RenderWindow& window,
                      sf::Texture& catapultTexture,
                      sf::Texture& cannonTexture,
                      sf::Texture& tankTexture) {
    // Save pointers to the tower textures so they can be reused later.
    catapultTexture_ = &catapultTexture;
    cannonTexture_ = &cannonTexture;
    tankTexture_ = &tankTexture;

    // Load the font used for displaying tower prices.
    // If loading fails, print an error in the console.
    if (!font_.openFromFile("photos/Inter_18pt-BlackItalic.ttf")) {
        std::cerr << "Could not load font for TowerMenu\n";
    }

    // Standard size used for the dragged tower preview.
    const sf::Vector2f towerSize(64.f, 64.f);

    // Configure the right-side menu panel.
    // It should be as tall as the window and fixed to the right edge.
    menuBackground_.setSize({menuWidth_, static_cast<float>(window.getSize().y)});
    menuBackground_.setPosition({
        static_cast<float>(window.getSize().x) - menuWidth_,
        0.f
    });
    menuBackground_.setFillColor(sf::Color(40, 40, 40)); // Dark gray background.

    // Compute a common X position for the tower buttons
    // so all buttons are aligned inside the menu.
    float menuX = static_cast<float>(window.getSize().x) - menuWidth_ + 50.f;

    // Set up the catapult button.
    catapultButton_.setSize({80.f, 80.f});
    catapultButton_.setPosition({menuX, 120.f});
    catapultButton_.setTexture(catapultTexture_, true);
    catapultButton_.setFillColor(sf::Color::White);

    // Set up the cannon tower button.
    cannonButton_.setSize({80.f, 80.f});
    cannonButton_.setPosition({menuX, 270.f});
    cannonButton_.setTexture(cannonTexture_, true);
    cannonButton_.setFillColor(sf::Color::White);

    // Set up the tank button.
    tankButton_.setSize({80.f, 80.f});
    tankButton_.setPosition({menuX, 420.f});
    tankButton_.setTexture(tankTexture_, true);
    tankButton_.setFillColor(sf::Color::White);

    // Configure the preview object shown while dragging a tower.
    // Its origin is centered so it snaps nicely to tile centers.
    dragPreview_.setSize(towerSize);
    dragPreview_.setOrigin({towerSize.x / 2.f, towerSize.y / 2.f});
    dragPreview_.setFillColor(sf::Color(255, 255, 255, 180)); // Semi-transparent preview.
}


// Draws the entire tower menu.
// This includes the background panel, tower buttons,
// price labels, and the drag preview if dragging is active.
void TowerMenu::draw(sf::RenderWindow& window) {
    // Draw the menu panel and all tower buttons first.
    window.draw(menuBackground_);
    window.draw(catapultButton_);
    window.draw(cannonButton_);
    window.draw(tankButton_);

    // Create the catapult price text.
    sf::Text catapultPrice(font_, "40 gold", 18);
    catapultPrice.setFillColor(sf::Color::Yellow);
    catapultPrice.setPosition({
        catapultButton_.getPosition().x + 8.f,
        catapultButton_.getPosition().y + catapultButton_.getSize().y + 6.f
    });

    // Create the cannon tower price text.
    sf::Text cannonPrice(font_, "60 gold", 18);
    cannonPrice.setFillColor(sf::Color::Yellow);
    cannonPrice.setPosition({
        cannonButton_.getPosition().x + 8.f,
        cannonButton_.getPosition().y + cannonButton_.getSize().y + 6.f
    });

    // Create the tank price text.
    sf::Text tankPrice(font_, "80 gold", 18);
    tankPrice.setFillColor(sf::Color::Yellow);
    tankPrice.setPosition({
        tankButton_.getPosition().x + 8.f,
        tankButton_.getPosition().y + tankButton_.getSize().y + 6.f
    });

    // Draw all price labels.
    window.draw(catapultPrice);
    window.draw(cannonPrice);
    window.draw(tankPrice);

    // If the player is dragging a tower,
    // also draw the preview on the map.
    if (isDragging_) {
        window.draw(dragPreview_);
    }
}


// Starts dragging a tower selected from the menu.
// The function remembers which tower type is being dragged,
// enables drag mode, and sets the correct preview texture.
void TowerMenu::startDragging(TowerType type) {
    draggedTower_ = type;
    isDragging_ = true;

    // Select the correct texture for the preview object.
    switch (draggedTower_) {
    case TowerType::Catapult:
        dragPreview_.setTexture(catapultTexture_, true);
        break;
    case TowerType::CannonTower:
        dragPreview_.setTexture(cannonTexture_, true);
        break;
    case TowerType::Tank:
        dragPreview_.setTexture(tankTexture_, true);
        break;
    default:
        break;
    }

    // Reset preview color to a normal semi-transparent white.
    dragPreview_.setFillColor(sf::Color(255, 255, 255, 180));
}


// Stops dragging the current tower.
// Drag mode is disabled and the selected tower type is cleared.
void TowerMenu::stopDragging() {
    isDragging_ = false;
    draggedTower_ = TowerType::None;
}


// Returns true if a tower is currently being dragged.
bool TowerMenu::isDragging() const {
    return isDragging_;
}


// Returns the type of tower currently selected for dragging.
TowerType TowerMenu::getDraggedTower() const {
    return draggedTower_;
}


// Updates the drag preview while the mouse moves.
// The preview is snapped to the grid and colored
// depending on whether the tower can be placed there.
void TowerMenu::updateDragPreview(sf::Vector2f mousePos,
                                  float tileSize,
                                  float menuWidth,
                                  const sf::RenderWindow& window,
                                  bool canPlace) {
    // Convert mouse position from pixels to tile coordinates.
    int col = static_cast<int>(mousePos.x / tileSize);
    int row = static_cast<int>(mousePos.y / tileSize);

    // Convert tile coordinates back to the center of that tile.
    // This makes the preview snap cleanly to the board.
    sf::Vector2f snappedPos{
        col * tileSize + tileSize / 2.f,
        row * tileSize + tileSize / 2.f
    };

    // Move preview to the snapped grid position.
    dragPreview_.setPosition(snappedPos);

    // If the mouse is inside the side menu area,
    // placement is automatically invalid.
    if (mousePos.x >= static_cast<float>(window.getSize().x) - menuWidth) {
        dragPreview_.setFillColor(sf::Color(255, 0, 0, 140));
        return;
    }

    // Make sure the preview still uses the correct texture
    // for the currently selected tower type.
    switch (draggedTower_) {
    case TowerType::Catapult:
        dragPreview_.setTexture(catapultTexture_, true);
        break;
    case TowerType::CannonTower:
        dragPreview_.setTexture(cannonTexture_, true);
        break;
    case TowerType::Tank:
        dragPreview_.setTexture(tankTexture_, true);
        break;
    default:
        break;
    }

    // Change preview color based on placement validity:
    // white = valid, red = invalid.
    if (canPlace) {
        dragPreview_.setFillColor(sf::Color(255, 255, 255, 180));
    } else {
        dragPreview_.setFillColor(sf::Color(255, 0, 0, 140));
    }
}


// Checks whether the mouse clicked inside the catapult button.
bool TowerMenu::clickedCatapult(sf::Vector2f mousePos) const {
    return catapultButton_.getGlobalBounds().contains(mousePos);
}


// Checks whether the mouse clicked inside the cannon tower button.
bool TowerMenu::clickedCannon(sf::Vector2f mousePos) const {
    return cannonButton_.getGlobalBounds().contains(mousePos);
}


// Checks whether the mouse clicked inside the tank button.
bool TowerMenu::clickedTank(sf::Vector2f mousePos) const {
    return tankButton_.getGlobalBounds().contains(mousePos);
}