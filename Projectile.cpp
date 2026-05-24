#include "Projectile.h"
#include <cmath>

Projectile::Projectile(sf::Vector2f start, Enemy* target, int damage, float speed)
    : target_(target), damage_(damage), speed_(speed), shape_(6.f) {
    x_ = start.x;
    y_ = start.y;
    shape_.setOrigin({6.f, 6.f});
    shape_.setFillColor(sf::Color::Yellow);
    shape_.setPosition({x_, y_});
}

void Projectile::update(float dt) {
    if (!target_ || !target_->isActive()) {
        active_ = false;
        return;
    }

    sf::Vector2f targetPos = target_->getPosition();
    sf::Vector2f dir = targetPos - sf::Vector2f{x_, y_};
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (len < 8.f) {
        hitTarget();
        return;
    }

    dir /= len;
    x_ += dir.x * speed_ * dt;
    y_ += dir.y * speed_ * dt;
    shape_.setPosition({x_, y_});
}

void Projectile::draw(sf::RenderWindow& window) {
    window.draw(shape_);
}

sf::FloatRect Projectile::getBounds() const {
    return shape_.getGlobalBounds();
}

void Projectile::hitTarget() {
    if (target_ && target_->isActive()) {
        target_->takeDamage(damage_); // albo inna logika zadawania obrażeń
    }
    active_ = false;
}

int Projectile::getDamage() const {
    return damage_;
}