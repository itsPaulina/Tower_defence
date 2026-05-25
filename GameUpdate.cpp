#include "Game.h"
#include "Enemy.h"
#include "Tower.h"
#include "Projectile.h"
#include <algorithm>
#include <cstdlib>

void Game::update(float dt) {
    float spawnInterval = 1.5f;

    if (toSpawn_ > 0 && spawnClock_.getElapsedTime().asSeconds() >= spawnInterval) {
        spawnEnemy();
        spawnClock_.restart();
        --toSpawn_;
    }

    for (auto& obj : objects_) {
        if (obj->isActive()) {
            obj->update(dt);
        }
    }

    for (auto& obj : objects_) {
        Tower* tower = dynamic_cast<Tower*>(obj.get());
        if (tower && tower->isActive()) {
            tower->attack(objects_);
        }
    }

    for (auto& obj : objects_) {
        Enemy* e = dynamic_cast<Enemy*>(obj.get());
        if (e && e->isActive() && e->reachedGoal()) {
            e->deactivate();
            --baseHP_;
        }
    }

    handleCollisions();

    for (auto& obj : objects_) {
        Enemy* e = dynamic_cast<Enemy*>(obj.get());
        if (e && !e->isActive()) {
            money_ += e->getReward();
        }
    }

    objects_.erase(
        std::remove_if(objects_.begin(), objects_.end(),
            [](const std::unique_ptr<GameObject>& o) { return !o->isActive(); }),
        objects_.end());
}

void Game::spawnEnemy() {
    int type = std::rand() % 3;

    if (type == 0) {
        objects_.push_back(std::make_unique<Slime>(path_, tileSize_));
    }
    else if (type == 1) {
        objects_.push_back(std::make_unique<Goblin>(path_, tileSize_));
    }
    else {
        objects_.push_back(std::make_unique<Wolf>(path_, tileSize_));
    }
}

void Game::handleCollisions() {
    for (auto& projObj : objects_) {
        Projectile* p = dynamic_cast<Projectile*>(projObj.get());
        if (!p || !p->isActive()) {
            continue;
        }

        for (auto& enemyObj : objects_) {
            Enemy* e = dynamic_cast<Enemy*>(enemyObj.get());
            if (!e || !e->isActive()) {
                continue;
            }

            if (p->getBounds().findIntersection(e->getBounds())) {
                e->takeDamage(p->getDamage());
                p->hitTarget();
                break;
            }
        }
    }
}

void Game::nextWave() {
    ++wave_;
    int base = 5;
    toSpawn_ += base + wave_ * 2;
}