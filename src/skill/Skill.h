#ifndef SKILL_H
#define SKILL_H

#include "raylib.h"
#include "../player/Player.h"
#include "../enemy/Enemy.h"
#include <vector>

const int SKILL_AUTO_BALLS = 0;
const int SKILL_LASER_BEAM = 1;
const int SKILL_THUNDER_STRIKE = 2;
const int SKILL_SHURIKEN = 3;
const int SKILL_SHIELD = 4;
const int SKILL_HAMMER = 5;

struct SkillStats {
    int damage;
    float cooldown;
    float range;
    float speed;
    int count;
    float effectRadius;
    bool special;
};

struct SkillProjectile {
    Vector2 position;
    Vector2 velocity;
    float lifeTime;
    float radius;
    float damage;
    Color color;
    int type;
    float angle;
};

class Skill {
private:
    int skillType;
    int skillLevel;
    SkillStats stats;
    Player* player;
    Texture2D texture;
    float currentCooldownTimer;
    float orbitAngle;
    std::vector<SkillProjectile> projectiles;

    void updateSkillStats();
    void loadTexture();

public:
    Skill(Player* owner, int type);
    ~Skill();

    const char* getName() const;
    int getLevel() const;
    void setLevel(int newLevel);

    void attack(const std::vector<Enemy*>& enemies);
    void update(std::vector<Enemy*>& enemies);
    void draw() const;
};

void updateSkillProjectiles(std::vector<SkillProjectile>& projectiles, std::vector<Enemy*>& enemies, float deltaTime);
void drawSkillProjectiles(const std::vector<SkillProjectile>& projectiles);

#endif
