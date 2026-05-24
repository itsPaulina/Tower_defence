#pragma once
#include "GameObject.h"
#include "Enemy.h"
#include <SFML/Graphics.hpp>

class Projectile : public GameObject {
private:
    Enemy* target_;
    int damage_;
    float speed_;
    sf::CircleShape shape_;

public:
    Projectile(sf::Vector2f start, Enemy* target, int damage, float speed);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    void hitTarget();
    int getDamage() const;
};