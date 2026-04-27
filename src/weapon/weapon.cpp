#include "weapon.h"
#include "level.h"
#include "raylib.h"
#include <cmath>

namespace {
float DistanceVec(Vector2 a, Vector2 b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return sqrtf(dx * dx + dy * dy);
}

Vector2 NormalizeVec(Vector2 v) {
    float length = sqrtf(v.x * v.x + v.y * v.y);
    if (length <= 0.0f) return {1.0f, 0.0f};
    return {v.x / length, v.y / length};
}

Vector2 RotateVec(Vector2 v, float angle) {
    float cosine = cosf(angle);
    float sine = sinf(angle);
    return {
        v.x * cosine - v.y * sine,
        v.x * sine + v.y * cosine
    };
}
}

Weapon::Weapon(int type) {
    weaponType = type;
    weaponLevel = 1;
    currentCooldownTimer = 0.0f;
    updateWeaponStats();
}

const char* Weapon::getName() const {
    return getWeaponLevelWeaponName(weaponType);
}

int Weapon::getLevel() const {
    return weaponLevel;
}

void Weapon::setLevel(int newLevel) {
    weaponLevel = newLevel < 0 ? 0 : (newLevel > 10 ? 10 : newLevel);
    updateWeaponStats();
}

// Hàm cập nhật chỉ số khi lên cấp
void Weapon::updateWeaponStats() {
    stats = getWeaponStats(weaponType, weaponLevel);
}

// Hàm kiểm tra cooldown và cho phép tấn công
void Weapon::update(Player& player, const std::vector<Enemy*>& enemies,
                    std::vector<WeaponProjectile>& projectiles, Vector2 targetPosition, bool isAttacking) {
    if (weaponLevel <= 0) return;

    currentCooldownTimer += GetFrameTime();
    if (isAttacking && currentCooldownTimer >= stats.cooldown) {
        currentCooldownTimer = 0.0f;
        attack(player, enemies, projectiles, targetPosition);
    }
}

// Hàm thực hiện tấn công 
void Weapon::attack(Player& player, const std::vector<Enemy*>& enemies,
                    std::vector<WeaponProjectile>& projectiles, Vector2 targetPosition) {
    if (weaponLevel <= 0) return;

    Vector2 playerPos = {player.getX(), player.getY()};
    int totalDamage = stats.damage + player.getDamage();

    switch (weaponType) {
        
        // Búa - Hammer
        case 0:
            for (Enemy* enemy : enemies) {
                if (!enemy) continue;
                if (DistanceVec(playerPos, {enemy->getX(), enemy->getY()}) <= stats.range) {
                    enemy->takeDamage(totalDamage);
                    if (stats.doubleHit) enemy->takeDamage(totalDamage);
                }
            }
            projectiles.push_back({playerPos, {0, 0}, 0.2f, stats.range, 0, ORANGE, 1, 0});
            break;
        
        // Gậy phép - Magic Wand
        case 1: {
            int shotsFired = 0;
            for (Enemy* enemy : enemies) {
                if (!enemy) continue;
                if (shotsFired >= stats.count) break;
                if (DistanceVec(playerPos, {enemy->getX(), enemy->getY()}) <= stats.range) {
                    
                    // Thuật toán kiểm tra vị trí enemies trong bán kính cho phép auto bắn
                    Vector2 dir = {enemy->getX() - playerPos.x, enemy->getY() - playerPos.y};
                    if (dir.x == 0 && dir.y == 0) dir = {1.0f, 0.0f};
                    else dir = NormalizeVec(dir);
                    projectiles.push_back({playerPos, {dir.x * stats.speed, dir.y * stats.speed},
                                           2.0f, 6.0f, (float)totalDamage, PURPLE, 0, 0});
                    shotsFired++;
                }
            }
            break;
        }

        // Dao ném - Knife
        case 2: {
            Vector2 dir = {targetPosition.x - playerPos.x, targetPosition.y - playerPos.y};
            if (dir.x == 0 && dir.y == 0) dir = {1.0f, 0.0f};
            else dir = NormalizeVec(dir);
            for (int i = 0; i < stats.count; i++) {
                
                // Thuật toán căn chỉnh góc độ khi số lượng dao tăng
                float angleOffset = (float)(i - stats.count / 2) * 8.0f;
                if (stats.count % 2 == 0) angleOffset += 4.0f;
                Vector2 v = RotateVec(dir, angleOffset * DEG2RAD);
                projectiles.push_back({playerPos, {v.x * stats.speed, v.y * stats.speed},
                                       1.0f, 4.0f, (float)totalDamage, SKYBLUE, 0, 0});
            }
            break;
        }

        // Sách phép - Spell Book
        case 3: {
            Vector2 dir = {targetPosition.x - playerPos.x, targetPosition.y - playerPos.y};
            if (dir.x == 0 && dir.y == 0) dir = {1.0f, 0.0f};
            else dir = NormalizeVec(dir);
            for (int i = 0; i < stats.count; i++) {

                // Thuật toán căn chỉnh góc độ khi số lượng đạn tăng
                float angleOffset = (float)(i - stats.count / 2) * 10.0f;
                if (stats.count % 2 == 0) angleOffset += 5.0f;
                Vector2 v = RotateVec(dir, angleOffset * DEG2RAD);
                projectiles.push_back({playerPos, {v.x * stats.speed, v.y * stats.speed},
                                       2.0f, 8.0f, (float)totalDamage, PURPLE, 2, stats.explosionRadius});
            }
            break;
        }
    }
}

