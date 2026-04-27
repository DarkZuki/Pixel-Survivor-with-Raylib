#include "Skill.h"
#include "SkillLevel.h"
#include "raymath.h"

Vector2 getSkillEnemyPos(Enemy* enemy) {
    return {(float)enemy->getX(), (float)enemy->getY()};
}

Vector2 getSkillDir(Vector2 from, Vector2 to) {
    Vector2 dir = Vector2Subtract(to, from);
    if (dir.x == 0.0f && dir.y == 0.0f) return {1.0f, 0.0f};
    return Vector2Normalize(dir);
}

Enemy* getNearestSkillEnemy(Vector2 center, const std::vector<Enemy*>& enemies) {
    Enemy* target = nullptr;
    float best = 999999.0f;

    for (Enemy* enemy : enemies) {
        if (!enemy) continue;
        float dist = Vector2Distance(center, getSkillEnemyPos(enemy));
        if (dist <= best) {
            best = dist;
            target = enemy;
        }
    }

    return target;
}

Skill::Skill(Player* owner, int type) {
    player = owner;
    skillType = type;
    skillLevel = 1;
    texture = {0};
    currentCooldownTimer = 0.0f;
    orbitAngle = 0.0f;
    updateSkillStats();
    loadTexture();
}

Skill::~Skill() {
    if (texture.id > 0) UnloadTexture(texture);
}

void Skill::loadTexture() {
    if (texture.id > 0) UnloadTexture(texture);
    texture = {0};

    switch (skillType) {
        case 2: texture = LoadTexture("Graphics/thunderdungdung.png"); break;
        case 3: texture = LoadTexture("Graphics/shuriken.png"); break;
        case 4: texture = LoadTexture("Graphics/khiendoitruongmy.png"); break;
        case 5: texture = LoadTexture("Graphics/buathor.png"); break;
    }
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
    if (skillLevel <= 0 || !player) return;

    float dt = GetFrameTime();
    currentCooldownTimer += dt;
    orbitAngle += dt * 2.5f;

    if (skillType == 0 || skillType == 3 || currentCooldownTimer >= stats.cooldown) {
        if (skillType != 0 && skillType != 3) currentCooldownTimer = 0.0f;
        attack(enemies);
    }

    updateSkillProjectiles(projectiles, enemies, dt);
}

void Skill::attack(const std::vector<Enemy*>& enemies) {
    if (skillLevel <= 0 || !player) return;

    Vector2 playerPos = {player->getX(), player->getY()};
    int totalDamage = stats.damage + player->getDamage();

    switch (skillType) {
        case 0:
        case 3:
            for (int i = 0; i < stats.count; i++) {
                float angle = orbitAngle + (2.0f * PI * i) / stats.count;
                Vector2 pos = {playerPos.x + cosf(angle) * stats.range, playerPos.y + sinf(angle) * stats.range};
                for (Enemy* enemy : enemies) {
                    if (!enemy) continue;
                    if (CheckCollisionCircles(pos, stats.effectRadius, getSkillEnemyPos(enemy), 20.0f)) {
                        enemy->takeDamage(totalDamage);
                    }
                }
            }
            break;

        case 1: {
            Enemy* target = getNearestSkillEnemy(playerPos, enemies);
            if (!target) break;

            Vector2 dir = getSkillDir(playerPos, getSkillEnemyPos(target));
            Vector2 end = Vector2Add(playerPos, Vector2Scale(dir, stats.range));

            for (Enemy* enemy : enemies) {
                if (!enemy) continue;
                Vector2 enemyPos = getSkillEnemyPos(enemy);
                if (CheckCollisionCircleLine(enemyPos, 20.0f, playerPos, end)) enemy->takeDamage(totalDamage);
                if (stats.special) {
                    Vector2 back = Vector2Add(playerPos, Vector2Scale(dir, -stats.range));
                    if (CheckCollisionCircleLine(enemyPos, 20.0f, playerPos, back)) enemy->takeDamage(totalDamage);
                }
            }

            projectiles.push_back({playerPos, Vector2Scale(dir, stats.range), 0.2f, stats.effectRadius, 0.0f, SKYBLUE, 0, 0.0f});
            if (stats.special) {
                projectiles.push_back({playerPos, Vector2Scale(dir, -stats.range), 0.2f, stats.effectRadius, 0.0f, SKYBLUE, 0, 0.0f});
            }
            break;
        }

        case 2: {
            std::vector<Enemy*> targets;
            for (Enemy* enemy : enemies) {
                if (!enemy) continue;
                if (Vector2Distance(playerPos, getSkillEnemyPos(enemy)) <= stats.range) targets.push_back(enemy);
            }

            for (int i = 0; i < stats.count && !targets.empty(); i++) {
                int index = GetRandomValue(0, (int)targets.size() - 1);
                Vector2 pos = getSkillEnemyPos(targets[index]);
                targets[index]->takeDamage(totalDamage);
                projectiles.push_back({pos, {0, 0}, 0.25f, stats.effectRadius, 0.0f, YELLOW, 1, 0.0f});
                targets.erase(targets.begin() + index);
            }
            break;
        }

        case 4:
        case 5: {
            Vector2 dir = {1.0f, 0.0f};
            if (skillType == 5) {
                Enemy* target = getNearestSkillEnemy(playerPos, enemies);
                if (!target) break;
                dir = getSkillDir(playerPos, getSkillEnemyPos(target));
            }

            for (int i = 0; i < stats.count; i++) {
                float angleOffset = (float)(i - stats.count / 2) * 8.0f;
                if (stats.count % 2 == 0) angleOffset += 4.0f;
                Vector2 velocity = skillType == 4
                    ? Vector2Rotate(dir, (2.0f * PI * i) / stats.count)
                    : Vector2Rotate(dir, angleOffset * DEG2RAD);

                projectiles.push_back({
                    playerPos,
                    {velocity.x * stats.speed, velocity.y * stats.speed},
                    skillType == 4 ? 1.2f : 1.5f,
                    stats.effectRadius,
                    (float)(totalDamage + (skillType == 5 && stats.special ? 20 : 0)),
                    skillType == 4 ? BLUE : ORANGE,
                    skillType == 4 ? 2 : 3,
                    skillType == 4 ? (2.0f * PI * i) * RAD2DEG : 0.0f
                });
            }
            break;
        }
    }
}

