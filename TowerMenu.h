#pragma once
#include <SFML/Graphics.hpp>

enum class TowerType {
    None,
    Catapult,
    CannonTower,
    Tank
};

class TowerMenu {
public:
    TowerMenu();

    void setup(const sf::RenderWindow& window,
               sf::Texture& catapultTexture,
               sf::Texture& cannonTexture,
               sf::Texture& tankTexture);

    void draw(sf::RenderWindow& window);

    void startDragging(TowerType type);
    void stopDragging();
    bool isDragging() const;
    TowerType getDraggedTower() const;

    void updateDragPreview(sf::Vector2f mousePos,
                           float tileSize,
                           float menuWidth,
                           const sf::RenderWindow& window,
                           bool canPlace);

    bool clickedCatapult(sf::Vector2f mousePos) const;
    bool clickedCannon(sf::Vector2f mousePos) const;
    bool clickedTank(sf::Vector2f mousePos) const;

private:
    float menuWidth_ = 180.f;

    sf::Font font_;
    
    sf::RectangleShape menuBackground_;
    sf::RectangleShape catapultButton_;
    sf::RectangleShape cannonButton_;
    sf::RectangleShape tankButton_;
    sf::RectangleShape dragPreview_;

    sf::Texture* catapultTexture_ = nullptr;
    sf::Texture* cannonTexture_ = nullptr;
    sf::Texture* tankTexture_ = nullptr;

    bool isDragging_ = false;
    TowerType draggedTower_ = TowerType::None;
};