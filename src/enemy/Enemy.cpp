#include "Enemy.h"
#include "raymath.h"

Enemy::Enemy(Player* p, int type, Texture2D* tex) : player(p), texture(tex), enemyType(type) {
    walkFrame = 0; 
    walkTimer = 0.0f;
    isWalking = false;
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

void Enemy::update() {
    // Move towards player
    Vector2 direction = Vector2Subtract({player->getX(), player->getY()}, {x, y});
    float dist = Vector2Length(direction);
    
    if (dist > 0.1f) {
        // tạo logic dùng lại để bắn của quái RANGED
        if (enemyType == 3 && dist < stoppingDistance){
            isWalking = false;
            // đã vào tầm bắn , tự động  nạp đạn
            fireTimer +=GetFrameTime();
        } else {
        isWalking = true;
        Vector2 step = Vector2Scale(Vector2Normalize(direction), speed);
        x += step.x;
        y += step.y;
        fireTimer = 0.0f;
        }
        // 3. Logic Animation RIÊNG cho quái NORMAL (loại 0)
        if (enemyType == 0 && isWalking) {
            walkTimer += GetFrameTime(); // Cộng dồn thời gian thực trôi qua
            
            // Nếu thời gian tích lũy vượt ngưỡng (ví dụ 0.12s giống Player)
            if (walkTimer >= 0.12f) {
                walkTimer = 0.0f; // Reset bộ đếm
                walkFrame++;      // Chuyển sang khung hình tiếp theo
                
                // Giả sử tấm ảnh NORMAL của bạn có 8 khung hình
                if (walkFrame >= 8) {
                    walkFrame = 0; // Quay lại khung hình đầu tiên (vòng lặp)
                }
            }
        } else if (enemyType == 0 && !isWalking) {
            // Nếu quái Normal đứng yên (ít xảy ra), reset về khung đầu
            walkFrame = 0;
            walkTimer = 0.0f;
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

void Enemy::draw() {
    if (texture !=nullptr){
        float targetSize = 170.0f;
        // Mặc định cho các loại quái khác (vẽ nguyên tấm ảnh)
        float frameWidth = (float)texture->width;
        float frameHeight = (float)texture->height;
        int currentFrame = 0;

        // XỬ LÝ RIÊNG CHO QUÁI NORMAL (Dựa trên file Normalrun.jpg có 8 khung hình)
        if (enemyType == 0) {
            frameWidth = (float)texture->width / 8.0f; // Chia chiều ngang ảnh làm 8 phần
            currentFrame = walkFrame; // Lấy khung hình hiện tại từ update()[cite: 1]
        }
        // tạo cấu hình vùng ảnh
        Rectangle source = { frameWidth*currentFrame, 0.0f, frameWidth* rotation, frameHeight };
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
