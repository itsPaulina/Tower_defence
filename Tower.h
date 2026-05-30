#pragma once // Ensures this header file is included only once.

#include "GameObject.h" // Base class for all game objects.
#include "Enemy.h"      // Needed for Enemy target selection.
#include "Projectile.h" // Needed for creating projectiles.

#include <vector> // Provides std::vector.
#include <memory> // Provides std::unique_ptr.


class Tower : public GameObject { // Base class for all tower types.
public:
    // Creates a tower at a given position with chosen stats and color.
    Tower(sf::Vector2f pos, float range, int damage, float cooldown, sf::Color color);

    void update(float dt) override;               // Updates the tower cooldown timer.
    void draw(sf::RenderWindow& window) override; // Draws the tower.
    sf::FloatRect getBounds() const override;     // Returns tower bounds for collisions or mouse checks.

    void setTexture(const sf::Texture* texture); // Sets a texture for the tower shape.

    Enemy* findTarget(const std::vector<std::unique_ptr<GameObject>>& objects) const; // Finds an enemy inside range.
    void attack(std::vector<std::unique_ptr<GameObject>>& objects); // Attacks by creating a projectile.

    bool isActive() const override { return true; } // Towers are always treated as active.

protected:
    float range_;         // Maximum attack range.
    int damage_;          // Damage dealt by each projectile.
    float cooldown_;      // Time between attacks.
    float cooldownTimer_; // Current remaining cooldown time.

private:
    sf::RectangleShape shape_; // Visual shape of the tower.
};


class Catapult : public Tower { // Tower with medium range and solid damage.
public:
    explicit Catapult(sf::Vector2f pos); // Creates a catapult tower.
    void update(float dt) override;      // Updates catapult logic.
};


class CannonTower : public Tower { // Tower with longer range and faster attacks.
public:
    explicit CannonTower(sf::Vector2f pos); // Creates a cannon tower.
    void update(float dt) override;         // Updates cannon tower logic.
};


class Tank : public Tower { // Tower with short range but high damage.
public:
    explicit Tank(sf::Vector2f pos); // Creates a tank tower.
    void update(float dt) override;  // Updates tank logic.
};