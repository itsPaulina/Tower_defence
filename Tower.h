#pragma once

#include "GameObject.h"
#include "Enemy.h"
#include "Projectile.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Tower : public GameObject {
protected:
    float range_;
    int damage_;
    float cooldown_;
    float cooldownTimer_;
    sf::RectangleShape shape_;

public:
    Tower(sf::Vector2f pos, float range, int damage, float cooldown, sf::Color color);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;

    void attack(std::vector<std::unique_ptr<GameObject>>& objects);
    Enemy* findTarget(const std::vector<std::unique_ptr<GameObject>>& objects) const;

    void setTexture(const sf::Texture* texture);
};

class Catapult : public Tower {
public:
    Catapult(sf::Vector2f pos);
    void update(float dt) override;
};

class CannonTower : public Tower {
public:
    CannonTower(sf::Vector2f pos);
    void update(float dt) override;
};

class Tank : public Tower {
public:
    Tank(sf::Vector2f pos);
    void update(float dt) override;
};