#include "Enemy.h"
#include <cmath>
#include <iostream>

// Rezerwacja pamięci dla tekstur statycznych komponentów klas
sf::Texture Slime::texture_;
sf::Texture Goblin::texture_;
sf::Texture Wolf::texture_;

sf::Vector2f Enemy::tileCenter(const sf::Vector2i& tile) const {
    return {
        tile.x * tileSize_ + tileSize_ / 2.f,
        tile.y * tileSize_ + tileSize_ / 2.f
    };
}

Enemy::Enemy(int hp, float speed, int reward,
             const std::vector<sf::Vector2i>& path,
             float tileSize, sf::Color color, float radius)
    : hp_(hp), maxHp_(hp), speed_(speed), reward_(reward),
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

// ========================= SLIME =========================

Slime::Slime(const std::vector<sf::Vector2i>& path, float tileSize)
    : Enemy(45, 50.f, 10, path, tileSize, sf::Color::White, 16.f),
      sprite_(texture_) 
{
    if (texture_.getSize().x == 0) {
        if (!texture_.loadFromFile("C:\\Users\\spaul\\Desktop\\slime.png")) {
            std::cerr << "Could not load slime.png\n";
        }
    }

    sprite_.setTextureRect(sf::IntRect(
        {0, 0},
        {frameWidth_, frameHeight_}
    ));

    sprite_.setOrigin({
        frameWidth_ / 2.f,
        frameHeight_ / 2.f
    });

    sf::Vector2f slimeScale(
        (tileSize * 0.7f) / frameWidth_,
        (tileSize * 0.7f) / frameHeight_
    );
    sprite_.setScale(slimeScale);

    sprite_.setPosition(getPosition());
}

void Slime::updateDirection(float dx, float dy) {
    if (std::abs(dx) > std::abs(dy)) {
        if (dx > 0)
            currentRow_ = 1; // right
        else
            currentRow_ = 3; // left
    } else {
        if (dy > 0)
            currentRow_ = 0; // down/front
        else
            currentRow_ = 2; // up/back
    }
}

void Slime::updateAnimation(float dt) {
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

void Slime::update(float dt) {
    sf::Vector2f oldPos = getPosition();

    Enemy::update(dt);

    sf::Vector2f newPos = getPosition();
    float dx = newPos.x - oldPos.x;
    float dy = newPos.y - oldPos.y;

    updateDirection(dx, dy);
    updateAnimation(dt);

    sprite_.setPosition(newPos);
}

void Slime::draw(sf::RenderWindow& window) {
    window.draw(sprite_);
     drawHealthBar(window);
}

sf::FloatRect Slime::getBounds() const {
    return sprite_.getGlobalBounds();
}

// ========================= GOBLIN =========================

Goblin::Goblin(const std::vector<sf::Vector2i>& path, float tileSize)
    : Enemy(80, 70.f, 15, path, tileSize, sf::Color(120, 200, 40), 18.f),
      sprite_(texture_)
{
    if (texture_.getSize().x == 0) {
        if (!texture_.loadFromFile("C:\\Users\\spaul\\Desktop\\gob.png")) {
            std::cerr << "Could not load gob.png\n";
        }
    }

    sprite_.setTextureRect(sf::IntRect(
        {0, 0},
        {frameWidth_, frameHeight_}
    ));

    sprite_.setOrigin({
        frameWidth_ / 2.f,
        frameHeight_ / 2.f
    });

    sprite_.setScale(sf::Vector2f{
        (tileSize * 1.3f) / frameWidth_,
        (tileSize * 1.3f) / frameHeight_
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
     drawHealthBar(window);
}

sf::FloatRect Goblin::getBounds() const {
    return sprite_.getGlobalBounds();
}

// ========================= WOLF =========================

Wolf::Wolf(const std::vector<sf::Vector2i>& path, float tileSize)
    : Enemy(130, 90.f, 20, path, tileSize, sf::Color::White, 20.f),
      sprite_(texture_)
{
    if (texture_.getSize().x == 0) {
        if (!texture_.loadFromFile("C:\\Users\\spaul\\Desktop\\wolf.png")) {
            std::cerr << "Could not load wolf.png\n";
        }
    }

    sprite_.setTextureRect(sf::IntRect(
        {0, 0},
        {frameWidth_, frameHeight_}
    ));

    sprite_.setOrigin({
        frameWidth_ / 2.f,
        frameHeight_ / 2.f
    });

    sf::Vector2f scale(
        (tileSize * 0.85f) / frameWidth_,
        (tileSize * 0.85f) / frameHeight_
    );
    sprite_.setScale(scale);

    sprite_.setPosition(getPosition());
}

void Wolf::updateDirection(float dx, float dy) {
    if (std::abs(dx) > std::abs(dy)) {
        if (dx > 0)
            currentRow_ = 2; // right
        else
            currentRow_ = 1; // left
    } else {
        if (dy > 0)
            currentRow_ = 0; // down/front
        else
            currentRow_ = 3; // up/back
    }
}

void Wolf::updateAnimation(float dt) {
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

void Wolf::update(float dt) {
    sf::Vector2f oldPos = getPosition();

    Enemy::update(dt);

    sf::Vector2f newPos = getPosition();
    float dx = newPos.x - oldPos.x;
    float dy = newPos.y - oldPos.y;

    updateDirection(dx, dy);
    updateAnimation(dt);

    sprite_.setPosition(newPos);
}

void Wolf::draw(sf::RenderWindow& window) {
    window.draw(sprite_);
    drawHealthBar(window);
}

sf::FloatRect Wolf::getBounds() const {
    return sprite_.getGlobalBounds();
}

void Enemy::drawHealthBar(sf::RenderWindow& window) {
    float barWidth = 40.f;
    float barHeight = 5.f;

    float ratio = hp_ / maxHp_;
    if (ratio < 0.f) ratio = 0.f;

    sf::FloatRect bounds = getBounds();

    float barX = bounds.position.x + (bounds.size.x - barWidth) / 2.f;
    float barY = bounds.position.y - 10.f;

    sf::RectangleShape back({barWidth, barHeight});
    back.setFillColor(sf::Color::Black);
    back.setPosition({barX, barY});

    sf::RectangleShape front({barWidth * ratio, barHeight});
    front.setFillColor(sf::Color::Green);
    front.setPosition({barX, barY});

    window.draw(back);
    window.draw(front);
}