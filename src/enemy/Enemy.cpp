#include "Enemy.h"
#include <cmath>

Enemy::Enemy(Player* p, int type) : player(p), enemyType(type) {
    x = GetRandomValue(240, 1680);
    y = GetRandomValue(180, 900);
    // Set hp and speed based on enemy type
    if (type == 0) { // NORMAL
        hp = 30;
        speed = 1.0f;
    } else if (type == 1) { // FAST
        hp = 15;
        speed = 2.0f;
    } else if (type == 2) { // TANK
        hp = 100;
        speed = 0.5f;
    } else if (type == 3) { // RANGED
        hp = 30;
        speed = 0.5f;
    }
}

void Enemy::update() {
    // Move towards player
    float dx = player->getX() - x;
    float dy = player->getY() - y;
    float dist = sqrt(dx*dx + dy*dy);
    
    if (dist > 0) {
        x += (dx / dist) * speed;
        y += (dy / dist) * speed;
    }
}

void Enemy::draw() {
    // Set color based on enemy type
    Color enemyColor = RED;
    if (enemyType == 1) enemyColor = ORANGE;
    else if (enemyType == 2) enemyColor = BROWN;
    else if (enemyType == 3) enemyColor = GREEN;
    DrawCircle(x, y, 14, enemyColor);
    DrawText(TextFormat("HP: %d", hp), x - 27, y - 36, 14, WHITE);
}

void removeEnemy(std::vector<Entity*>& entities, std::vector<Enemy*>& enemies, int idx) {
    removeEntity(entities, enemies[idx]);
    delete enemies[idx];
    enemies.erase(enemies.begin() + idx);
}
