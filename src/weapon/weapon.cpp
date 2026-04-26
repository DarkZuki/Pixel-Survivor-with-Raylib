#include "weapon.h"
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
    WeaponStats stats = getWeaponStats(weaponType, weaponLevel);

    weaponDamage = stats.damage;
    attackCooldown = stats.cooldown;
    attackRange = stats.range;
    projectileSpeed = stats.speed;
    projectileCount = stats.count;
    explosionRadius = stats.explosionRadius;
    doubleHit = stats.doubleHit;
}

void Weapon::update(Player& player, const std::vector<Enemy*>& enemies,
                    std::vector<WeaponProjectile>& projectiles, Vector2 targetPosition, bool isAttacking) {
    if (weaponLevel <= 0) return;

    currentCooldownTimer += GetFrameTime();
    if (isAttacking && currentCooldownTimer >= attackCooldown) {
        currentCooldownTimer = 0.0f;
        attack(player, enemies, projectiles, targetPosition);
    }
}

void Weapon::attack(Player& player, const std::vector<Enemy*>& enemies,
                    std::vector<WeaponProjectile>& projectiles, Vector2 targetPosition) {
    if (weaponLevel <= 0) return;

    Vector2 playerPos = {player.getX(), player.getY()};
    int totalDamage = weaponDamage + player.getDamage();

    switch (weaponType) {
        case 0:
            for (Enemy* enemy : enemies) {
                if (!enemy) continue;
                if (Vector2Distance(playerPos, {enemy->getX(), enemy->getY()}) <= attackRange) {
                    enemy->takeDamage(totalDamage);
                    if (doubleHit) enemy->takeDamage(totalDamage);
                }
            }
            projectiles.push_back({playerPos, {0, 0}, 0.2f, attackRange, 0, ORANGE, 1, 0});
            break;

        case 1: {
            int shotsFired = 0;
            for (Enemy* enemy : enemies) {
                if (!enemy) continue;
                if (shotsFired >= projectileCount) break;
                if (Vector2Distance(playerPos, {enemy->getX(), enemy->getY()}) <= attackRange) {
                    Vector2 dir = {enemy->getX() - playerPos.x, enemy->getY() - playerPos.y};
                    if (dir.x == 0 && dir.y == 0) dir = {1.0f, 0.0f};
                    else dir = Vector2Normalize(dir);
                    projectiles.push_back({playerPos, {dir.x * projectileSpeed, dir.y * projectileSpeed},
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
            for (int i = 0; i < projectileCount; i++) {
                float angleOffset = (float)(i - projectileCount / 2) * 8.0f;
                if (projectileCount % 2 == 0) angleOffset += 4.0f;
                Vector2 v = Vector2Rotate(dir, angleOffset * DEG2RAD);
                projectiles.push_back({playerPos, {v.x * projectileSpeed, v.y * projectileSpeed},
                                       1.0f, 4.0f, (float)totalDamage, SKYBLUE, 0, 0});
            }
            break;
        }

        case 3: {
            Vector2 dir = {targetPosition.x - playerPos.x, targetPosition.y - playerPos.y};
            if (dir.x == 0 && dir.y == 0) dir = {1.0f, 0.0f};
            else dir = Vector2Normalize(dir);
            for (int i = 0; i < projectileCount; i++) {
                float angleOffset = (float)(i - projectileCount / 2) * 10.0f;
                if (projectileCount % 2 == 0) angleOffset += 5.0f;
                Vector2 v = Vector2Rotate(dir, angleOffset * DEG2RAD);
                projectiles.push_back({playerPos, {v.x * projectileSpeed, v.y * projectileSpeed},
                                       2.0f, 8.0f, (float)totalDamage, PURPLE, 2, explosionRadius});
            }
            break;
        }
    }
}

void updateProjectiles(std::vector<WeaponProjectile>& projectiles, std::vector<Enemy*>& enemies, float dt) {
    std::vector<WeaponProjectile> effects;

    for (int i = (int)projectiles.size() - 1; i >= 0; i--) {
        WeaponProjectile& p = projectiles[i];

        p.position.x += p.velocity.x * dt;
        p.position.y += p.velocity.y * dt;
        p.lifeTime -= dt;

        if (p.damage > 0) {
            for (Enemy* enemy : enemies) {
                if (!enemy) continue;
                if (CheckCollisionCircles(p.position, p.radius, {enemy->getX(), enemy->getY()}, 10)) {
                    enemy->takeDamage((int)p.damage);

                    if (p.type == 2) {
                        for (Enemy* splashEnemy : enemies) {
                            if (!splashEnemy) continue;
                            if (Vector2Distance(p.position, {splashEnemy->getX(), splashEnemy->getY()}) <= p.angle) {
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

        if (p.lifeTime <= 0) {
            projectiles.erase(projectiles.begin() + i);
        }
    }

    for (WeaponProjectile effect : effects) projectiles.push_back(effect);
}

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
