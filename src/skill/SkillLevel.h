#ifndef SKILL_LEVEL_H
#define SKILL_LEVEL_H

#include <string>
#include "Skill.h"

// Du lieu bonus cua moi moc nang cap skill
struct SkillLevel {
    int level;
    std::string name;
    std::string description;
    int damageBonus;
    int rangeBonus;
    float cooldownMultiplier;
    int projectileBonus;
    int speedBonus;
    int effectRadiusBonus;
    bool special;
};

// Cac ham lay ten, du lieu level va stat tong hop cua skill
const char* getSkillLevelSkillName(int skillType);
SkillLevel getSkillLevelData(int skillType, int level);
SkillStats getSkillStats(int skillType, int skillLevel);

#endif
