#include "Projectile.h" // Includes the Projectile class declaration.
#include <cmath>        // Needed for std::sqrt.


Projectile::Projectile(sf::Vector2f start, sf::Vector2f targetPos, int damage, float speed)
    : damage_(damage), shape_(6.f) { // Initialize projectile damage and create a circular shape with radius 6.
    x_ = start.x; // Set starting X position.
    y_ = start.y; // Set starting Y position.

    shape_.setOrigin({6.f, 6.f});          // Set shape origin to its center.
    shape_.setFillColor(sf::Color::Yellow); // Set projectile color.
    shape_.setPosition({x_, y_});          // Place shape at starting position.

    sf::Vector2f dir = targetPos - start; // Compute direction from start to target.
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y); // Compute length of that direction vector.

    if (len > 0.f) {      // Check that the target is not exactly at the start position.
        dir /= len;       // Normalize direction vector.
        velocity_ = dir * speed; // Set projectile velocity using normalized direction and speed.
    } else {
        velocity_ = {0.f, 0.f}; // If direction length is zero, projectile does not move.
        active_ = false;        // Deactivate invalid projectile immediately.
    }
}


void Projectile::update(float dt) {
    if (!active_) // Skip update if projectile is inactive.
        return;

    x_ += velocity_.x * dt; // Move in X direction based on velocity and delta time.
    y_ += velocity_.y * dt; // Move in Y direction based on velocity and delta time.
    shape_.setPosition({x_, y_}); // Update projectile shape position.

    lifetime_ -= dt; // Reduce remaining lifetime.
    if (lifetime_ <= 0.f) {
        active_ = false; // Deactivate projectile when its lifetime ends.
    }
}


void Projectile::draw(sf::RenderWindow& window) {
    window.draw(shape_); // Draw projectile shape.
}


sf::FloatRect Projectile::getBounds() const {
    return shape_.getGlobalBounds(); // Return projectile bounds for collision detection.
}


int Projectile::getDamage() const {
    return damage_; // Return how much damage this projectile deals.
}