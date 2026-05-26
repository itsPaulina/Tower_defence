#include "Tower.h"
#include <cmath>

// Tower constructor.
// Sets range, damage, cooldown, and visual appearance.
Tower::Tower(sf::Vector2f pos, float range, int damage, float cooldown, sf::Color color)
    : range_(range), damage_(damage), cooldown_(cooldown), cooldownTimer_(0.f),
      shape_({64.f, 64.f}) {
    // Store tower position.
    x_ = pos.x;
    y_ = pos.y;

    // Set origin to the center of the shape.
    shape_.setOrigin({32.f, 32.f});

    // Set initial position and color.
    shape_.setPosition({x_, y_});
    shape_.setFillColor(color);
}

void Tower::setTexture(const sf::Texture* texture) {
    shape_.setTexture(texture, true);
    shape_.setFillColor(sf::Color::White);
}

// Updates the tower.
// Decreases the cooldown timer until the tower can fire again.
void Tower::update(float dt) {
    if (cooldownTimer_ > 0.f)
        cooldownTimer_ -= dt;
}

// Draws the tower on the screen.
void Tower::draw(sf::RenderWindow& window) {
    window.draw(shape_);
}

// Returns the tower's collision bounds.
// Useful for collision checks or mouse interaction.
sf::FloatRect Tower::getBounds() const {
    return shape_.getGlobalBounds();
}

// Finds the nearest active enemy inside the tower's range.
Enemy* Tower::findTarget(const std::vector<std::unique_ptr<GameObject>>& objects) const {
    Enemy* best = nullptr;
    float bestDist = range_;

    // Loop through all game objects.
    for (const auto& obj : objects) {
        // Try to cast the object to Enemy.
        Enemy* e = dynamic_cast<Enemy*>(obj.get());
        if (!e || !e->isActive())
            continue;

        // Get the enemy's current position.
        auto p = e->getPosition();

        // Compute distance from tower to enemy.
        float dx = p.x - x_;
        float dy = p.y - y_;
        float dist = std::sqrt(dx * dx + dy * dy);

        // Keep the closest enemy found so far.
        if (dist <= bestDist) {
            bestDist = dist;
            best = e;
        }
    }

    return best;
}

// Tower attack logic.
// If cooldown is over and a target is found,
// create a projectile that follows the enemy.
void Tower::attack(std::vector<std::unique_ptr<GameObject>>& objects) {
    if (cooldownTimer_ > 0.f)
        return;

    Enemy* target = findTarget(objects);
    if (!target)
        return;

    objects.push_back(std::make_unique<Projectile>(
        sf::Vector2f{x_, y_},
        target->getPosition(),
        damage_,
        340.f
    ));

    cooldownTimer_ = cooldown_;
}

// Catapult tower with its own stats.
Catapult::Catapult(sf::Vector2f pos)
    : Tower(pos, 170.f, 20, 1.2f, sf::Color(160, 110, 60)) {}

// Catapult update.
// Uses the base Tower logic for now.
void Catapult::update(float dt) {
    Tower::update(dt);
}

// CannonTower with faster firing and longer range.
CannonTower::CannonTower(sf::Vector2f pos)
    : Tower(pos, 210.f, 14, 0.55f, sf::Color(70, 130, 255)) {}

// CannonTower update.
void CannonTower::update(float dt) {
    Tower::update(dt);
}

// Tank tower: slow but powerful.
Tank::Tank(sf::Vector2f pos)
    : Tower(pos, 120.f, 40, 1.8f, sf::Color(170, 60, 60)) {}

// Tank update.
void Tank::update(float dt) {
    Tower::update(dt);
}