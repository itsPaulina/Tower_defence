#include "Projectile.h"
#include <cmath>

Projectile::Projectile(sf::Vector2f start, sf::Vector2f targetPos, int damage, float speed)
    : damage_(damage), shape_(6.f) {
    x_ = start.x;
    y_ = start.y;

    shape_.setOrigin({6.f, 6.f});
    shape_.setFillColor(sf::Color::Yellow);
    shape_.setPosition({x_, y_});

    sf::Vector2f dir = targetPos - start;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (len > 0.f) {
        dir /= len;
        velocity_ = dir * speed;
    } else {
        velocity_ = {0.f, 0.f};
        active_ = false;
    }
}

void Projectile::update(float dt) {
    if (!active_)
        return;

    x_ += velocity_.x * dt;
    y_ += velocity_.y * dt;
    shape_.setPosition({x_, y_});

    lifetime_ -= dt;
    if (lifetime_ <= 0.f) {
        active_ = false;
    }
}

void Projectile::draw(sf::RenderWindow& window) {
    window.draw(shape_);
}

sf::FloatRect Projectile::getBounds() const {
    return shape_.getGlobalBounds();
}

int Projectile::getDamage() const {
    return damage_;
}