#include "Skill.h"
#include "SkillLevel.h"
#include "raymath.h"
#include <cmath>

namespace {
const int SKILL_PROJECTILE_LASER = 1;
const int SKILL_PROJECTILE_THUNDER = 2;
const int SKILL_PROJECTILE_SHIELD = 3;
const int SKILL_PROJECTILE_HAMMER = 4;

Enemy* findNearestEnemy(Vector2 center, const std::vector<Enemy*>& enemies, float maxRange = -1.0f) {
    Enemy* nearest = nullptr;
    float bestDistance = maxRange >= 0.0f ? maxRange : 999999.0f;

    for (Enemy* enemy : enemies) {
        if (!enemy) continue;
        float distance = Vector2Distance(center, {enemy->getX(), enemy->getY()});
        if (distance <= bestDistance) {
            bestDistance = distance;
            nearest = enemy;
        }
    }

    return nearest;
}

void drawOrbitSkill(Vector2 center, float orbitAngle, const SkillStats& stats, Texture2D texture, Color fallbackColor) {
    for (int i = 0; i < stats.count; i++) {
        float angle = orbitAngle + (2.0f * PI * i) / stats.count;
        Vector2 pos = {center.x + cosf(angle) * stats.range, center.y + sinf(angle) * stats.range};

        if (texture.id > 0) {
            DrawTexturePro(
                texture,
                {0.0f, 0.0f, (float)texture.width, (float)texture.height},
                {pos.x, pos.y, stats.effectRadius * 2.0f, stats.effectRadius * 2.0f},
                {stats.effectRadius, stats.effectRadius},
                angle * RAD2DEG,
                WHITE
            );
        } else {
            DrawCircleV(pos, stats.effectRadius, fallbackColor);
        }
    }
}
}

Skill::Skill(Player* owner, int type) {
    player = owner;
    skillType = type;
    skillLevel = 1;
    currentCooldownTimer = 0.0f;
    orbitAngle = 0.0f;
    laserDirection = {1.0f, 0.0f};
    updateSkillStats();
}

const char* Skill::getName() const {
    return getSkillLevelSkillName(skillType);
}

int Skill::getLevel() const {
    return skillLevel;
}

void Skill::setLevel(int newLevel) {
    skillLevel = newLevel < 0 ? 0 : (newLevel > 10 ? 10 : newLevel);
    if (skillLevel <= 0) projectiles.clear();
    updateSkillStats();
}

void Skill::updateSkillStats() {
    stats = getSkillStats(skillType, skillLevel);
}

void Skill::update(std::vector<Enemy*>& enemies) {
    if (skillLevel <= 0 || player == nullptr) return;

    float dt = GetFrameTime();
    currentCooldownTimer += dt;
    orbitAngle += dt * 2.5f;

    if (skillType == SKILL_AUTO_BALLS || skillType == SKILL_SHURIKEN) {
        attack(enemies);
    } else if (currentCooldownTimer >= stats.cooldown) {
        currentCooldownTimer = 0.0f;
        attack(enemies);
    }

    updateSkillProjectiles(projectiles, enemies, dt);
}

