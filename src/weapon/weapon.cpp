#include "weapon.h"
#include "raylib.h"
#include <cmath>
#include <algorithm>

// Helper to normalize a vector
static Vector2 normalize(Vector2 v) {
    float len = sqrtf(v.x * v.x + v.y * v.y);
    if (len == 0) return {0, 0};
    return {v.x / len, v.y / len};
}

// Helper to calculate distance between two points
static float Vector2Dist(Vector2 a, Vector2 b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return sqrtf(dx * dx + dy * dy);
}

// Weapon constructor - sets stats based on weapon type
Weapon::Weapon(int type) : weaponType(type), timer(0) {
    // Set default stats for each weapon type
    switch (type) {
        case 0: // Sword
            damage = 15;
            cooldown = 0.5f;
            speed = 0;
            range = 80;
            break;
        case 1: // Magic Wand
            damage = 8;
            cooldown = 0.8f;
            speed = 300;
            range = 400;
            break;
        case 2: // Knife
            damage = 6;
            cooldown = 0.3f;
            speed = 500;
            range = 300;
            break;
        case 3: // Spell Book
            damage = 20;
            cooldown = 1.0f;
            speed = 400;
            range = 500;
            break;
    }
}

// Get weapon name
const char* Weapon::getName() const {
    switch (weaponType) {
        case 0: return "Sword";
        case 1: return "Magic Wand";
        case 2: return "Knife";
        case 3: return "Spell Book";
        default: return "Unknown";
    }
}

// Update weapon - handles cooldown and attacks
void Weapon::update(Player& player, const std::vector<Enemy*>& enemies,
                    std::vector<WeaponBullet>& bullets, Vector2 targetPos, bool isAttacking) {
    timer += GetFrameTime();
    if (isAttacking && timer >= cooldown) {
        timer = 0;
        attack(player, enemies, bullets, targetPos);
    }
}

// Internal attack method - handles different weapon behaviors
void Weapon::attack(Player& player, const std::vector<Enemy*>& enemies,
                    std::vector<WeaponBullet>& bullets, Vector2 targetPos) {
    Vector2 playerPos = {player.getX(), player.getY()};
    
    switch (weaponType) {
        case 0: { // Sword - 45 degree cone attack
            Vector2 toTarget = normalize({targetPos.x - playerPos.x, targetPos.y - playerPos.y});
            float coneThreshold = 0.924f; // cos(22.5 degrees)
            
            for (Enemy* enemy : enemies) {
                Vector2 enemyPos = {enemy->getX(), enemy->getY()};
                float dist = Vector2Dist(playerPos, enemyPos);
                
                if (dist <= range) {
                    Vector2 toEnemy = normalize({enemyPos.x - playerPos.x, enemyPos.y - playerPos.y});
                    float dot = toTarget.x * toEnemy.x + toTarget.y * toEnemy.y;
                    
                    if (dot >= coneThreshold) {
                        enemy->takeDamage(damage + player.getDamage());
                    }
                }
            }
            
            // Visual effect for sword slash
            float attackAngle = atan2(toTarget.y, toTarget.x);
            bullets.push_back({playerPos, {0,0}, 0.1f, range, 0, YELLOW, 1, attackAngle});
            break;
        }
        
        case 1: { // Magic Wand - auto-target nearest enemy
            Enemy* nearest = nullptr;
            float minDist = range;
            
            for (Enemy* enemy : enemies) {
                float d = Vector2Dist(playerPos, {enemy->getX(), enemy->getY()});
                if (d < minDist) {
                    minDist = d;
                    nearest = enemy;
                }
            }
            
            if (nearest) {
                Vector2 dir = normalize({nearest->getX() - playerPos.x, nearest->getY() - playerPos.y});
                bullets.push_back({playerPos, {dir.x * speed, dir.y * speed}, 2.0f, 6.0f, 
                                  damage + player.getDamage(), PURPLE, 0, 0});
            }
            break;
        }
        
        case 2: { // Knife - shoots straight toward target
            Vector2 dir = normalize({targetPos.x - playerPos.x, targetPos.y - playerPos.y});
            bullets.push_back({playerPos, {dir.x * speed, dir.y * speed}, 1.0f, 4.0f, 
                              damage + player.getDamage(), SKYBLUE, 0, 0});
            break;
        }
        
        case 3: { // Spell Book - explosive projectile
            Vector2 dir = normalize({targetPos.x - playerPos.x, targetPos.y - playerPos.y});
            bullets.push_back({playerPos, {dir.x * speed, dir.y * speed}, 2.0f, 8.0f, 
                              damage + player.getDamage(), PURPLE, 2, 100.0f}); // type=2 for explosion, angle=explosion radius
            break;
        }
    }
}

// Update all active bullets
void updateBullets(Player& player, std::vector<WeaponBullet>& bullets, 
                   std::vector<Enemy*>& enemies, float dt) {
    for (int i = (int)bullets.size() - 1; i >= 0; i--) {
        bullets[i].pos.x += bullets[i].vel.x * dt;
        bullets[i].pos.y += bullets[i].vel.y * dt;
        bullets[i].lifetime -= dt;
        
        // Collision check with enemies
        for (int j = (int)enemies.size() - 1; j >= 0; j--) {
            if (bullets[i].damage > 0 && 
                CheckCollisionCircles(bullets[i].pos, bullets[i].radius, 
                                     {enemies[j]->getX(), enemies[j]->getY()}, 10)) {
                
                enemies[j]->takeDamage(bullets[i].damage);
                
                // Handle explosion (type 2)
                if (bullets[i].type == 2) {
                    float explosionRadius = bullets[i].angle;
                    
                    // Damage all enemies in explosion radius
                    for (int k = 0; k < (int)enemies.size(); k++) {
                        if (k != j) {
                            Vector2 enemyPos = {enemies[k]->getX(), enemies[k]->getY()};
                            float distToExplosion = Vector2Dist(bullets[i].pos, enemyPos);
                            
                            if (distToExplosion <= explosionRadius) {
                                enemies[k]->takeDamage(bullets[i].damage / 2);
                            }
                        }
                    }
                    
                    // Create explosion visual
                    bullets.push_back({bullets[i].pos, {0,0}, 0.3f, 0, 0, ORANGE, 2, explosionRadius});
                }
                
                bullets[i].lifetime = 0; // Destroy bullet on hit
                break;
            }
        }
        
        if (bullets[i].lifetime <= 0) {
            bullets.erase(bullets.begin() + i);
        }
    }
}

// Draw all bullets
void drawBullets(const std::vector<WeaponBullet>& bullets) {
    for (const auto& b : bullets) {
        // Sword slash visual (type 1)
        if (b.type == 1) {
            float angleDeg = b.angle * (180.0f / 3.14159265f);
            float startAngle = angleDeg - 22.5f;
            float endAngle = angleDeg + 22.5f;
            
            DrawCircleSector(b.pos, b.radius, startAngle, endAngle, 8, Fade(YELLOW, b.lifetime * 10));
            DrawCircleSectorLines(b.pos, b.radius, startAngle, endAngle, 8, Fade(WHITE, b.lifetime * 10));
        }
        // Explosion visual (type 2, damage=0 means it's the visual effect)
        else if (b.type == 2 && b.damage == 0) {
            DrawCircleLines(b.pos.x, b.pos.y, b.angle, Fade(ORANGE, b.lifetime * 5));
            DrawCircleV(b.pos, b.angle * 0.3f, Fade(ORANGE, b.lifetime * 3));
        }
        // Normal projectiles
        else {
            DrawCircleV(b.pos, b.radius, b.color);
        }
    }
}