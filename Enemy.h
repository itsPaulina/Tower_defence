#pragma once // Ensures this header file is included only once.

#include <SFML/Graphics.hpp> // Provides SFML graphics classes like Sprite, Texture, Color, and RenderWindow.
#include <vector>            // Provides std::vector.
#include "GameObject.h"      // Base class for all game objects.
#include "Animation.h"       // Animation class used by enemies.


class Enemy : public GameObject { // Base class for all enemy types.
protected:
    int hp_;          // Current health points.
    float maxHp_;     // Maximum health points.
    float speed_;     // Movement speed of the enemy.
    int reward_;      // Reward given when the enemy is defeated.

    Animation animation_;              // Basic animation controller.
    std::vector<sf::Vector2i> path_;   // Path of tiles that the enemy follows.
    float tileSize_;                   // Size of one map tile.
    size_t pathIndex_ = 0;             // Current index in the path.

    sf::CircleShape shape_;            // Basic shape used for enemy position and fallback drawing.

    sf::Vector2f tileCenter(const sf::Vector2i& tile) const; // Returns the center of a given tile.
    void moveAlongPath(float dt);                          // Moves the enemy along the path.
    void drawHealthBar(sf::RenderWindow& window);          // Draws the health bar above the enemy.

public:
    Enemy(int hp, float speed, int reward,
          const std::vector<sf::Vector2i>& path,
          float tileSize, sf::Color color, float radius); // Constructor for initializing enemy data.

    virtual ~Enemy() = default; // Virtual destructor for safe inheritance.

    void update(float dt) override;                 // Updates enemy logic.
    void draw(sf::RenderWindow& window) override;   // Draws the enemy.
    sf::FloatRect getBounds() const override;       // Returns enemy bounds.

    void takeDamage(int dmg); // Reduces enemy health.

    bool reachedGoal() const; // Checks if the enemy reached the end of the path.
    int getReward() const;    // Returns reward value.
    int getHp() const;        // Returns current health.
};


class Slime : public Enemy { // Slime enemy class.
private:
    static sf::Texture texture_; // Shared texture for all Slime objects.
    sf::Sprite sprite_;          // Sprite used to display the slime.

    int frameWidth_ = 53;   // Width of one animation frame.
    int frameHeight_ = 52;  // Height of one animation frame.
    int frameCount_ = 11;   // Number of frames in the animation.

    int currentFrame_ = 0;  // Current animation frame.
    int currentRow_ = 0;    // Current spritesheet row based on direction.

    float animationTimer_ = 0.f;   // Time passed since last frame change.
    float animationSpeed_ = 0.08f; // Time between frame changes.

    void updateDirection(float dx, float dy); // Chooses animation row based on movement direction.
    void updateAnimation(float dt);           // Updates slime sprite animation.

public:
    Slime(const std::vector<sf::Vector2i>& path, float tileSize); // Creates a Slime enemy.
    void update(float dt) override;                               // Updates slime logic and animation.
    void draw(sf::RenderWindow& window) override;                 // Draws slime sprite.
    sf::FloatRect getBounds() const override;                     // Returns slime bounds.
};


class Goblin : public Enemy { // Goblin enemy class.
private:
    static sf::Texture texture_; // Shared texture for all Goblin objects.
    sf::Sprite sprite_;          // Sprite used to display the goblin.

    int frameWidth_ = 103;  // Width of one animation frame.
    int frameHeight_ = 86;  // Height of one animation frame.
    int frameCount_ = 6;    // Number of frames in the animation.

    int currentFrame_ = 0;  // Current animation frame.
    int currentRow_ = 0;    // Current spritesheet row based on direction.

    float animationTimer_ = 0.f;   // Time passed since last frame update.
    float animationSpeed_ = 0.12f; // Speed of the animation.

    void updateDirection(float dx, float dy); // Updates facing direction.
    void updateAnimation(float dt);           // Updates goblin animation.

public:
    Goblin(const std::vector<sf::Vector2i>& path, float tileSize); // Creates a Goblin enemy.
    void update(float dt) override;                                // Updates goblin state.
    void draw(sf::RenderWindow& window) override;                  // Draws goblin sprite.
    sf::FloatRect getBounds() const override;                      // Returns goblin bounds.
};


class Wolf : public Enemy { // Wolf enemy class.
private:
    static sf::Texture texture_; // Shared texture for all Wolf objects.
    sf::Sprite sprite_;          // Sprite used to display the wolf.

    int frameWidth_ = 48;   // Width of one animation frame.
    int frameHeight_ = 48;  // Height of one animation frame.
    int frameCount_ = 4;    // Number of animation frames.

    int currentFrame_ = 0;  // Current frame index.
    int currentRow_ = 0;    // Current direction row in the spritesheet.

    float animationTimer_ = 0.f;   // Timer for frame switching.
    float animationSpeed_ = 0.10f; // Delay between animation frames.

    void updateDirection(float dx, float dy); // Sets direction row from movement.
    void updateAnimation(float dt);           // Updates wolf animation.

public:
    Wolf(const std::vector<sf::Vector2i>& path, float tileSize); // Creates a Wolf enemy.
    void update(float dt) override;                              // Updates wolf logic.
    void draw(sf::RenderWindow& window) override;                // Draws wolf sprite.
    sf::FloatRect getBounds() const override;                    // Returns wolf bounds.
};