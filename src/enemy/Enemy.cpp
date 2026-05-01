#include "Enemy.h"
#include <cmath>

Enemy::Enemy(Player* p, int type, Texture2D* tex) : player(p), texture(tex), enemyType(type) {
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
        stoppingDistance = (float)GetRandomValue(200,300);
    } else {
        stoppingDistance = 0.0f;
    }
}

void Enemy::update(const std::vector<Enemy*>& allEnemies) {
    // Move towards player
    float dx = player->getX() - x;
    float dy = player->getY() - y;
    float dist = sqrt(dx*dx + dy*dy);
    float moveX =0;
    float moveY = 0;
    
    if (dist > 0.1f) {
        // tạo logic dùng lại để bắn của quái RANGED
        if (enemyType == 3 && dist < stoppingDistance){
            // đã vào tầm bắn , tự động  nạp đạn
            fireTimer +=GetFrameTime();
        } else {
        moveX += (dx / dist) * speed;
        moveY += (dy / dist) * speed;
        fireTimer = 0.0f;
        }
        // cập nhật góc quay
        rotation = (dx < 0) ? -1.0f : 1.0f;
    }
    if (!allEnemies.empty()) {
        float sepX = 0, sepY = 0;
        // khoang cach gioi han da duoc cai dat
        float space = 55.0f;
        for (auto other : allEnemies) {
            if (other == this) continue;
            float diffX = x - other->getX();
            float diffY = y - other->getY();
            float distSq = diffX*diffX + diffY*diffY;
            if (distSq > 0 && distSq < space * space) {
                float d = sqrt(distSq);
                sepX += (diffX / d) * (space - d) * 0.1f;
                sepY += (diffY / d) * (space - d) * 0.1f;
            }
        }
        moveX += sepX;
        moveY += sepY;
    }

    // 3. Cập nhật vị trí cuối cùng
    x += moveX;
    y += moveY;
}
void Enemy::update(){
    std::vector<Enemy*> emptyList;
    update(emptyList);
}
// kiem tra dieu kien ra dan cua quai RANGED
bool Enemy::canShoot(){
    if (enemyType == 3 && fireTimer >=1.5){
        fireTimer = 0.0f; // Reset lại sau khi xác nhận bắn
        return true;
    }
    return false;
}

void Enemy::draw() {
    if (texture !=nullptr){
        float targetSize = 70.0f;
        // tạo cấu hình vùng ảnh
        Rectangle source = { 0.0f, 0.0f, (float)texture->width* rotation, (float)texture->height };
        // tạo cấu hình vùng va chạm
        Rectangle dest = { x, y, targetSize, targetSize };
        // thiết lập điểm gốc (tâm hình chữ nhật) để tính góc xoay từ tâm
        Vector2 origin = { targetSize / 2.0f, targetSize / 2.0f };
        // Vẽ
        DrawTexturePro(*texture, source, dest, origin, 0.0f, WHITE);
    }
    DrawText(TextFormat("HP: %d", hp), x - 28, y - 38, 16, WHITE);
}

void removeEnemy(std::vector<Entity*>& entities, std::vector<Enemy*>& enemies, int idx) {
    removeEntity(entities, enemies[idx]);
    delete enemies[idx];
    enemies.erase(enemies.begin() + idx);
}
