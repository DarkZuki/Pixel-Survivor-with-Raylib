#ifndef SKILL_LEVEL_H
#define SKILL_LEVEL_H

#include <string>
#include "Skill.h"

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

const char* getSkillLevelSkillName(int skillType);
SkillLevel getSkillLevelData(int skillType, int level);
SkillStats getSkillStats(int skillType, int skillLevel);

#endif