void Skill::attack(const std::vector<Enemy*>& enemies) {
    if (skillLevel <= 0 || player == nullptr) return;

    Vector2 playerPos = {player->getX(), player->getY()};
    int totalDamage = stats.damage + player->getDamage();

    switch (skillType) {
        case SKILL_AUTO_BALLS:
        case SKILL_SHURIKEN:
            for (int i = 0; i < stats.count; i++) {
                float angle = orbitAngle + (2.0f * PI * i) / stats.count;
                Vector2 pos = {playerPos.x + cosf(angle) * stats.range, playerPos.y + sinf(angle) * stats.range};
                for (Enemy* enemy : enemies) {
                    if (!enemy) continue;
                    if (CheckCollisionCircles(pos, stats.effectRadius, {enemy->getX(), enemy->getY()}, 20.0f)) {
                        enemy->takeDamage(totalDamage);
                    }
                }
            }
            break;

        case SKILL_LASER_BEAM: {
            Enemy* target = findNearestEnemy(playerPos, enemies);
            if (!target) return;

            Vector2 targetDirection = Vector2Subtract({target->getX(), target->getY()}, playerPos);
            laserDirection = (targetDirection.x == 0.0f && targetDirection.y == 0.0f)
                ? Vector2{1.0f, 0.0f}
                : Vector2Normalize(targetDirection);
            Vector2 endPos = Vector2Add(playerPos, Vector2Scale(laserDirection, stats.range));
            for (Enemy* enemy : enemies) {
                if (!enemy) continue;
                if (CheckCollisionCircleLine({enemy->getX(), enemy->getY()}, 20.0f, playerPos, endPos)) {
                    enemy->takeDamage(totalDamage);
                }
                if (stats.special) {
                    Vector2 reverseEndPos = Vector2Add(playerPos, Vector2Scale(laserDirection, -stats.range));
                    if (CheckCollisionCircleLine({enemy->getX(), enemy->getY()}, 20.0f, playerPos, reverseEndPos)) {
                        enemy->takeDamage(totalDamage);
                    }
                }
            }

            projectiles.push_back({playerPos, Vector2Scale(laserDirection, stats.range), 0.2f, stats.effectRadius, 0.0f, SKYBLUE, SKILL_PROJECTILE_LASER, 0.0f});
            if (stats.special) {
                projectiles.push_back({playerPos, Vector2Scale(laserDirection, -stats.range), 0.2f, stats.effectRadius, 0.0f, SKYBLUE, SKILL_PROJECTILE_LASER, 0.0f});
            }
            break;
        }

        case SKILL_THUNDER_STRIKE: {
            std::vector<Enemy*> targets;
            for (Enemy* enemy : enemies) {
                if (!enemy) continue;
                if (Vector2Distance(playerPos, {enemy->getX(), enemy->getY()}) <= stats.range) {
                    targets.push_back(enemy);
                }
            }
            if (targets.empty()) return;

            for (int i = 0; i < stats.count && !targets.empty(); i++) {
                int randomIndex = GetRandomValue(0, (int)targets.size() - 1);
                Enemy* target = targets[randomIndex];
                if (target) {
                    Vector2 targetPos = {target->getX(), target->getY()};
                    target->takeDamage(totalDamage);
                    projectiles.push_back({targetPos, {0.0f, 0.0f}, 0.25f, stats.effectRadius, 0.0f, YELLOW, SKILL_PROJECTILE_THUNDER, 0.0f});
                }
                targets.erase(targets.begin() + randomIndex);
            }
            break;
        }

        case SKILL_SHIELD:
            for (int i = 0; i < stats.count; i++) {
                float angle = (2.0f * PI * i) / stats.count;
                Vector2 dir = {cosf(angle), sinf(angle)};
                projectiles.push_back({
                    playerPos,
                    {dir.x * stats.speed, dir.y * stats.speed},
                    1.2f,
                    stats.effectRadius,
                    (float)totalDamage,
                    BLUE,
                    SKILL_PROJECTILE_SHIELD,
                    angle * RAD2DEG
                });
            }
            break;

        case SKILL_HAMMER: {
            Enemy* target = findNearestEnemy(playerPos, enemies);
            if (!target) return;

            Vector2 baseDir = Vector2Subtract({target->getX(), target->getY()}, playerPos);
            if (baseDir.x == 0.0f && baseDir.y == 0.0f) {
                baseDir = {1.0f, 0.0f};
            } else {
                baseDir = Vector2Normalize(baseDir);
            }
            for (int i = 0; i < stats.count; i++) {
                float angleOffset = (float)(i - stats.count / 2) * 8.0f;
                if (stats.count % 2 == 0) angleOffset += 4.0f;
                Vector2 dir = Vector2Rotate(baseDir, angleOffset * DEG2RAD);
                projectiles.push_back({
                    playerPos,
                    {dir.x * stats.speed, dir.y * stats.speed},
                    1.5f,
                    stats.effectRadius,
                    (float)(totalDamage + (stats.special ? 20 : 0)),
                    ORANGE,
                    SKILL_PROJECTILE_HAMMER,
                    0.0f
                });
            }
            break;
        }
    }
}