void Skill::draw() const {
    if (skillLevel <= 0 || !player) return;

    Vector2 playerPos = {player->getX(), player->getY()};

    if (skillType == 0 || skillType == 3) {
        for (int i = 0; i < stats.count; i++) {
            float angle = orbitAngle + (2.0f * PI * i) / stats.count;
            Vector2 pos = {playerPos.x + cosf(angle) * stats.range, playerPos.y + sinf(angle) * stats.range};

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
                DrawCircleV(pos, stats.effectRadius, skillType == 0 ? BLUE : SKYBLUE);
            }
        }
    }

    for (const SkillProjectile& projectile : projectiles) {
        switch (projectile.type) {
            case 0: {
                Vector2 end = Vector2Add(projectile.position, projectile.velocity);
                DrawLineEx(projectile.position, end, projectile.radius, projectile.color);
                DrawLineEx(projectile.position, end, projectile.radius / 3.0f, WHITE);
                break;
            }
            case 1:
                if (texture.id > 0) {
                    DrawTexturePro(
                        texture,
                        {0.0f, 0.0f, (float)texture.width, (float)texture.height},
                        {projectile.position.x, projectile.position.y - projectile.radius * 0.5f, projectile.radius * 0.45f, projectile.radius},
                        {projectile.radius * 0.225f, projectile.radius * 0.5f},
                        0.0f,
                        WHITE
                    );
                } else {
                    DrawLineEx({projectile.position.x, projectile.position.y - projectile.radius}, projectile.position, 6.0f, YELLOW);
                    DrawCircleV(projectile.position, 16.0f, ORANGE);
                }
                break;
            case 2:
            case 3:
                if (texture.id > 0) {
                    float size = projectile.type == 2 ? projectile.radius * 2.0f : projectile.radius * 3.0f;
                    float origin = projectile.type == 2 ? projectile.radius : projectile.radius * 1.5f;
                    DrawTexturePro(
                        texture,
                        {0.0f, 0.0f, (float)texture.width, (float)texture.height},
                        {projectile.position.x, projectile.position.y, size, size},
                        {origin, origin},
                        projectile.angle,
                        WHITE
                    );
                } else {
                    DrawCircleV(projectile.position, projectile.radius, projectile.color);
                }
                break;
            default:
                DrawCircleV(projectile.position, projectile.radius, projectile.color);
                break;
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
                if (CheckCollisionCircles(projectile.position, projectile.radius, getSkillEnemyPos(enemy), 10.0f)) {
                    enemy->takeDamage((int)projectile.damage);
                    projectile.lifeTime = 0.0f;
                    break;
                }
            }
        }

        if (projectile.lifeTime <= 0.0f) projectiles.erase(projectiles.begin() + i);
    }
}

void drawSkillProjectiles(const std::vector<SkillProjectile>& projectiles) {
    for (const SkillProjectile& projectile : projectiles) {
        DrawCircleV(projectile.position, projectile.radius, projectile.color);
    }
}
