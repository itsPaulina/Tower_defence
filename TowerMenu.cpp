#include "TowerMenu.h"
#include <iostream>

TowerMenu::TowerMenu() {
}

void TowerMenu::setup(const sf::RenderWindow& window,
                      sf::Texture& catapultTexture,
                      sf::Texture& cannonTexture,
                      sf::Texture& tankTexture) {
    catapultTexture_ = &catapultTexture;
    cannonTexture_ = &cannonTexture;
    tankTexture_ = &tankTexture;

    if (!font_.openFromFile("C:\\Users\\spaul\\Desktop\\Inter_18pt-BlackItalic.ttf")) {
        std::cerr << "Could not load font for TowerMenu\n";
    }
    
    const sf::Vector2f towerSize(64.f, 64.f);

    menuBackground_.setSize({menuWidth_, static_cast<float>(window.getSize().y)});
    menuBackground_.setPosition({static_cast<float>(window.getSize().x) - menuWidth_, 0.f});
    menuBackground_.setFillColor(sf::Color(40, 40, 40));

    float menuX = static_cast<float>(window.getSize().x) - menuWidth_ + 50.f;

    catapultButton_.setSize({80.f, 80.f});
    catapultButton_.setPosition({menuX, 120.f});
    catapultButton_.setTexture(catapultTexture_, true);
    catapultButton_.setFillColor(sf::Color::White);

    cannonButton_.setSize({80.f, 80.f});
    cannonButton_.setPosition({menuX, 270.f});
    cannonButton_.setTexture(cannonTexture_, true);
    cannonButton_.setFillColor(sf::Color::White);

    tankButton_.setSize({80.f, 80.f});
    tankButton_.setPosition({menuX, 420.f});
    tankButton_.setTexture(tankTexture_, true);
    tankButton_.setFillColor(sf::Color::White);

    dragPreview_.setSize(towerSize);
    dragPreview_.setOrigin({towerSize.x / 2.f, towerSize.y / 2.f});
    dragPreview_.setFillColor(sf::Color(255, 255, 255, 180));
}

void TowerMenu::draw(sf::RenderWindow& window) {
    window.draw(menuBackground_);
    window.draw(catapultButton_);
    window.draw(cannonButton_);
    window.draw(tankButton_);

    sf::Text catapultPrice(font_, "40 gold", 18);
    catapultPrice.setFillColor(sf::Color::Yellow);
    catapultPrice.setPosition({
        catapultButton_.getPosition().x + 8.f,
        catapultButton_.getPosition().y + catapultButton_.getSize().y + 6.f
    });

    sf::Text cannonPrice(font_, "60 gold", 18);
    cannonPrice.setFillColor(sf::Color::Yellow);
    cannonPrice.setPosition({
        cannonButton_.getPosition().x + 8.f,
        cannonButton_.getPosition().y + cannonButton_.getSize().y + 6.f
    });

    sf::Text tankPrice(font_, "80 gold", 18);
    tankPrice.setFillColor(sf::Color::Yellow);
    tankPrice.setPosition({
        tankButton_.getPosition().x + 8.f,
        tankButton_.getPosition().y + tankButton_.getSize().y + 6.f
    });

    window.draw(catapultPrice);
    window.draw(cannonPrice);
    window.draw(tankPrice);

    if (isDragging_) {
        window.draw(dragPreview_);
    }
}

void TowerMenu::startDragging(TowerType type) {
    draggedTower_ = type;
    isDragging_ = true;

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

    dragPreview_.setFillColor(sf::Color(255, 255, 255, 180));
}

void TowerMenu::stopDragging() {
    isDragging_ = false;
    draggedTower_ = TowerType::None;
}

bool TowerMenu::isDragging() const {
    return isDragging_;
}

TowerType TowerMenu::getDraggedTower() const {
    return draggedTower_;
}

void TowerMenu::updateDragPreview(sf::Vector2f mousePos,
                                  float tileSize,
                                  float menuWidth,
                                  const sf::RenderWindow& window,
                                  bool canPlace) {
    int col = static_cast<int>(mousePos.x / tileSize);
    int row = static_cast<int>(mousePos.y / tileSize);

    sf::Vector2f snappedPos{
        col * tileSize + tileSize / 2.f,
        row * tileSize + tileSize / 2.f
    };

    dragPreview_.setPosition(snappedPos);

    if (mousePos.x >= static_cast<float>(window.getSize().x) - menuWidth) {
        dragPreview_.setFillColor(sf::Color(255, 0, 0, 140));
        return;
    }

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

    if (canPlace) {
        dragPreview_.setFillColor(sf::Color(255, 255, 255, 180));
    } else {
        dragPreview_.setFillColor(sf::Color(255, 0, 0, 140));
    }
}

bool TowerMenu::clickedCatapult(sf::Vector2f mousePos) const {
    return catapultButton_.getGlobalBounds().contains(mousePos);
}

bool TowerMenu::clickedCannon(sf::Vector2f mousePos) const {
    return cannonButton_.getGlobalBounds().contains(mousePos);
}

bool TowerMenu::clickedTank(sf::Vector2f mousePos) const {
    return tankButton_.getGlobalBounds().contains(mousePos);
}