#include "Enemy.h"
#include <cmath>
#include <iostream>

sf::Vector2f Enemy::tileCenter(const sf::Vector2i& tile) const {
    return {
        tile.x * tileSize_ + tileSize_ / 2.f,
        tile.y * tileSize_ + tileSize_ / 2.f
    };
}

Enemy::Enemy(int hp, float speed, int reward,
             const std::vector<sf::Vector2i>& path,
             float tileSize, sf::Color color, float radius)
    : hp_(hp), speed_(speed), reward_(reward),
      animation_(4, 6.f), path_(path), tileSize_(tileSize),
      pathIndex_(0), shape_(radius) {
    shape_.setOrigin({radius, radius});
    shape_.setFillColor(color);

    auto start = tileCenter(path_.front());
    x_ = start.x;
    y_ = start.y;
    shape_.setPosition({x_, y_});
}

void Enemy::moveAlongPath(float dt) {
    if (pathIndex_ >= path_.size() - 1) {
        active_ = false;
        return;
    }

    sf::Vector2f current{x_, y_};
    sf::Vector2f target = tileCenter(path_[pathIndex_ + 1]);
    sf::Vector2f dir = target - current;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (len < 2.f) {
        ++pathIndex_;
        x_ = target.x;
        y_ = target.y;
    } else {
        dir /= len;
        x_ += dir.x * speed_ * dt;
        y_ += dir.y * speed_ * dt;
    }

    shape_.setPosition({x_, y_});
}

void Enemy::takeDamage(int dmg) {
    hp_ -= dmg;
    if (hp_ <= 0)
        active_ = false;
}

void Enemy::update(float dt) {
    animation_.update(dt);
    moveAlongPath(dt);
}

sf::Vector2f Enemy::getPosition() const {
    return {x_, y_};
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(shape_);
}

sf::FloatRect Enemy::getBounds() const {
    return shape_.getGlobalBounds();
}

bool Enemy::reachedGoal() const {
    return pathIndex_ >= path_.size() - 1;
}

int Enemy::getReward() const {
    return reward_;
}

int Enemy::getHp() const {
    return hp_;
}

Slime::Slime(const std::vector<sf::Vector2i>& path, float tileSize)
    : Enemy(30, 70.f, 10, path, tileSize, sf::Color(0, 200, 100), 16.f) {
}

void Slime::update(float dt) {
    Enemy::update(dt);
}

Goblin::Goblin(const std::vector<sf::Vector2i>& path, float tileSize)
    : Enemy(55, 100.f, 15, path, tileSize, sf::Color(120, 200, 40), 18.f) {
    if (!texture_.loadFromFile("C:\\Users\\spaul\\Desktop\\gob.png")) {
        std::cerr << "Could not load goblin1.png\n";
    }

    sprite_.setTexture(texture_);
    sprite_.setTextureRect(sf::IntRect(
        {0, 0},
        {frameWidth_, frameHeight_}
    ));

    sprite_.setOrigin({
        frameWidth_ / 2.f,
        frameHeight_ / 2.f
    });

    sprite_.setPosition(getPosition());
}

void Goblin::updateDirection(float dx, float dy) {
    if (std::abs(dx) > std::abs(dy)) {
        if (dx > 0)
            currentRow_ = 3; // right
        else
            currentRow_ = 2; // left
    } else {
        if (dy > 0)
            currentRow_ = 0; // down
        else
            currentRow_ = 1; // up
    }
}

void Goblin::updateAnimation(float dt) {
    animationTimer_ += dt;

    if (animationTimer_ >= animationSpeed_) {
        animationTimer_ = 0.f;
        currentFrame_ = (currentFrame_ + 1) % frameCount_;

        sprite_.setTextureRect(sf::IntRect(
            {currentFrame_ * frameWidth_, currentRow_ * frameHeight_},
            {frameWidth_, frameHeight_}
        ));
    }
}

void Goblin::update(float dt) {
    sf::Vector2f oldPos = getPosition();

    Enemy::update(dt);

    sf::Vector2f newPos = getPosition();
    float dx = newPos.x - oldPos.x;
    float dy = newPos.y - oldPos.y;

    updateDirection(dx, dy);
    updateAnimation(dt);

    sprite_.setPosition(newPos);
}

void Goblin::draw(sf::RenderWindow& window) {
    window.draw(sprite_);
}

Wolf::Wolf(const std::vector<sf::Vector2i>& path, float tileSize)
    : Enemy(90, 135.f, 20, path, tileSize, sf::Color(190, 190, 190), 20.f) {
}

void Wolf::update(float dt) {
    Enemy::update(dt);
}