#pragma once
#include <SFML/Graphics.hpp>

class GameObject {
public:
    virtual ~GameObject() = default;

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual sf::FloatRect getBounds() const = 0;

   virtual bool isActive() const=0;
    void deactivate();
    sf::Vector2f getPosition() const;

protected:
    float x_{0.f};
    float y_{0.f};
    bool active_{true};
};