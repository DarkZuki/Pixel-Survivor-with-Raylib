#include "Enemy.h"
#include <cmath>

Enemy::Enemy(Player* p) : player(p), hp(30), speed(1.0f) {
    x = GetRandomValue(100, 700);
    y = GetRandomValue(100, 500);
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
    DrawCircle(x, y, 8, RED);
    DrawText(TextFormat("HP: %d", hp), x - 15, y - 20, 8, WHITE);
}
