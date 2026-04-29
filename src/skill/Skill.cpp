#include "Skill.h"
#include "SkillLevel.h"
#include "raymath.h"
#include <cfloat>

namespace {
// Chuan hoa huong bay, neu vector rong thi tra ve huong mac dinh
Vector2 NormalizeOrFallback(Vector2 direction) {
    return Vector2Length(direction) > 0.0f ? Vector2Normalize(direction) : Vector2{1.0f, 0.0f};
}

Enemy* FindNearestEnemy(const Vector2& from, const std::vector<Enemy*>& enemies) {
    Enemy* nearest = nullptr;
    float bestDistance = FLT_MAX;

    for (Enemy* enemy : enemies) {
        if (!enemy || enemy->getHp() <= 0) continue;
        float distance = Vector2Distance(from, {(float)enemy->getX(), (float)enemy->getY()});
        if (distance < bestDistance) {
            bestDistance = distance;
            nearest = enemy;
        }
    }

    return nearest;
}
}

Skill::Skill(Player* owner, int type) {
    // Khoi tao skill va dong bo ngay stat/texture theo type
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
    // Moi lan doi skill se clear texture cu roi tai texture moi
    if (texture.id > 0) UnloadTexture(texture);
    texture = {0};
    if (skillType == SKILL_THUNDER_STRIKE) texture = LoadTexture("Graphics/thunderdungdung.png");
    if (skillType == SKILL_SHURIKEN) texture = LoadTexture("Graphics/shuriken.png");
    if (skillType == SKILL_SHIELD) texture = LoadTexture("Graphics/khiendoitruongmy.png");
    if (skillType == SKILL_HAMMER) texture = LoadTexture("Graphics/buathor.png");
}

const char* Skill::getName() const { return getSkillLevelSkillName(skillType); }
int Skill::getLevel() const { return skillLevel; }
void Skill::updateSkillStats() { stats = getSkillStats(skillType, skillLevel); }

void Skill::setLevel(int newLevel) {
    // Khoa level vao khoang hop le va tat projectile neu level = 0
    skillLevel = newLevel < 0 ? 0 : (newLevel > 10 ? 10 : newLevel);
    if (skillLevel <= 0) projectiles.clear();
    updateSkillStats();
}

void Skill::update(std::vector<Enemy*>& enemies) {
    if (skillLevel <= 0 || !player) return;
    // Tang cooldown va kich hoat attack khi du dieu kien
    float dt = GetFrameTime();
    currentCooldownTimer += dt;
    orbitAngle += dt * 2.5f;
    if (skillType == SKILL_SHURIKEN || currentCooldownTimer >= stats.cooldown) {
        if (skillType != SKILL_SHURIKEN) currentCooldownTimer = 0.0f;
        attack(enemies);
    }
    updateSkillProjectiles(projectiles, enemies, dt);
}

