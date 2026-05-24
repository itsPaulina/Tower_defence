#pragma once
#include <vector>

enum class TowerType { Catapult, CannonTower, Tank };

class TowerMenu {
public:
    void selectTower(TowerType type);
    void unlockTower(TowerType type);
    TowerType getSelectedTower() const;
    int getCost(TowerType type) const;

private:
    TowerType selected_{TowerType::Catapult};
    std::vector<TowerType> unlocked_{
        TowerType::Catapult,
        TowerType::CannonTower,
        TowerType::Tank
    };
};