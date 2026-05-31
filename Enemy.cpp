#include "Enemy.h"   // Includes declarations of Enemy, Slime, Goblin, and Wolf.
#include <cmath>     // Needed for std::sqrt and std::abs.
#include <iostream>  // Needed for std::cerr error messages.



sf::Texture Slime::texture_;   // Shared texture for all Slime objects.
sf::Texture Goblin::texture_;  // Shared texture for all Goblin objects.
sf::Texture Wolf::texture_;    // Shared texture for all Wolf objects.


sf::Vector2f Enemy::tileCenter(const sf::Vector2i& tile) const {
    return {
        tile.x * tileSize_ + tileSize_ / 2.f, // X coordinate of the tile center.
        tile.y * tileSize_ + tileSize_ / 2.f  // Y coordinate of the tile center.
    };
}


Enemy::Enemy(int hp, float speed, int reward,
             const std::vector<sf::Vector2i>& path,
             float tileSize, sf::Color color, float radius)
    : hp_(hp), maxHp_(hp), speed_(speed), reward_(reward), // Initialize basic enemy stats.
      animation_(4, 6.f), path_(path), tileSize_(tileSize), // Initialize animation, path, and tile size.
      pathIndex_(0), shape_(radius) { // Start at the first path point and create the body shape.
    shape_.setOrigin({radius, radius}); // Set origin to the center of the circle.
    shape_.setFillColor(color);         // Set the enemy color.

    auto start = tileCenter(path_.front()); // Get the center of the first tile in the path.
    x_ = start.x;                           // Set initial X position.
    y_ = start.y;                           // Set initial Y position.
    shape_.setPosition({x_, y_});           // Place the shape at the starting position.
}


void Enemy::moveAlongPath(float dt) {
    if (pathIndex_ >= path_.size() - 1) { // Check if the enemy reached the last tile.
        active_ = false;                  // Mark enemy as inactive.
        return;                           // Stop moving.
    }

    sf::Vector2f current{x_, y_};                 // Current enemy position.
    sf::Vector2f target = tileCenter(path_[pathIndex_ + 1]); // Next target tile center, which is the next point in the path.
    sf::Vector2f dir = target - current;          // Direction vector from current position to target.
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y); // Length of the direction vector.

    if (len < 2.f) {          // If the enemy is very close to the target tile,
        ++pathIndex_;         // move to the next path segment.
        x_ = target.x;        // Snap exactly to target X.
        y_ = target.y;        // Snap exactly to target Y.
    } else {
        dir /= len;                   // Normalize direction vector.
        x_ += dir.x * speed_ * dt;    // Move in X direction using speed and delta time.
        y_ += dir.y * speed_ * dt;    // Move in Y direction using speed and delta time.
    }

    shape_.setPosition({x_, y_}); // Update shape position after movement.
}


void Enemy::takeDamage(int dmg) {
    hp_ -= dmg;            // Subtract damage from current health.
    if (hp_ <= 0)
        active_ = false;   // Deactivate enemy if health drops to zero or below.
}


void Enemy::update(float dt) {
    animation_.update(dt); // Update basic animation timer.
    moveAlongPath(dt);     // Move enemy along the path.
}




void Enemy::draw(sf::RenderWindow& window) {
    window.draw(shape_); // Draw the basic enemy shape.
}


sf::FloatRect Enemy::getBounds() const {
    return shape_.getGlobalBounds(); // Return global bounds of the enemy shape.
}


bool Enemy::reachedGoal() const {
    return pathIndex_ >= path_.size() - 1; // Check if the enemy has reached the last tile.
}


int Enemy::getReward() const {
    return reward_; // Return gold/reward value for defeating the enemy.
}


int Enemy::getHp() const {
    return hp_; // Return current health points.
}


// ========================= SLIME =========================


Slime::Slime(const std::vector<sf::Vector2i>& path, float tileSize)
    : Enemy(50, 50.f, 10, path, tileSize, sf::Color::White, 16.f), // Create a Slime with its stats.
      sprite_(texture_) 
{
    if (texture_.getSize().x == 0) { // Load texture only once if it has not been loaded yet.
        if (!texture_.loadFromFile("photos/slime.png")) {
            std::cerr << "Could not load slime.png\n"; // Print error if loading fails.
        }
    }

    sprite_.setTextureRect(sf::IntRect(
        {0, 0},                         // Start from the first frame in the spritesheet.
        {frameWidth_, frameHeight_}     // Size of a single animation frame.
    ));

    sprite_.setOrigin({
        frameWidth_ / 2.f,  // Set origin to the center of the frame in X.
        frameHeight_ / 2.f  // Set origin to the center of the frame in Y.
    });

    sf::Vector2f slimeScale(
        (tileSize * 0.7f) / frameWidth_,   // Scale width to fit tile size.
        (tileSize * 0.7f) / frameHeight_   // Scale height to fit tile size.
    );
    sprite_.setScale(slimeScale); // Apply scale to the sprite.

    sprite_.setPosition(getPosition()); // Set sprite position to match enemy position.
}


