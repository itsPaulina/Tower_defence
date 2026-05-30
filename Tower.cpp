#include "Tower.h" // Includes the Tower class declarations.
#include <cmath>   // Needed for std::sqrt.
#include <memory>

// Tower constructor.
// Sets range, damage, cooldown, and visual appearance.
Tower::Tower(sf::Vector2f pos, float range, int damage, float cooldown, sf::Color color)
    : range_(range), damage_(damage), cooldown_(cooldown), cooldownTimer_(0.f),
      shape_({64.f, 64.f}) {
    x_ = pos.x; // Store tower X position.
    y_ = pos.y; // Store tower Y position.

    shape_.setOrigin({32.f, 32.f}); // Set origin to the center of the tower shape.

    shape_.setPosition({x_, y_});   // Place the tower on the map.
    shape_.setFillColor(color);     // Set default tower color.
}


void Tower::setTexture(const sf::Texture* texture) {
    shape_.setTexture(texture, true);     // Apply texture to the tower shape.
    shape_.setFillColor(sf::Color::White); // Reset fill color so texture colors are visible.
}


// Updates the tower.
// Decreases the cooldown timer until the tower can fire again.
void Tower::update(float dt) {
    if (cooldownTimer_ > 0.f)
        cooldownTimer_ -= dt; // Reduce remaining cooldown time.
}


// Draws the tower on the screen.
void Tower::draw(sf::RenderWindow& window) {
    window.draw(shape_); // Draw the tower shape.
}


// Returns the tower's collision bounds.
// Useful for collision checks or mouse interaction.
sf::FloatRect Tower::getBounds() const {
    return shape_.getGlobalBounds(); // Return tower bounds in world coordinates.
}


// Finds the nearest active enemy inside the tower's range.
Enemy* Tower::findTarget(const std::vector<std::unique_ptr<GameObject>>& objects) const {
    Enemy* best = nullptr;   // Stores the current best target.
    float bestDist = range_; // Stores the distance to the best target found so far.

    for (const auto& obj : objects) { // Loop through all game objects.
        Enemy* e = dynamic_cast<Enemy*>(obj.get()); // Try to treat object as an enemy.
        if (!e || !e->isActive())
            continue; // Skip objects that are not active enemies.

        auto p = e->getPosition(); // Get enemy position.

        float dx = p.x - x_; // Horizontal distance from tower to enemy.
        float dy = p.y - y_; // Vertical distance from tower to enemy.
        float dist = std::sqrt(dx * dx + dy * dy); // Compute actual distance.

        if (dist <= bestDist) { // Keep the closest enemy inside range.
            bestDist = dist;
            best = e;
        }
    }

    return best; // Return the chosen target or nullptr if none found.
}


// Tower attack logic.
// If cooldown is over and a target is found,
// create a projectile that follows the enemy.
void Tower::attack(std::vector<std::unique_ptr<GameObject>>& objects) {
    if (cooldownTimer_ > 0.f)
        return; // Do not attack if tower is still reloading.

    Enemy* target = findTarget(objects); // Look for an enemy in range.
    if (!target)
        return; // Stop if no valid target exists.

    objects.push_back(std::make_unique<Projectile>(
        sf::Vector2f{x_, y_},    // Projectile start position.
        target->getPosition(),   // Target position at the moment of firing.
        damage_,                 // Damage dealt by projectile.
        340.f                    // Projectile speed.
    ));

    cooldownTimer_ = cooldown_; // Reset cooldown after firing.
}


// Catapult tower with its own stats.
Catapult::Catapult(sf::Vector2f pos)
    : Tower(pos, 170.f, 20, 1.8f, sf::Color(160, 110, 60)) {}


// Catapult update.
// Uses the base Tower logic for now.
void Catapult::update(float dt) {
    Tower::update(dt); // Reuse base tower cooldown logic.
}


// CannonTower with faster firing and longer range.
CannonTower::CannonTower(sf::Vector2f pos)
    : Tower(pos, 210.f, 14, 0.9f, sf::Color(70, 130, 255)) {}


// CannonTower update.
void CannonTower::update(float dt) {
    Tower::update(dt); // Reuse base tower cooldown logic.
}


// Tank tower: slow but powerful.
Tank::Tank(sf::Vector2f pos)
    : Tower(pos, 120.f, 40, 2.4f, sf::Color(170, 60, 60)) {}


// Tank update.
void Tank::update(float dt) {
    Tower::update(dt); // Reuse base tower cooldown logic.
}