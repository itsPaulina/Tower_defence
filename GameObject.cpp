#include "GameObject.h" // Includes the GameObject class declaration.


bool GameObject::isActive() const {
    return active_; // Returns whether this object is still active in the game.
}


void GameObject::deactivate() {
    active_ = false; // Marks the object as inactive.
}


sf::Vector2f GameObject::getPosition() const {
    return {x_, y_}; // Returns the object's current position as an SFML vector.
}