void Slime::updateDirection(float dx, float dy) {
    if (std::abs(dx) > std::abs(dy)) { // Check whether horizontal movement is stronger than vertical.
        if (dx > 0)
            currentRow_ = 1; // Right animation row.
        else
            currentRow_ = 3; // Left animation row.
    } else {
        if (dy > 0)
            currentRow_ = 0; // Down/front animation row.
        else
            currentRow_ = 2; // Up/back animation row.
    }
}


void Slime::updateAnimation(float dt) {
    animationTimer_ += dt; // Add elapsed time to animation timer.

    if (animationTimer_ >= animationSpeed_) { // Change frame when enough time has passed.
        animationTimer_ = 0.f;                // Reset animation timer.
        currentFrame_ = (currentFrame_ + 1) % frameCount_; // Go to the next frame and loop.

        sprite_.setTextureRect(sf::IntRect(
            {currentFrame_ * frameWidth_, currentRow_ * frameHeight_}, // Frame position in spritesheet.
            {frameWidth_, frameHeight_} // Size of one frame.
        ));
    }
}


void Slime::update(float dt) {
    sf::Vector2f oldPos = getPosition(); // Save old position before moving.

    Enemy::update(dt); // Run base enemy update.

    sf::Vector2f newPos = getPosition(); // Get new position after movement.
    float dx = newPos.x - oldPos.x;      // Calculate movement in X.
    float dy = newPos.y - oldPos.y;      // Calculate movement in Y.

    updateDirection(dx, dy); // Update sprite row depending on movement direction.
    updateAnimation(dt);     // Update sprite animation frame.

    sprite_.setPosition(newPos); // Move sprite to the new enemy position.
}


void Slime::draw(sf::RenderWindow& window) {
    window.draw(sprite_);   // Draw the slime sprite.
    drawHealthBar(window);  // Draw the health bar above it.
}


sf::FloatRect Slime::getBounds() const {
    return sprite_.getGlobalBounds(); // Return sprite bounds instead of circle bounds.
}


// ========================= GOBLIN =========================


Goblin::Goblin(const std::vector<sf::Vector2i>& path, float tileSize)
    : Enemy(120, 70.f, 15, path, tileSize, sf::Color(120, 200, 40), 18.f), // Create a Goblin with its stats.
      sprite_(texture_)
{
    if (texture_.getSize().x == 0) { // Load texture only once.
        if (!texture_.loadFromFile("photos/gob.png")) {
            std::cerr << "Could not load gob.png\n"; // Print error if loading fails.
        }
    }

    sprite_.setTextureRect(sf::IntRect(
        {0, 0},                         // First animation frame.
        {frameWidth_, frameHeight_}     // Frame size.
    ));

    sprite_.setOrigin({
        frameWidth_ / 2.f,  // Center origin in X.
        frameHeight_ / 2.f  // Center origin in Y.
    });

    sprite_.setScale(sf::Vector2f{
        (tileSize * 1.3f) / frameWidth_,   // Scale width.
        (tileSize * 1.3f) / frameHeight_   // Scale height.
    });

    sprite_.setPosition(getPosition()); // Match sprite position with enemy position.
}


void Goblin::updateDirection(float dx, float dy) {
    if (std::abs(dx) > std::abs(dy)) { // Check main movement direction.
        if (dx > 0)
            currentRow_ = 3; // Right row.
        else
            currentRow_ = 2; // Left row.
    } else {
        if (dy > 0)
            currentRow_ = 0; // Down row.
        else
            currentRow_ = 1; // Up row.
    }
}


void Goblin::updateAnimation(float dt) {
    animationTimer_ += dt; // Add delta time.

    if (animationTimer_ >= animationSpeed_) { // Advance frame when timer is large enough.
        animationTimer_ = 0.f;                // Reset timer.
        currentFrame_ = (currentFrame_ + 1) % frameCount_; // Go to next frame.

        sprite_.setTextureRect(sf::IntRect(
            {currentFrame_ * frameWidth_, currentRow_ * frameHeight_}, // Current frame in spritesheet.
            {frameWidth_, frameHeight_} // Frame size.
        ));
    }
}


void Goblin::update(float dt) {
    sf::Vector2f oldPos = getPosition(); // Save old position.

    Enemy::update(dt); // Update base movement and animation.

    sf::Vector2f newPos = getPosition(); // Read new position.
    float dx = newPos.x - oldPos.x;      // Change in X.
    float dy = newPos.y - oldPos.y;      // Change in Y.

    updateDirection(dx, dy); // Update facing direction.
    updateAnimation(dt);     // Update frame animation.

    sprite_.setPosition(newPos); // Place sprite at new position.
}


