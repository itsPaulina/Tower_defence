#include "GameObject.h"

bool GameObject::isActive() const {
    return active_;
}

void GameObject::deactivate() {
    active_ = false;
}

sf::Vector2f GameObject::getPosition() const {
    return {x_, y_};
}