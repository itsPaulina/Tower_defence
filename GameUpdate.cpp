#include "Game.h"
#include "Enemy.h"
#include "Tower.h"
#include "Projectile.h"
#include <algorithm>
#include <cstdlib>

void Game::update(float dt) {
    if (gameOver_ || victory_) {
        return;
    }

    if (levelInProgress_ && enemiesToSpawn_ > 0) {
        spawnTimer_ += dt;

        if (spawnTimer_ >= spawnInterval_) {
            spawnTimer_ = 0.f;
            spawnNextEnemy();
        }
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

    for (auto it = objects_.begin(); it != objects_.end();) {
        Enemy* e = dynamic_cast<Enemy*>(it->get());

        if (e && e->isActive() && e->reachedGoal()) {
            e->deactivate();
            --baseHP_;

            if (baseHP_ <= 0) {
                gameOver_ = true;
            }
        }

        if (!(*it)->isActive()) {
            if (e && !e->reachedGoal()) {
                gold_ += e->getReward();
            }

            it = objects_.erase(it);
        } else {
            ++it;
        }
    }

    checkLevelFinished();
}