void Goblin::draw(sf::RenderWindow& window) {
    window.draw(sprite_);   // Draw goblin sprite.
    drawHealthBar(window);  // Draw health bar.
}


sf::FloatRect Goblin::getBounds() const {
    return sprite_.getGlobalBounds(); // Return goblin sprite bounds.
}


// ========================= WOLF =========================


Wolf::Wolf(const std::vector<sf::Vector2i>& path, float tileSize)
    : Enemy(180, 90.f, 20, path, tileSize, sf::Color::White, 20.f), // Create a Wolf with its stats.
      sprite_(texture_)
{
    if (texture_.getSize().x == 0) { // Load texture only once.
        if (!texture_.loadFromFile("photos/wolf.png")) {
            std::cerr << "Could not load wolf.png\n"; // Print error if loading fails.
        }
    }

    sprite_.setTextureRect(sf::IntRect(
        {0, 0},                         // First frame in spritesheet.
        {frameWidth_, frameHeight_}     // Frame size.
    ));

    sprite_.setOrigin({
        frameWidth_ / 2.f,  // Center origin in X.
        frameHeight_ / 2.f  // Center origin in Y.
    });

    sf::Vector2f scale(
        (tileSize * 0.85f) / frameWidth_,   // Scale width.
        (tileSize * 0.85f) / frameHeight_   // Scale height.
    );
    sprite_.setScale(scale); // Apply scaling.

    sprite_.setPosition(getPosition()); // Match sprite position to enemy position.
}


void Wolf::updateDirection(float dx, float dy) {
    if (std::abs(dx) > std::abs(dy)) { // Check whether movement is mostly horizontal.
        if (dx > 0)
            currentRow_ = 2; // Right row.
        else
            currentRow_ = 1; // Left row.
    } else {
        if (dy > 0)
            currentRow_ = 0; // Down/front row.
        else
            currentRow_ = 3; // Up/back row.
    }
}


void Wolf::updateAnimation(float dt) {
    animationTimer_ += dt; // Increase animation timer.

    if (animationTimer_ >= animationSpeed_) { // If enough time passed, switch frame.
        animationTimer_ = 0.f;                // Reset timer.
        currentFrame_ = (currentFrame_ + 1) % frameCount_; // Advance frame with looping.

        sprite_.setTextureRect(sf::IntRect(
            {currentFrame_ * frameWidth_, currentRow_ * frameHeight_}, // Current frame position.
            {frameWidth_, frameHeight_} // Frame dimensions.
        ));
    }
}


void Wolf::update(float dt) {
    sf::Vector2f oldPos = getPosition(); // Save old position before update.

    Enemy::update(dt); // Update base enemy logic.

    sf::Vector2f newPos = getPosition(); // Read updated position.
    float dx = newPos.x - oldPos.x;      // Movement in X.
    float dy = newPos.y - oldPos.y;      // Movement in Y.

    updateDirection(dx, dy); // Set correct animation row.
    updateAnimation(dt);     // Advance animation frames.

    sprite_.setPosition(newPos); // Update sprite position.
}


void Wolf::draw(sf::RenderWindow& window) {
    window.draw(sprite_);   // Draw wolf sprite.
    drawHealthBar(window);  // Draw health bar above it.
}


sf::FloatRect Wolf::getBounds() const {
    return sprite_.getGlobalBounds(); // Return wolf sprite bounds.
}


void Enemy::drawHealthBar(sf::RenderWindow& window) {
    float barWidth = 40.f;  // Width of the health bar.
    float barHeight = 5.f;  // Height of the health bar.

    float ratio = static_cast<float>(hp_) / static_cast<float>(maxHp_); // Current HP as a percentage.
    if (ratio < 0.f) ratio = 0.f; // Prevent negative width.

    sf::FloatRect bounds = getBounds(); // Get current enemy bounds.

    float barX = bounds.position.x + (bounds.size.x - barWidth) / 2.f; // Center health bar horizontally.
    float barY = bounds.position.y - 10.f; // Place health bar above the enemy.

    sf::RectangleShape back({barWidth, barHeight}); // Background bar.
    back.setFillColor(sf::Color::Black);            // Set background color.
    back.setPosition({barX, barY});                 // Place background bar.

    sf::RectangleShape front({barWidth * ratio, barHeight}); // Foreground bar based on health ratio.
    front.setFillColor(sf::Color::Green);                   // Set health color.
    front.setPosition({barX, barY});                       // Place foreground bar.

    window.draw(back);  // Draw background bar.
    window.draw(front); // Draw current health bar.
}