void Skill::draw() const {
    if (skillLevel <= 0 || player == nullptr) return;

    static Texture2D shurikenTexture = LoadTexture("Graphics/shuriken.png");
    static Texture2D thunderTexture = LoadTexture("Graphics/thunderdungdung.png");
    static Texture2D shieldTexture = LoadTexture("Graphics/khiendoitruongmy.png");
    static Texture2D hammerTexture = LoadTexture("Graphics/buathor.png");

    Vector2 playerPos = {player->getX(), player->getY()};

    if (skillType == SKILL_AUTO_BALLS) {
        drawOrbitSkill(playerPos, orbitAngle, stats, Texture2D{}, BLUE);
    } else if (skillType == SKILL_SHURIKEN) {
        drawOrbitSkill(playerPos, orbitAngle, stats, shurikenTexture, SKYBLUE);
    }

    for (const SkillProjectile& projectile : projectiles) {
        if (projectile.type == SKILL_PROJECTILE_LASER) {
            Vector2 endPos = Vector2Add(projectile.position, projectile.velocity);
            DrawLineEx(projectile.position, endPos, projectile.radius, projectile.color);
            DrawLineEx(projectile.position, endPos, projectile.radius / 3.0f, WHITE);
        } else if (projectile.type == SKILL_PROJECTILE_THUNDER) {
            if (thunderTexture.id > 0) {
                DrawTexturePro(
                    thunderTexture,
                    {0.0f, 0.0f, (float)thunderTexture.width, (float)thunderTexture.height},
                    {projectile.position.x, projectile.position.y - projectile.radius * 0.5f, projectile.radius * 0.45f, projectile.radius},
                    {projectile.radius * 0.225f, projectile.radius * 0.5f},
                    0.0f,
                    WHITE
                );
            } else {
                DrawLineEx({projectile.position.x, projectile.position.y - projectile.radius}, projectile.position, 6.0f, YELLOW);
                DrawCircleV(projectile.position, 16.0f, ORANGE);
            }
        } else if (projectile.type == SKILL_PROJECTILE_SHIELD) {
            if (shieldTexture.id > 0) {
                DrawTexturePro(
                    shieldTexture,
                    {0.0f, 0.0f, (float)shieldTexture.width, (float)shieldTexture.height},
                    {projectile.position.x, projectile.position.y, projectile.radius * 2.0f, projectile.radius * 2.0f},
                    {projectile.radius, projectile.radius},
                    projectile.angle,
                    WHITE
                );
            } else {
                DrawCircleV(projectile.position, projectile.radius, BLUE);
            }
        } else if (projectile.type == SKILL_PROJECTILE_HAMMER) {
            if (hammerTexture.id > 0) {
                DrawTexturePro(
                    hammerTexture,
                    {0.0f, 0.0f, (float)hammerTexture.width, (float)hammerTexture.height},
                    {projectile.position.x, projectile.position.y, projectile.radius * 3.0f, projectile.radius * 3.0f},
                    {projectile.radius * 1.5f, projectile.radius * 1.5f},
                    projectile.angle,
                    WHITE
                );
            } else {
                DrawCircleV(projectile.position, projectile.radius, ORANGE);
            }
        } else {
            DrawCircleV(projectile.position, projectile.radius, projectile.color);
        }
    }
}

void updateSkillProjectiles(std::vector<SkillProjectile>& projectiles, std::vector<Enemy*>& enemies, float dt) {
    for (int i = (int)projectiles.size() - 1; i >= 0; i--) {
        SkillProjectile& projectile = projectiles[i];

        projectile.position.x += projectile.velocity.x * dt;
        projectile.position.y += projectile.velocity.y * dt;
        projectile.lifeTime -= dt;
        projectile.angle += 360.0f * dt;

        if (projectile.damage > 0.0f) {
            for (Enemy* enemy : enemies) {
                if (!enemy) continue;
                if (CheckCollisionCircles(projectile.position, projectile.radius, {enemy->getX(), enemy->getY()}, 10.0f)) {
                    enemy->takeDamage((int)projectile.damage);
                    projectile.lifeTime = 0.0f;
                    break;
                }
            }
        }

        if (projectile.lifeTime <= 0.0f) {
            projectiles.erase(projectiles.begin() + i);
        }
    }
}

void drawSkillProjectiles(const std::vector<SkillProjectile>& projectiles) {
    for (const SkillProjectile& projectile : projectiles) {
        DrawCircleV(projectile.position, projectile.radius, projectile.color);
    }
}