void Skill::attack(const std::vector<Enemy*>& enemies) {
    if (skillLevel <= 0 || !player) return;

    Vector2 playerPos = {player->getX(), player->getY()};
    int totalDamage = stats.damage + player->getDamage();
    Enemy* target = nullptr;
    float best = 999999.0f;

    // Tim enemy gan nhat cho cac skill can target
    for (Enemy* enemy : enemies) {
        if (!enemy) continue;
        float ex = enemy->getX(), ey = enemy->getY();
        float dist = Vector2Distance(playerPos, {ex, ey});
        if (dist <= best) { best = dist; target = enemy; }
    }

    switch (skillType) {
        case SKILL_SHURIKEN:
            // Shuriken quay quanh player va gay damage lien tuc
            for (int i = 0; i < stats.count; i++) {
                float angle = orbitAngle + 2.0f * PI * i / stats.count;
                Vector2 pos = {playerPos.x + cosf(angle) * stats.range, playerPos.y + sinf(angle) * stats.range};
                for (Enemy* enemy : enemies) {
                    if (enemy && CheckCollisionCircles(pos, stats.effectRadius, {(float)enemy->getX(), (float)enemy->getY()}, 20.0f)) enemy->takeDamage(totalDamage);
                }
            }
            break;

        case SKILL_LASER_BEAM:
            if (!target) break;
            {
                // Laser ban thang theo huong target, special se co them tia nguoc
                Vector2 dir = NormalizeOrFallback(Vector2Subtract({target->getX(), target->getY()}, playerPos));
                Vector2 end = Vector2Add(playerPos, Vector2Scale(dir, stats.range));
                Vector2 back = Vector2Add(playerPos, Vector2Scale(dir, -stats.range));
                for (Enemy* enemy : enemies) {
                    if (!enemy) continue;
                    Vector2 enemyPos = {(float)enemy->getX(), (float)enemy->getY()};
                    if (CheckCollisionCircleLine(enemyPos, 30.0f, playerPos, end)) enemy->takeDamage(totalDamage);
                    if (stats.special && CheckCollisionCircleLine(enemyPos, 20.0f, playerPos, back)) enemy->takeDamage(totalDamage);
                }
                projectiles.push_back({playerPos, Vector2Scale(dir, stats.range), 0.2f, stats.effectRadius, 0.0f, SKYBLUE, 0, 0.0f, 0});
                if (stats.special) projectiles.push_back({playerPos, Vector2Scale(dir, -stats.range), 0.2f, stats.effectRadius, 0.0f, SKYBLUE, 0, 0.0f, 0});
            }
            break;

        case SKILL_THUNDER_STRIKE: {
            // Set danh ngau nhien cac muc tieu trong tam
            std::vector<Enemy*> targets;
            for (Enemy* enemy : enemies) {
                if (enemy && Vector2Distance(playerPos, {(float)enemy->getX(), (float)enemy->getY()}) <= stats.range) targets.push_back(enemy);
            }
            for (int i = 0; i < stats.count && !targets.empty(); i++) {
                int index = GetRandomValue(0, (int)targets.size() - 1);
                Enemy* enemy = targets[index];
                enemy->takeDamage(totalDamage);
                projectiles.push_back({{(float)enemy->getX(), (float)enemy->getY()}, {0, 0}, 0.25f, stats.effectRadius, 0.0f, YELLOW, 1, 0.0f, 0});
                targets.erase(targets.begin() + index);
            }
            break;
        }

        case SKILL_SHIELD:
        case SKILL_HAMMER:
            if (skillType == SKILL_HAMMER && !target) break;
            {
                // Shield va hammer sinh projectile theo doi hinh rieng
                Vector2 dir = {1.0f, 0.0f};
                if (skillType == SKILL_HAMMER) {
                    dir = NormalizeOrFallback(Vector2Subtract({target->getX(), target->getY()}, playerPos));
                }
                for (int i = 0; i < stats.count; i++) {
                    float angle = skillType == SKILL_SHIELD ? 2.0f * PI * i / stats.count : ((float)(i - stats.count / 2) * 8.0f + (stats.count % 2 == 0 ? 4.0f : 0.0f)) * DEG2RAD;
                    Vector2 velocity = Vector2Rotate(dir, angle);
                    projectiles.push_back({playerPos, {velocity.x * stats.speed, velocity.y * stats.speed}, skillType == SKILL_SHIELD ? 1.2f : 1.5f, stats.effectRadius, (float)(totalDamage + (skillType == SKILL_HAMMER && stats.special ? 20 : 0)), skillType == SKILL_SHIELD ? BLUE : ORANGE, skillType == SKILL_SHIELD ? 2 : 3, skillType == SKILL_SHIELD ? angle * RAD2DEG : 0.0f, skillType == SKILL_SHIELD ? 3 : 0});
                }
            }
            break;
    }
}

