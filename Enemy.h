#pragma once
#include "GameObject.h"
#include "Animation.h"
#include <vector>

class Enemy : public GameObject {
public:
    Enemy(int hp, float speed, int reward,
          const std::vector<sf::Vector2i>& path,
          float tileSize, sf::Color color, float radius);

    void moveAlongPath(float dt);
    void takeDamage(int dmg);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    sf::Vector2f getPosition() const;
    bool reachedGoal() const;
    int getReward() const;
    int getHp() const;

protected:
    int hp_;
    float speed_;
    int reward_;
    Animation animation_;

private:

        
    std::vector<sf::Vector2i> path_;
    float tileSize_;
    std::size_t pathIndex_;
    sf::CircleShape shape_;

    sf::Vector2f tileCenter(const sf::Vector2i& tile) const;
};

class Slime : public Enemy {
public:
    explicit Slime(const std::vector<sf::Vector2i>& path, float tileSize);
    void update(float dt) override;
};
class Goblin : public Enemy {
public:
    Goblin(const std::vector<sf::Vector2i>& path, float tileSize);
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    sf::Texture texture_;
    sf::Sprite sprite_{texture_};

    int frameWidth_ = 102;
    int frameHeight_ = 81;
    int currentFrame_ = 0;
    int currentRow_ = 0;
    int frameCount_ = 6;

    float animationTimer_ = 0.f;
    float animationSpeed_ = 0.12f;

    void updateAnimation(float dt);
    void updateDirection(float dx, float dy);
};

class Wolf : public Enemy {
public:
    explicit Wolf(const std::vector<sf::Vector2i>& path, float tileSize);
    void update(float dt) override;
};