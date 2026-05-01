#ifndef WEAPON_LEVEL_H
#define WEAPON_LEVEL_H

#include <string>
#include "weapon.h"

// Struct chỉ số bonus khi lên cấp và effect đặc biệt
struct WeaponLevel {
    int level;
    std::string name;
    std::string description;
    int damageBonus;
    int rangeBonus;
    float cooldownMultiplier;
    int projectileBonus;
    int speedBonus;
    int explosionRadiusBonus;
    bool doubleHit;
};

// Các hàm getter
const char* getWeaponLevelWeaponName(int weaponType);
WeaponLevel getWeaponLevelData(int weaponType, int level);
WeaponStats getWeaponStats(int weaponType, int weaponLevel);

#endif // WEAPON_LEVEL_H
