#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "GameObject.h"
#include "Animation.h"

class Enemy : public GameObject {
protected:
    int hp_;
    float speed_;
    int reward_;
    bool active_ = true;

    Animation animation_;
    std::vector<sf::Vector2i> path_;
    float tileSize_;
    size_t pathIndex_ = 0;

    float x_ = 0.f;
    float y_ = 0.f;

    sf::CircleShape shape_;

    sf::Vector2f tileCenter(const sf::Vector2i& tile) const;
    void moveAlongPath(float dt);

public:
    Enemy(int hp, float speed, int reward,
          const std::vector<sf::Vector2i>& path,
          float tileSize, sf::Color color, float radius);

    virtual ~Enemy() = default;

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;

    void takeDamage(int dmg);
    sf::Vector2f getPosition() const;

    bool isActive() const override { return active_; }
    void deactivate() { active_ = false; }

    bool reachedGoal() const;
    int getReward() const;
    int getHp() const;
};

class Slime : public Enemy {
private:
    static sf::Texture texture_; // Statyczna tekstura dzielona przez wszystkie instancje
    sf::Sprite sprite_;

    int frameWidth_ = 53;
    int frameHeight_ = 52;
    int frameCount_ = 11;

    int currentFrame_ = 0;
    int currentRow_ = 0;

    float animationTimer_ = 0.f;
    float animationSpeed_ = 0.08f;

    void updateDirection(float dx, float dy);
    void updateAnimation(float dt);

public:
    Slime(const std::vector<sf::Vector2i>& path, float tileSize);
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
};

class Goblin : public Enemy {
private:
    static sf::Texture texture_; // Statyczna tekstura dzielona przez wszystkie instancje
    sf::Sprite sprite_;

    int frameWidth_ = 103;
    int frameHeight_ = 86;
    int frameCount_ = 6;

    int currentFrame_ = 0;
    int currentRow_ = 0;

    float animationTimer_ = 0.f;
    float animationSpeed_ = 0.12f;

    void updateDirection(float dx, float dy);
    void updateAnimation(float dt);

public:
    Goblin(const std::vector<sf::Vector2i>& path, float tileSize);
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
};

class Wolf : public Enemy {
private:
    static sf::Texture texture_; // Statyczna tekstura dzielona przez wszystkie instancje
    sf::Sprite sprite_;

    int frameWidth_ = 48; 
    int frameHeight_ = 48; 
    int frameCount_ = 4;

    int currentFrame_ = 0;
    int currentRow_ = 0;

    float animationTimer_ = 0.f;
    float animationSpeed_ = 0.10f;

    void updateDirection(float dx, float dy);
    void updateAnimation(float dt);

public:
    Wolf(const std::vector<sf::Vector2i>& path, float tileSize);
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
};