// Hàm xử lý va chạm
void updateProjectiles(std::vector<WeaponProjectile>& projectiles, std::vector<Enemy*>& enemies, float dt) {
    std::vector<WeaponProjectile> effects;

    for (int i = (int)projectiles.size() - 1; i >= 0; i--) {
        WeaponProjectile& p = projectiles[i];

        // Tính vị trí khi cộng với khoảng cách bằng công thức vật lý
        p.position.x += p.velocity.x * dt;
        p.position.y += p.velocity.y * dt;
        p.lifeTime -= dt;

        if (p.damage > 0) {
            for (Enemy* enemy : enemies) {
                if (!enemy) continue;
                if (CheckCollisionCircles(p.position, p.radius, {enemy->getX(), enemy->getY()}, 10)) {
                    enemy->takeDamage((int)p.damage);

                    // Trường hợp đạn từ sách phép nổ
                    if (p.type == 2) {
                        for (Enemy* splashEnemy : enemies) {
                            if (!splashEnemy) continue;
                            if (DistanceVec(p.position, {splashEnemy->getX(), splashEnemy->getY()}) <= p.angle) {
                                splashEnemy->takeDamage((int)(p.damage / 2));
                            }
                        }
                        effects.push_back({p.position, {0, 0}, 0.3f, 0, 0, ORANGE, 2, p.angle});
                    }

                    p.lifeTime = 0;
                    break;
                }
            }
        }

        // Thời gian cho phép tồn tại hết sẽ mất --> tránh gây lag
        if (p.lifeTime <= 0) {
            projectiles.erase(projectiles.begin() + i);
        }
    }

    for (WeaponProjectile effect : effects) projectiles.push_back(effect);
}

// Hàm vẽ các vật thể bay sử dụng raylib
void drawProjectiles(const std::vector<WeaponProjectile>& projectiles) {
    for (const WeaponProjectile& p : projectiles) {
        if (p.type == 1) {
            DrawCircleV(p.position, p.radius, Fade(ORANGE, p.lifeTime * 5));
            DrawCircleLinesV(p.position, p.radius, Fade(YELLOW, p.lifeTime * 5));
        } else if (p.type == 2 && p.damage == 0) {
            DrawCircleV(p.position, p.angle, Fade(ORANGE, p.lifeTime * 3));
        } else {
            DrawCircleV(p.position, p.radius, p.color);
        }
    }
}