void Skill::draw() const {
    if (skillLevel <= 0 || !player) return;
    Vector2 playerPos = {player->getX(), player->getY()};

    if (skillType == SKILL_SHURIKEN) {
        // Ve shuriken dang orbit quanh player
        for (int i = 0; i < stats.count; i++) {
            float angle = orbitAngle + 2.0f * PI * i / stats.count;
            Vector2 pos = {playerPos.x + cosf(angle) * stats.range, playerPos.y + sinf(angle) * stats.range};
            if (texture.id > 0) DrawTexturePro(texture, {0, 0, (float)texture.width, (float)texture.height}, {pos.x, pos.y, stats.effectRadius * 2.0f, stats.effectRadius * 2.0f}, {stats.effectRadius, stats.effectRadius}, GetTime() * 720.0f, WHITE);
            else DrawCircleV(pos, stats.effectRadius, SKYBLUE);
        }
    }

    for (const SkillProjectile& projectile : projectiles) {
        if (projectile.type == 0) {
            // Ve laser beam
            Vector2 end = Vector2Add(projectile.position, projectile.velocity);
            DrawLineEx(projectile.position, end, projectile.radius, projectile.color);
            DrawLineEx(projectile.position, end, projectile.radius / 3.0f, WHITE);
        } else if (projectile.type == 1) {
            // Ve hieu ung set roi xuong
            if (texture.id > 0) DrawTexturePro(texture, {0, 0, (float)texture.width, (float)texture.height}, {projectile.position.x, projectile.position.y - projectile.radius * 0.5f, projectile.radius * 0.45f, projectile.radius}, {projectile.radius * 0.225f, projectile.radius * 0.5f}, 0.0f, WHITE);
            else {
                DrawLineEx({projectile.position.x, projectile.position.y - projectile.radius}, projectile.position, 6.0f, YELLOW);
                DrawCircleV(projectile.position, 16.0f, ORANGE);
            }
        } else if (projectile.type == 2 || projectile.type == 3) {
            // Ve shield/hammer projectile bang texture neu co
            if (texture.id > 0) {
                float size = projectile.type == 2 ? projectile.radius * 2.0f : projectile.radius * 3.0f;
                float origin = projectile.type == 2 ? projectile.radius : projectile.radius * 1.5f;
                DrawTexturePro(texture, {0, 0, (float)texture.width, (float)texture.height}, {projectile.position.x, projectile.position.y, size, size}, {origin, origin}, projectile.angle, WHITE);
            } else DrawCircleV(projectile.position, projectile.radius, projectile.color);
        } else DrawCircleV(projectile.position, projectile.radius, projectile.color);
    }
}

void updateSkillProjectiles(std::vector<SkillProjectile>& projectiles, std::vector<Enemy*>& enemies, float dt) {
    // Duyet nguoc de xoa projectile an toan trong luc update
    for (int i = (int)projectiles.size() - 1; i >= 0; i--) {
        SkillProjectile& projectile = projectiles[i];
        if (projectile.type == 2) {
            // Shield tu dong doi huong ve enemy gan nhat trong moi frame
            Enemy* target = FindNearestEnemy(projectile.position, enemies);
            if (target) {
                float speed = Vector2Length(projectile.velocity);
                Vector2 desiredDirection = NormalizeOrFallback(Vector2Subtract({(float)target->getX(), (float)target->getY()}, projectile.position));
                projectile.velocity = Vector2Scale(desiredDirection, speed);
                projectile.angle = atan2f(projectile.velocity.y, projectile.velocity.x) * RAD2DEG;
            }
        }
        projectile.position.x += projectile.velocity.x * dt;
        projectile.position.y += projectile.velocity.y * dt;
        projectile.lifeTime -= dt;
        projectile.angle += 360.0f * dt;
        if (projectile.damage > 0.0f) {
            // Projectile gay damage se bien mat sau khi cham enemy
            for (Enemy* enemy : enemies) {
                if (enemy && enemy->getHp() > 0 && CheckCollisionCircles(projectile.position, projectile.radius, {(float)enemy->getX(), (float)enemy->getY()}, 10.0f)) {
                    enemy->takeDamage((int)projectile.damage);
                    if (projectile.type == 2) {
                        // Shield chi duoc nay 3 lan, moi lan cham se doi muc tieu gan nhat tiep theo
                        projectile.remainingBounces--;
                        if (projectile.remainingBounces <= 0) {
                            projectile.lifeTime = 0.0f;
                            break;
                        }
                        Vector2 enemyPos = {(float)enemy->getX(), (float)enemy->getY()};
                        Enemy* nextTarget = FindNearestEnemy(enemyPos, enemies);
                        Vector2 bounceDir = nextTarget && nextTarget != enemy
                            ? NormalizeOrFallback(Vector2Subtract({(float)nextTarget->getX(), (float)nextTarget->getY()}, enemyPos))
                            : NormalizeOrFallback(Vector2Subtract(projectile.position, enemyPos));
                        float speed = Vector2Length(projectile.velocity);
                        projectile.velocity = Vector2Scale(bounceDir, speed);
                        projectile.position = Vector2Add(enemyPos, Vector2Scale(bounceDir, projectile.radius + 12.0f));
                        projectile.angle = atan2f(projectile.velocity.y, projectile.velocity.x) * RAD2DEG;
                    } else {
                        projectile.lifeTime = 0.0f;
                    }
                    break;
                }
            }
        }
        if (projectile.lifeTime <= 0.0f) projectiles.erase(projectiles.begin() + i);
    }
}

void drawSkillProjectiles(const std::vector<SkillProjectile>& projectiles) {
    // Ham ve don gian bang circle de dung khi can
    for (const SkillProjectile& projectile : projectiles) DrawCircleV(projectile.position, projectile.radius, projectile.color);
}
