#include "Enemy.h"
#include <cmath>

Enemy::Enemy(Player* p, int type, Texture2D* tex) : player(p), enemyType(type), texture(tex) {
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
        rotation = atan2f(dx, dy) * (180.0f / PI);
    }
}

void Enemy::draw() {
    if (texture !=nullptr){
        float targetSize = 32.0f;
        float scale = targetSize / texture-> width;
        Vector2 pos =  { (float)x - (targetSize)/2, (float)y - (targetSize) / 2 };
        DrawTextureEx(*texture, pos, rotation, scale,  WHITE);

    }
    DrawText(TextFormat("HP: %d", hp), x - 15, y - 20, 8, WHITE);
}
// can chinh lai hit box sao cho no bao boc quai
// chinh lai cai goc xoay tu dong, nhin no hia cot qua 