#pragma once // Prevents multiple inclusion of this header file.

#include <SFML/Graphics.hpp> // Provides SFML graphics classes like RenderWindow, Texture, Font, and RectangleShape.


// Defines all possible tower types available in the menu.
enum class TowerType {
    None,        // No tower selected.
    Catapult,    // Catapult tower type.
    CannonTower, // Cannon tower type.
    Tank         // Tank tower type.
};


// Handles the tower selection menu and drag-and-drop preview.
class TowerMenu {
public:
    TowerMenu(); // Creates the tower menu object.

    // Initializes the menu with window size and tower textures.
    void setup(const sf::RenderWindow& window,
               sf::Texture& catapultTexture,
               sf::Texture& cannonTexture,
               sf::Texture& tankTexture);

    void draw(sf::RenderWindow& window); // Draws the menu and tower price labels.

    void startDragging(TowerType type); // Starts dragging the selected tower.
    void stopDragging();                // Stops dragging the current tower.
    bool isDragging() const;            // Returns true if a tower is currently being dragged.
    TowerType getDraggedTower() const;  // Returns the type of the dragged tower.

    // Updates the drag preview position and color depending on placement validity.
    void updateDragPreview(sf::Vector2f mousePos,
                           float tileSize,
                           float menuWidth,
                           const sf::RenderWindow& window,
                           bool canPlace);

    bool clickedCatapult(sf::Vector2f mousePos) const; // Checks if the catapult button was clicked.
    bool clickedCannon(sf::Vector2f mousePos) const;   // Checks if the cannon button was clicked.
    bool clickedTank(sf::Vector2f mousePos) const;     // Checks if the tank button was clicked.

private:
    float menuWidth_ = 180.f; // Width of the side menu panel.

    sf::Font font_; // Font used to display prices.

    sf::RectangleShape menuBackground_; // Background rectangle of the menu.
    sf::RectangleShape catapultButton_; // Button for the catapult tower.
    sf::RectangleShape cannonButton_;   // Button for the cannon tower.
    sf::RectangleShape tankButton_;     // Button for the tank tower.
    sf::RectangleShape dragPreview_;    // Preview rectangle shown while dragging a tower.

    sf::Texture* catapultTexture_ = nullptr; // Pointer to catapult texture.
    sf::Texture* cannonTexture_ = nullptr;   // Pointer to cannon texture.
    sf::Texture* tankTexture_ = nullptr;     // Pointer to tank texture.

    bool isDragging_ = false;                  // Indicates whether dragging is active.
    TowerType draggedTower_ = TowerType::None; // Stores the currently selected tower type.
};