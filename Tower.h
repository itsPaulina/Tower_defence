#pragma once
#include "GameObject.h"
#include "Enemy.h"
#include "Projectile.h"
#include <vector>
#include <memory>

class Tower : public GameObject {
public:
    Tower(sf::Vector2f pos, float range, int damage, float cooldown, sf::Color color);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;

    void setTexture(const sf::Texture* texture);

    Enemy* findTarget(const std::vector<std::unique_ptr<GameObject>>& objects) const;
    virtual void attack(std::vector<std::unique_ptr<GameObject>>& objects);

    // TUTAJ DODAJEMY ROZWIĄZANIE:
    // Wieża spełnia wymóg klasy GameObject i zawsze zwraca, że jest aktywna
    bool isActive() const override { return true; }

protected:
    float range_;
    int damage_;
    float cooldown_;
    float cooldownTimer_;

private:
    sf::RectangleShape shape_;
};

class Catapult : public Tower {
public:
    explicit Catapult(sf::Vector2f pos);
    void update(float dt) override;
};

class CannonTower : public Tower {
public:
    explicit CannonTower(sf::Vector2f pos);
    void update(float dt) override;
};

class Tank : public Tower {
public:
    explicit Tank(sf::Vector2f pos);
    void update(float dt) override;
};