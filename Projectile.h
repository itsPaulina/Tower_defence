#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

class Projectile : public GameObject {
private:
    sf::Vector2f velocity_;
    int damage_;
    float lifetime_ = 2.0f;
    sf::CircleShape shape_;
   

public:
    Projectile(sf::Vector2f start, sf::Vector2f targetPos, int damage, float speed);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    int getDamage() const;
    
};