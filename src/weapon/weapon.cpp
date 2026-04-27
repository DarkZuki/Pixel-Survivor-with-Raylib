#include "weapon.h"
#include "level.h"
#include "raylib.h"
#include "raymath.h"

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

void Weapon::updateWeaponStats() {
    stats = getWeaponStats(weaponType, weaponLevel);
}

void Weapon::update(Player& player, const std::vector<Enemy*>& enemies,
                    std::vector<WeaponProjectile>& projectiles, Vector2 targetPosition, bool isAttacking) {
    if (weaponLevel <= 0) return;

    currentCooldownTimer += GetFrameTime();
    if (isAttacking && currentCooldownTimer >= stats.cooldown) {
        currentCooldownTimer = 0.0f;
        attack(player, enemies, projectiles, targetPosition);
    }
}

void Weapon::attack(Player& player, const std::vector<Enemy*>& enemies,
                    std::vector<WeaponProjectile>& projectiles, Vector2 targetPosition) {
    if (weaponLevel <= 0) return;

    Vector2 playerPos = {player.getX(), player.getY()};
    int totalDamage = stats.damage + player.getDamage();

    switch (weaponType) {
        case 0:
            for (Enemy* enemy : enemies) {
                if (!enemy) continue;
                if (Vector2Distance(playerPos, {enemy->getX(), enemy->getY()}) <= stats.range) {
                    enemy->takeDamage(totalDamage);
                    if (stats.doubleHit) enemy->takeDamage(totalDamage);
                }
            }
            projectiles.push_back({playerPos, {0, 0}, 0.2f, stats.range, 0, ORANGE, 1, 0});
            break;

        case 1: {
            int shotsFired = 0;
            for (Enemy* enemy : enemies) {
                if (!enemy) continue;
                if (shotsFired >= stats.count) break;
                if (Vector2Distance(playerPos, {enemy->getX(), enemy->getY()}) <= stats.range) {
                    Vector2 dir = {enemy->getX() - playerPos.x, enemy->getY() - playerPos.y};
                    if (dir.x == 0 && dir.y == 0) dir = {1.0f, 0.0f};
                    else dir = Vector2Normalize(dir);

                    projectiles.push_back({playerPos, {dir.x * stats.speed, dir.y * stats.speed},
                                           2.0f, 6.0f, (float)totalDamage, PURPLE, 0, 0});
                    shotsFired++;
                }
            }
            break;
        }

        case 2: {
            Vector2 dir = {targetPosition.x - playerPos.x, targetPosition.y - playerPos.y};
            if (dir.x == 0 && dir.y == 0) dir = {1.0f, 0.0f};
            else dir = Vector2Normalize(dir);

            for (int i = 0; i < stats.count; i++) {
                float angleOffset = (float)(i - stats.count / 2) * 8.0f;
                if (stats.count % 2 == 0) angleOffset += 4.0f;
                Vector2 velocity = Vector2Rotate(dir, angleOffset * DEG2RAD);
                projectiles.push_back({playerPos, {velocity.x * stats.speed, velocity.y * stats.speed},
                                       1.0f, 4.0f, (float)totalDamage, SKYBLUE, 0, 0});
            }
            break;
        }

        case 3: {
            Vector2 dir = {targetPosition.x - playerPos.x, targetPosition.y - playerPos.y};
            if (dir.x == 0 && dir.y == 0) dir = {1.0f, 0.0f};
            else dir = Vector2Normalize(dir);

            for (int i = 0; i < stats.count; i++) {
                float angleOffset = (float)(i - stats.count / 2) * 10.0f;
                if (stats.count % 2 == 0) angleOffset += 5.0f;
                Vector2 velocity = Vector2Rotate(dir, angleOffset * DEG2RAD);
                projectiles.push_back({playerPos, {velocity.x * stats.speed, velocity.y * stats.speed},
                                       2.0f, 8.0f, (float)totalDamage, PURPLE, 2, stats.explosionRadius});
            }
            break;
        }
    }
}

void updateProjectiles(std::vector<WeaponProjectile>& projectiles, std::vector<Enemy*>& enemies, float dt) {
    std::vector<WeaponProjectile> effects;

    for (int i = (int)projectiles.size() - 1; i >= 0; i--) {
        WeaponProjectile& projectile = projectiles[i];

        projectile.position.x += projectile.velocity.x * dt;
        projectile.position.y += projectile.velocity.y * dt;
        projectile.lifeTime -= dt;

        if (projectile.damage > 0) {
            for (Enemy* enemy : enemies) {
                if (!enemy) continue;
                if (CheckCollisionCircles(projectile.position, projectile.radius, {enemy->getX(), enemy->getY()}, 10)) {
                    enemy->takeDamage((int)projectile.damage);

                    if (projectile.type == 2) {
                        for (Enemy* splashEnemy : enemies) {
                            if (!splashEnemy) continue;
                            if (Vector2Distance(projectile.position, {splashEnemy->getX(), splashEnemy->getY()}) <= projectile.angle) {
                                splashEnemy->takeDamage((int)(projectile.damage / 2));
                            }
                        }
                        effects.push_back({projectile.position, {0, 0}, 0.3f, 0, 0, ORANGE, 2, projectile.angle});
                    }

                    projectile.lifeTime = 0;
                    break;
                }
            }
        }

        if (projectile.lifeTime <= 0) {
            projectiles.erase(projectiles.begin() + i);
        }
    }

    for (const WeaponProjectile& effect : effects) {
        projectiles.push_back(effect);
    }
}

void drawProjectiles(const std::vector<WeaponProjectile>& projectiles) {
    for (const WeaponProjectile& projectile : projectiles) {
        if (projectile.type == 1) {
            DrawCircleV(projectile.position, projectile.radius, Fade(ORANGE, projectile.lifeTime * 5));
            DrawCircleLinesV(projectile.position, projectile.radius, Fade(YELLOW, projectile.lifeTime * 5));
        } else if (projectile.type == 2 && projectile.damage == 0) {
            DrawCircleV(projectile.position, projectile.angle, Fade(ORANGE, projectile.lifeTime * 3));
        } else {
            DrawCircleV(projectile.position, projectile.radius, projectile.color);
        }
    }
}
