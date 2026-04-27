#pragma once
#include "raylib.h"
#include "../weapon/weapon.h"
#include <vector>

// Giới hạn 3 nâng cấp xuất hiện khi lên cấp
const int MAX_UPGRADE_OPTIONS = 3;

// Struct kiểm tra các trường hợp nâng cấp
struct UpgradeOption {
    int weaponType = -1;
    int upgradeLevel = 1;
    bool isNewWeapon = false;
    Weapon* weaponPtr = nullptr;
};

// Class menu nâng cấp kiểm tra điều kiện
class UpgradeSystem {
private:
    bool active = false;
    bool paused = false;
    float delay = 0.0f;
    UpgradeOption selected;
    UpgradeOption options[3];
    Rectangle boxes[3];
    Rectangle skipBox = {780, 860, 360, 90};

public:
    void showUpgradeMenu(std::vector<Weapon*>& weapons, int weaponCount = 0, int maxWeapons = 2);
    void update();
    void draw();
    bool isMenuActive() const { return active; }
    bool isGamePaused() const { return paused; }
    UpgradeOption getSelectedUpgrade() const { return selected; }
};
