#pragma once // Ensures this header file is included only once.

#include <SFML/Graphics.hpp> // Provides SFML types like RenderWindow, FloatRect, and Vector2f.


class GameObject {
public:
    virtual ~GameObject() = default; // Virtual destructor for safe deletion through a base-class pointer.

    virtual void update(float dt) = 0;                  // Pure virtual function for updating object logic, virtual to allow polymorphic behavior.
    virtual void draw(sf::RenderWindow& window) = 0;    // Pure virtual function for drawing the object.
    virtual sf::FloatRect getBounds() const = 0;        // Pure virtual function for returning object bounds.

    virtual bool isActive() const; // Returns whether the object is still active.
    virtual void deactivate();     // Marks the object as inactive.
    sf::Vector2f getPosition() const; // Returns the current object position.

protected:
    float x_{0.f};      // X position of the object.
    float y_{0.f};      // Y position of the object.
    bool active_{true}; // Whether the object is active in the game.
};