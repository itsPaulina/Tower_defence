#include "Game.h"
#include "Enemy.h"


void Game::setupWaves() {
    waves_ = {
        {3, 0, 0, 2},
        {4, 1, 0, 3},
        {5, 2, 0, 4},
        {6, 2, 1, 5},
        {8, 3, 2, 6}
    };
}

void Game::startLevel(int level) {
    if (level > maxLevels_) {
        victory_ = true;
        state_ = GameState::Victory;
        levelInProgress_ = false;
        return;
    }

    currentLevel_ = level;

    const WaveData& wave = waves_[level - 1];

    int slimeBonus = 0;
    int goblinBonus = 0;
    int wolfBonus = 0;

    if (selectedDifficulty_ == Difficulty::Medium) {
        slimeBonus = 2;
        goblinBonus = 1;
        wolfBonus = 1;
    }
    else if (selectedDifficulty_ == Difficulty::Hard) {
        slimeBonus = 4;
        goblinBonus = 2;
        wolfBonus = 2;
    }

    slimesLeftToSpawn_ = wave.slimes + slimeBonus;
    goblinsLeftToSpawn_ = wave.goblins + goblinBonus;
    wolvesLeftToSpawn_ = wave.wolves + wolfBonus;

    enemiesToSpawn_ = slimesLeftToSpawn_ + goblinsLeftToSpawn_ + wolvesLeftToSpawn_;
    towerLimit_ = wave.towerLimit;

    towersPlaced_ = 0;
    spawnTimer_ = 0.f;
    levelInProgress_ = true;
}

void Game::spawnNextEnemy() {
    if (slimesLeftToSpawn_ > 0) {
        objects_.push_back(std::make_unique<Slime>(path_, tileSize_));
        slimesLeftToSpawn_--;
        enemiesToSpawn_--;
        return;
    }

    if (goblinsLeftToSpawn_ > 0) {
        objects_.push_back(std::make_unique<Goblin>(path_, tileSize_));
        goblinsLeftToSpawn_--;
        enemiesToSpawn_--;
        return;
    }

    if (wolvesLeftToSpawn_ > 0) {
        objects_.push_back(std::make_unique<Wolf>(path_, tileSize_));
        wolvesLeftToSpawn_--;
        enemiesToSpawn_--;
        return;
    }
}

void Game::checkLevelFinished() {
    bool anyEnemyAlive = false;

    for (const auto& obj : objects_) {
        Enemy* enemy = dynamic_cast<Enemy*>(obj.get());
        if (enemy && enemy->isActive()) {
            anyEnemyAlive = true;
            break;
        }
    }

    if (levelInProgress_ && enemiesToSpawn_ == 0 && !anyEnemyAlive) {
        levelInProgress_ = false;
        betweenLevels_ = true;
        levelPauseTimer_ = levelPauseDuration_;
    }
}


