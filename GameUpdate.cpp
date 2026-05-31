#include "Game.h"        // Includes the Game class declaration.
#include "Enemy.h"       // Includes enemy classes and Enemy type.
#include "Tower.h"       // Includes tower classes and Tower type.
#include "Projectile.h"  // Includes projectile classes and Projectile type.
#include <algorithm>     // Standard algorithms library.
#include <cstdlib>       // General utilities library.


void Game::update(float dt) {
    if (gameOver_ || victory_) { // Stop updating if the game already ended.
        return;
    }

    if (betweenLevels_) { // Handle pause between levels.
        levelPauseTimer_ -= dt; // Reduce pause timer by elapsed time.

        if (levelPauseTimer_ <= 0.f) { // When pause is over, start the next level.
            betweenLevels_ = false;    // leave pause state
            startLevel(currentLevel_ + 1); // and start the next level.
        }

        return; // Skip rest of update while between levels.
    }

    if (levelInProgress_ && enemiesToSpawn_ > 0) { // Spawn enemies only if level is active and some remain to spawn.
        spawnTimer_ += dt; // Add elapsed time to spawn timer.

        if (spawnTimer_ >= spawnInterval_) { // Spawn a new enemy when enough time passed.
            spawnTimer_ = 0.f; // Reset spawn timer.
            spawnNextEnemy();  // Create the next enemy.
        }
    }

    std::size_t objectCount = objects_.size(); // Save object count before updating, it's possible that new objects are added during update.

    for (std::size_t i = 0; i < objectCount; ++i) {
        if (objects_[i]->isActive()) { // Update only active objects.
            objects_[i]->update(dt); // Update object logic, which may include movement, animation, or other behavior. This calls the appropriate update function for each object type due to polymorphism.
        }
    }

    objectCount = objects_.size(); // Read object count again in case update added new objects.

    for (std::size_t i = 0; i < objectCount; ++i) {
        Tower* tower = dynamic_cast<Tower*>(objects_[i].get()); // Try to treat object as a tower.
        if (tower && tower->isActive()) { // If object is a tower and still active,
            tower->attack(objects_);      // let it attack enemies.
        }
    }

    for (auto& obj : objects_) { // Check all objects for projectiles.
        Projectile* projectile = dynamic_cast<Projectile*>(obj.get()); // Try to cast object to projectile.
        if (!projectile || !projectile->isActive()) // Skip if it is not an active projectile.
            continue;

        for (auto& other : objects_) { // Compare projectile with all other objects.
            Enemy* enemy = dynamic_cast<Enemy*>(other.get()); // Try to cast other object to enemy.
            if (!enemy || !enemy->isActive()) // Skip if it is not an active enemy.
                continue;

            if (projectile->getBounds().findIntersection(enemy->getBounds())) { // Check collision between projectile and enemy.
                enemy->takeDamage(projectile->getDamage()); // Deal projectile damage to enemy.
                projectile->deactivate(); // Remove projectile after hit.
                break; // Stop checking this projectile after first collision.
            }
        }
    }

    for (auto it = objects_.begin(); it != objects_.end();) { // Loop through objects for cleanup.
        Enemy* e = dynamic_cast<Enemy*>(it->get()); // Try to cast current object to enemy.

        if (e && e->isActive() && e->reachedGoal()) { // If an active enemy reached the castle,
            e->deactivate(); // deactivate the enemy
            --castleHP_;     // and reduce castle health.

            if (castleHP_ <= 0) { // If castle health reaches zero,
                castleHP_ = 0;    // clamp it to zero
                gameOver_ = true; // mark game as lost
                state_ = GameState::GameOver; // and switch to game over state.
            }
        }

        if (!(*it)->isActive()) { // Remove objects that are no longer active.
            if (e && !e->reachedGoal()) { // If removed object is a defeated enemy,
                gold_ += e->getReward();  // give player the reward.
            }

            it = objects_.erase(it); // Erase object and move iterator to next valid element.
        } else {
            ++it; // Move to next object if current one stays.
        }
    }

    checkLevelFinished(); // Check whether the current level is complete.
}