#pragma once // Ensures this header file is included only once.

#include "GameObject.h"     // Base class for all game objects.
#include <SFML/Graphics.hpp> // Provides SFML classes like Vector2f, FloatRect, and CircleShape.


class Projectile : public GameObject { // Class representing a moving projectile.
private:
    sf::Vector2f velocity_;   // Direction and speed of the projectile.
    int damage_;              // Damage dealt when the projectile hits an enemy.
    float lifetime_ = 2.0f;   // Time before the projectile disappears automatically.
    sf::CircleShape shape_;   // Visual shape used to draw the projectile.

public:
    Projectile(sf::Vector2f start, sf::Vector2f targetPos, int damage, float speed); // Creates a projectile from a start point toward a target.

    void update(float dt) override;               // Updates projectile movement and lifetime.
    void draw(sf::RenderWindow& window) override; // Draws the projectile.
    sf::FloatRect getBounds() const override;     // Returns projectile bounds for collision checks.
    int getDamage() const;                        // Returns projectile damage value.
};