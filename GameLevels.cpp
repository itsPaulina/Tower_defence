#include "Game.h"   // Includes the Game class declaration.
#include "Enemy.h"  // Includes enemy classes like Slime, Goblin, and Wolf.



void Game::setupWaves() {
    waves_ = {
        {2, 0, 0, 2}, // Wave 1: 2 slimes, 0 goblins, 0 wolves, tower limit 2.
        {3, 1, 0, 2}, // Wave 2: 3 slimes, 1 goblin, 0 wolves, tower limit 2.
        {5, 1, 1, 3}, // Wave 3: 5 slimes, 1 goblin, 1 wolf, tower limit 3.
        {6, 2, 2, 3}, // Wave 4: 6 slimes, 2 goblins, 2 wolves, tower limit 3.
        {7, 2, 5, 3}, // Wave 5: 7 slimes, 2 goblins, 5 wolves, tower limit 3.
        {5, 2, 9, 3}, // Wave 6: 5 slimes, 2 goblins, 9 wolves, tower limit 3.
        {4, 1, 15, 3} // Wave 7: 4 slimes, 1 goblin, 15 wolves, tower limit 3.
    };
}


void Game::startLevel(int level) {
    if (level > maxLevels_) { // Check if there are no more levels left.
        victory_ = true;      // Mark game as won.
        state_ = GameState::Victory; // Switch to victory state.
        levelInProgress_ = false;    // Stop level processing.
        return;
    }

    currentLevel_ = level; // Store the current level number.

    const WaveData& wave = waves_[level - 1]; // Get wave data for this level.

    int slimeBonus = 0;  // Extra slimes added by difficulty.
    int goblinBonus = 0; // Extra goblins added by difficulty.
    int wolfBonus = 0;   // Extra wolves added by difficulty.

    if (selectedDifficulty_ == Difficulty::Medium) { // Medium difficulty adjustments.
        slimeBonus = 0;
        goblinBonus = 1;
        wolfBonus = 1;
    }
    else if (selectedDifficulty_ == Difficulty::Hard) { // Hard difficulty adjustments.
        slimeBonus = 4;
        goblinBonus = 2;
        wolfBonus = 2;
    }

    slimesLeftToSpawn_ = wave.slimes + slimeBonus;   // Set total slimes to spawn.
    goblinsLeftToSpawn_ = wave.goblins + goblinBonus; // Set total goblins to spawn.
    wolvesLeftToSpawn_ = wave.wolves + wolfBonus;     // Set total wolves to spawn.

    enemiesToSpawn_ = slimesLeftToSpawn_ + goblinsLeftToSpawn_ + wolvesLeftToSpawn_; // Count all enemies still waiting to spawn.
    towerLimit_ = wave.towerLimit; // Set tower limit for this level.

    towersPlaced_ = 0;      // Reset number of placed towers.
    spawnTimer_ = 0.f;      // Reset spawn timer.
    levelInProgress_ = true; // Mark level as active.
}


void Game::spawnNextEnemy() {
    if (slimesLeftToSpawn_ > 0) { // Spawn slime first if any are left.
        objects_.push_back(std::make_unique<Slime>(path_, tileSize_)); // Add a new Slime object.
        slimesLeftToSpawn_--;  // Decrease slime count.
        enemiesToSpawn_--;     // Decrease total enemies left to spawn.
        return;
    }

    if (goblinsLeftToSpawn_ > 0) { // If no slimes are left, try spawning goblin.
        objects_.push_back(std::make_unique<Goblin>(path_, tileSize_)); // Add a new Goblin object.
        goblinsLeftToSpawn_--; // Decrease goblin count.
        enemiesToSpawn_--;     // Decrease total spawn count.
        return;
    }

    if (wolvesLeftToSpawn_ > 0) { // If no goblins are left, try spawning wolf.
        objects_.push_back(std::make_unique<Wolf>(path_, tileSize_)); // Add a new Wolf object.
        wolvesLeftToSpawn_--;  // Decrease wolf count.
        enemiesToSpawn_--;     // Decrease total spawn count.
        return;
    }
}


void Game::checkLevelFinished() {
    bool anyEnemyAlive = false; // Will track whether at least one enemy is still active.

    for (const auto& obj : objects_) { // Check all game objects.
        Enemy* enemy = dynamic_cast<Enemy*>(obj.get()); // Try to treat object as an Enemy.
        if (enemy && enemy->isActive()) { // If this object is an enemy and is still active,
            anyEnemyAlive = true;         // mark that at least one enemy is alive.
            break;                        // No need to keep checking.
        }
    }

    if (levelInProgress_ && enemiesToSpawn_ == 0 && !anyEnemyAlive) { // If level is active, no enemies remain to spawn, and none are alive,
        levelInProgress_ = false;   // end the current level.
        betweenLevels_ = true;      // Enter pause between levels.
        levelPauseTimer_ = levelPauseDuration_; // Reset the pause timer.
    }
}