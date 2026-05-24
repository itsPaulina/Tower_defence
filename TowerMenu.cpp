#include "TowerMenu.h"

void TowerMenu::selectTower(TowerType type) { selected_ = type; }
TowerType TowerMenu::getSelectedTower() const { return selected_; }
void TowerMenu::unlockTower(TowerType type) { unlocked_.push_back(type); }

int TowerMenu::getCost(TowerType type) const {
    switch (type) {
        case TowerType::Catapult: return 50;
        case TowerType::CannonTower: return 70;
        case TowerType::Tank: return 100;
    }
    return 0;
}