#include "Enemy.h"
#include "raymath.h"

Enemy::Enemy(Player* p, int type, Texture2D* tex) : player(p), texture(tex), enemyType(type) {
    currentFrame = 0;
    frameTimer = 0.0f;
    frameSpeed = 0.1f; // Mặc định 0.1s chuyển 1 frame
    frameCount = 1;    // Mặc định là 1 (nếu chưa set)
    // Set hp and speed based on enemy type
    if (type == 0) { // NORMAL
        hp = 30;
        speed = 1.0f;
        frameCount = 8;
    } else if (type == 1) { // FAST
        hp = 15;
        speed = 2.0f;
        frameCount = 4;
    } else if (type == 2) { // TANK
        hp = 100;
        speed = 0.5f;
        frameCount = 6;
    } else if (type == 3) { // RANGED
        hp = 30;
        speed = 0.5f;
        stoppingDistance = (float)GetRandomValue(200,300);
        frameCount = 5;
    } else {
        stoppingDistance = 0.0f;
    }
}

void Enemy::update() {
    // Move towards player
    Vector2 direction = Vector2Subtract({player->getX(), player->getY()}, {x, y});
    float dist = Vector2Length(direction);
    
    if (dist > 0.1f) {
        // tạo logic dùng lại để bắn của quái RANGED
        if (enemyType == 3 && dist < stoppingDistance){
            // đã vào tầm bắn , tự động  nạp đạn
            fireTimer +=GetFrameTime();
        } else {
        Vector2 step = Vector2Scale(Vector2Normalize(direction), speed);
        x += step.x;
        y += step.y;
        fireTimer = 0.0f;
        }
        if (frameCount > 1) {
        frameTimer += GetFrameTime();
        if (frameTimer >= frameSpeed) {
            frameTimer = 0.0f;
            currentFrame = (currentFrame + 1) % frameCount;
           }
        }
        // cập nhật góc quay
        rotation = (direction.x < 0) ? -1.0f : 1.0f;
    }
}
// kiem tra dieu kien ra dan cua quai RANGED
bool Enemy::canShoot(){
    if (enemyType == 3 && fireTimer >=1.5){
        fireTimer = 0.0f; // Reset lại sau khi xác nhận bắn
        return true;
    }
    return false;
}

float Enemy::getVisualYOffset() const {
    if (enemyType == 0) return 52.0f;
    if (enemyType == 1) return 36.0f;
    if (enemyType == 2) return 78.0f;
    if (enemyType == 3) return 48.0f;
    return 0.0f;
}

void Enemy::draw() {
    if (texture !=nullptr){
        float frameWidth = (float)texture->width / frameCount;
        float renderWidth = 0.0f;
        float renderHeight = 0.0f;
        float renderYOffset = getVisualYOffset();
        
        if (enemyType == 0) {         // Quái NORMAL
            renderWidth = 120.0f;
            renderHeight = 235.0f;
        } 
        else if (enemyType == 1) {    // Quái FAST
            renderWidth = 110.0f;
            renderHeight = 145.0f;
        } 
        else if (enemyType == 2) {    // Quái TANK
            renderWidth = 140.0f;
            renderHeight = 345.0f;    
        } 
        else if (enemyType == 3) {    // Quái RANGED
            renderWidth = 120.0f;
            renderHeight = 220.0f;
        }
        // tạo cấu hình vùng ảnh
        Rectangle source = { frameWidth* 4.0f * currentFrame, 0.0f, frameWidth * rotation, (float)texture->height };
        // tạo cấu hình vùng va chạm
        Rectangle dest = { x, y - renderYOffset, renderWidth, renderHeight };
        // thiết lập điểm gốc (tâm hình chữ nhật) để tính góc xoay từ tâm
        Vector2 origin = { renderWidth / 2.0f, renderHeight / 2.0f };
        // Vẽ
        DrawTexturePro(*texture, source, dest, origin, 0.0f, WHITE);
    }
    DrawText(TextFormat("HP: %d", hp), x - 28, y - getVisualYOffset() - 30, 16, WHITE);
}

void removeEnemy(std::vector<Entity*>& entities, std::vector<Enemy*>& enemies, int idx) {
    removeEntity(entities, enemies[idx]);
    delete enemies[idx];
    enemies.erase(enemies.begin() + idx);
}
