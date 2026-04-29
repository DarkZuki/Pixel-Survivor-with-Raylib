#ifndef SKILL_H
#define SKILL_H

#include "raylib.h"
#include "../player/Player.h"
#include "../enemy/Enemy.h"
#include <vector>

const int SKILL_LASER_BEAM = 0;
const int SKILL_THUNDER_STRIKE = 1;
const int SKILL_SHURIKEN = 2;
const int SKILL_SHIELD = 3;
const int SKILL_HAMMER = 4;

// Bo chi so tong hop cua skill sau khi cong theo level
struct SkillStats {
    int damage;
    float cooldown;
    float range;
    float speed;
    int count;
    float effectRadius;
    bool special;
};

// Projectile rieng cua he thong skill
struct SkillProjectile {
    Vector2 position;
    Vector2 velocity;
    float lifeTime;
    float radius;
    float damage;
    Color color;
    int type;
    float angle;
    int remainingBounces;
};

class Skill {
private:
    // Loai skill, level va chi so dang duoc ap dung
    int skillType;
    int skillLevel;
    SkillStats stats;
    Player* player;
    Texture2D texture;
    // Timer hoi chieu va projectile dang ton tai cua skill
    float currentCooldownTimer;
    float orbitAngle;
    std::vector<SkillProjectile> projectiles;

    // Tinh lai stat va tai texture dung theo loai skill
    void updateSkillStats();
    void loadTexture();

public:
    Skill(Player* owner, int type);
    ~Skill();

    const char* getName() const;
    int getLevel() const;
    void setLevel(int newLevel);

    // Vong doi chinh cua skill: tan cong, update va draw
    void attack(const std::vector<Enemy*>& enemies);
    void update(std::vector<Enemy*>& enemies);
    void draw() const;
};

// Ham dung chung cho projectile cua skill
void updateSkillProjectiles(std::vector<SkillProjectile>& projectiles, std::vector<Enemy*>& enemies, float deltaTime);
void drawSkillProjectiles(const std::vector<SkillProjectile>& projectiles);

#endif
