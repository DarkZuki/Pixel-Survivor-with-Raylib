#pragma once
#include "raylib.h"
#include "../weapon/weapon.h"
#include <vector>

const int MAX_UPGRADE_OPTIONS = 3;

struct UpgradeOption {
    int weaponType = -1;
    int upgradeLevel = 1;
    bool isNewWeapon = false;
    Weapon* weaponPtr = nullptr;
};

class UpgradeSystem {
private:
    bool active = false;
    bool paused = false;
    float delay = 0.0f;
    UpgradeOption selected;
    UpgradeOption options[3];
    Rectangle boxes[3];
    Rectangle skipBox = {325, 500, 150, 50};

public:
    void showUpgradeMenu(std::vector<Weapon*>& weapons);
    void update();
    void draw();
    bool isMenuActive() const { return active; }
    bool isGamePaused() const { return paused; }
    UpgradeOption getSelectedUpgrade() const { return selected; }
};
