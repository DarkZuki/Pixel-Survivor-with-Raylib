#include "raylib.h"
#include <vector>
#include <algorithm>
#include <cmath>

#include "core/Entity.h"
#include "player/Player.h"
#include "enemy/Enemy.h"
#include "skill/Skill.h"
#include "bullet/Bullet.h"
#include "Item/Item.h"
using namespace std;

float distance(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1, dy = y2 - y1;
    return sqrt(dx*dx + dy*dy);
}

void removeEntity(vector<Entity*>& entities, Entity* entity) {
    entities.erase(remove(entities.begin(), entities.end(), entity), entities.end());
}

void removeEnemy(vector<Entity*>& entities, vector<Enemy*>& enemies, int idx) {
    removeEntity(entities, enemies[idx]);
    delete enemies[idx];
    enemies.erase(enemies.begin() + idx);
}

int main() {
    InitWindow(800, 600, "PIXEL SURVIVOR");
    SetTargetFPS(60);

    Player player;
    vector<Entity*> entities;
    vector<Enemy*> enemies;
    vector<Bullet*> bullets;
    vector<Item*> items;
    float enemyFireTimer=0; // Track cooldown for ranged enemies
    float spawnTimer = 0.0f; // Track time for spawning enemies
    float gameTimer = 0.0f; // Track total survival time
    float hpSpawnTimer = 0.0f; // Track time for spawning HP items
    //nút dừng game
    bool isPaused = false; // Trạng thái game
    Rectangle pauseButton = { 740, 10, 50, 50 }; // Vị trí nút Pause (Góc trên bên phải)
    entities.push_back(&player);
    // --- KHỞI TẠO DANH SÁCH SKILLS ---
    vector<Skill*> skills;

    // Mày muốn chơi skill nào thì push_back cái đó vào
    skills.push_back(new Skill(&player, SkillType::LASER_BEAM));
    skills.push_back(new Skill(&player, SkillType::HAMMER));
    skills.push_back(new Skill(&player, SkillType::SHIELD));
    skills.push_back(new Skill(&player, SkillType::SHURIKEN));
    skills.push_back(new Skill(&player, SkillType::THUNDER_STRIKE));

    // Thêm các skill vào danh sách entities để nó tự gọi hàm draw()
    for (auto s : skills) {
        entities.push_back(s);
    }
    
    while (!WindowShouldClose()) { 

//  Kiểm tra bấm nút Pause hoặc bấm phím ESC
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(GetMousePosition(), pauseButton)) {
                isPaused = !isPaused; // Đảo trạng thái
            }
        }
        if (IsKeyPressed(KEY_ESCAPE)) isPaused = !isPaused;

        if (!isPaused) {
                

        hpSpawnTimer += GetFrameTime();
        if (hpSpawnTimer >= 10.0f) { // Spawn HP item every 10 seconds
            float randomX = GetRandomValue(50, 750);
            float randomY = GetRandomValue(50, 550);
            Item* hpItem = new Item(randomX, randomY, 0, 1); // ID 1 for HP item
            items.push_back(hpItem);
            entities.push_back(hpItem);
            hpSpawnTimer = 0.0f;
        }
        if (player.getHp() <= 0) {
            BeginDrawing();
            ClearBackground(BLACK);
            // Format time as MM:SS
            int mins = (int)(gameTimer / 60);
            int secs = (int)(gameTimer) % 60;
            DrawText("GAME OVER", 280, 250, 40, RED);
            DrawText(TextFormat("SCORE: %d", player.getScore()), 350, 320, 20, WHITE);
            // Display survival time in MM:SS format
            DrawText(TextFormat("TIME SURVIVED: %02d:%02d", mins, secs), 285, 360, 20, WHITE);
            EndDrawing();
            continue;
        }
        gameTimer += GetFrameTime(); // Update game timer
            
        // Update
        for (auto e : entities) e->update();
        // RANGED ENEMY LOGIC (Type 3)
            enemyFireTimer += GetFrameTime(); 
        for (auto e : enemies) {
            if (e->getEnemyType() == 3) { 
                // Calculate distance between this enemy and player
                float d = distance(e->getX(), e->getY(), player.getX(), player.getY());
                if (d < 250) { 
                // If within range, stop moving and prepare to shoot
                    e->setSpeed(0); 
                    if (enemyFireTimer >= 1.5f) { 
                        float sX = (player.getX() > e->getX()) ? 15 : -15;
                        float sY = (player.getY() > e->getY()) ? 15 : -15;
                // Spawn an enemy bullet targeting the player's current position
                        Bullet* eb = new Bullet(e->getX() + sX, e->getY() + sY, player.getX(), player.getY());
                        eb->setIsEnemyBullet(true);
                        bullets.push_back(eb);
                        entities.push_back(eb);
                    }
                } else {
                // If player is far away, resume chasing
                    e->setSpeed(0.8f); 
                }
            }
        }
        // Reset shooting timer after 1.5 seconds
        if (enemyFireTimer >= 1.5f) enemyFireTimer = 0;

        // Spawn enemies
        // Spawn logic: Every second, spawn an enemy at a random angle around the player, at a fixed radius
        const float FIXEL_SPAWN_RADIUS = 400.0f;
        spawnTimer += GetFrameTime();
        if (spawnTimer >= 0.0001f) {
            float randomAngle = GetRandomValue(0, 360) * (PI / 180.0f);
            float spawnX = player.getX() + cos(randomAngle) * FIXEL_SPAWN_RADIUS;
            float spawnY = player.getY() + sin(randomAngle) * FIXEL_SPAWN_RADIUS;
        // Determine enemy type based on random value
        int r=GetRandomValue(0, 99);
        int type = 0;
        if (r < 40) {
            type = 0; // 40% chance for NORMAL
        }   
        else if (r < 70) {
            type = 2; // 30% chance for FAST
        }  
        else if(r < 90) {
            type = 1; // 20% chance for TANK
        }
        
        else {
            type = 3; // 10% chance for RANGED
        } 
            Enemy* e = new Enemy(&player, type);
            // Set spawn position based on random angle and fixed radius from player
            e->setPosition(spawnX, spawnY);
            enemies.push_back(e);
            entities.push_back(e);
            spawnTimer = 0.0f;
        }

        // Shoot bullets
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 m = GetMousePosition();
            Bullet* b = new Bullet(player.getX(), player.getY(), m.x, m.y);
            bullets.push_back(b);
            entities.push_back(b);
        }

        // Bullet-enemy collisions
        for (size_t i = 0; i < enemies.size(); i++) {
            for (size_t j = 0; j < bullets.size(); j++) {
                if (!bullets[j]->getIsEnemyBullet() && distance(bullets[j]->getX(), bullets[j]->getY(), 
                    enemies[i]->getX(), enemies[i]->getY()) < 15) {
                    
                    enemies[i]->takeDamage(20); // Chỉ trừ máu
                    bullets[j]->setX(-1000);    // Đánh dấu xóa đạn
                }
            }
        }
       
        
        
        player.update();
        for (auto s : skills) {
            s->update(); // Cập nhật vị trí, timer của từng skill
            
            // Gọi tất cả trigger, skill nào đúng Type của nó thì nó mới chạy
            s->triggerLaser(enemies);
            s->triggerThunder(enemies);
            s->triggerShieldCollision(enemies);
            s->triggerHammerCollision(enemies);
            s->triggerShurikenCollision(enemies);
        }


        // Player-enemy collisions
        for (auto enemy : enemies) {
            if (distance(player.getX(), player.getY(), 
                        enemy->getX(), enemy->getY()) < 20) {
                player.takeDamage(1);
            }
        }

        // Enemy bullet-player collisions
        for (size_t j = 0; j < bullets.size(); j++) {
            if (bullets[j]->getIsEnemyBullet() && distance(bullets[j]->getX(), bullets[j]->getY(), player.getX(), player.getY()) < 15) {
                player.takeDamage(1);
                removeEntity(entities, bullets[j]);
                bullets.erase(bullets.begin() + j);
                j--;
            }
        }
        
        // Item collection
        for (size_t k =0; k < items.size(); k++){
           float dist = distance(player.getX(), player.getY(), items[k]->getX(), items[k]->getY());
            if (dist < 20) {
                if (items[k]->getID() == 1) { // HP item
                    player.setHp(player.getHp() + 10);
                } else { // EXP item
                    int pointsEarned = items[k]->getExpValue();
                
                    // LƯU LẠI CẤP ĐỘ TRƯỚC KHI CỘNG EXP
                    int oldLevel = player.getLevel();
                    
                    player.addExp(pointsEarned); // Hàm này sẽ tự gọi levelUp() bên trong Player.cpp

                    // NẾU CẤP ĐỘ THAY ĐỔI -> NÂNG CẤP SKILL
                    if (player.getLevel() > oldLevel) {
                        TraceLog(LOG_INFO, "PLAYER LEVEL UP! Upgrading all skills...");
                        for (auto s : skills) {
                            s->levelUp(); // GỌI HÀM NÀY THÌ SKILL MỚI LÊN CẤP ĐƯỢC
                        }
                }
                }
                removeEntity(entities, items[k]);
                delete items[k];
                items.erase(items.begin() + k);
                k--;
                }
            
        }
    }
    for (int i = (int)enemies.size() - 1; i >= 0; i--) {
    if (enemies[i]->getHp() <= 0) {
        // 1. Tính toán giá trị EXP dựa trên loại quái
        int val = 10;
        int type = enemies[i]->getEnemyType();
        if (type == 1) val = 15; // FAST
        if (type == 2) val = 25; // TANK
        if (type == 3) val = 20; // RANGED

        // 2. TẠO ITEM KINH NGHIỆM RƠI RA ĐẤT
        // ID 0 là EXP, ID 1 là HP (theo file Item.cpp của mày)
        Item* crystal = new Item(enemies[i]->getX(), enemies[i]->getY(), val, 0);
        
        items.push_back(crystal);
        entities.push_back(crystal);

        // 3. Cộng điểm Score (không phải EXP) cho oai
        player.addScore(val);

        // 4. XÓA QUÁI THỰC SỰ KHỎI GAME
        removeEnemy(entities, enemies, i);
    }
}
        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
        for (auto e : entities) e->draw();
        
        DrawFPS(10, 10);
        DrawText(TextFormat("HP: %d/%d", player.getHp(), player.getMaxHp()), 10, 30, 20, WHITE);
        DrawText(TextFormat("LV: %d", player.getLevel()), 10, 55, 20, YELLOW);
        
        // Draw EXP progress bar
        int expBarWidth = 800;
        int expBarHeight = 20;
        int expBarX = 0;
        int expBarY = 580;
        float expProgress = (float)player.getExp() / player.getExpToNextLevel();
        DrawRectangle(expBarX, expBarY, expBarWidth, expBarHeight, DARKGREEN);
        DrawRectangle(expBarX, expBarY, (int)(expBarWidth * expProgress), expBarHeight, LIME);
        DrawRectangleLines(expBarX, expBarY, expBarWidth, expBarHeight, WHITE);

        DrawText(TextFormat("EXP: %d/%d", player.getExp(), player.getExpToNextLevel()), 330, 580, 20, SKYBLUE);
        
        DrawText(TextFormat("Score: %d", player.getScore()), 10, 80, 20, WHITE);
        // Format time as MM:Ss
        int mins = (int)(gameTimer / 60);
        int secs = (int)(gameTimer) % 60;
        // Display survival time in MM:SS format
        DrawText(TextFormat("Time: %02d:%02d", mins, secs), 330, 20, 25, WHITE);
      
        DrawRectangleRec(pauseButton, DARKGRAY);
        DrawText("||", pauseButton.x + 18, pauseButton.y + 10, 30, WHITE);

        // NẾU ĐANG PAUSE THÌ VẼ BẢNG MENU
        if (isPaused) {
            // Vẽ lớp nền mờ đè lên game
            DrawRectangle(0, 0, 800, 600, Fade(BLACK, 0.6f));

            // Vẽ cái bảng Menu ở giữa
            DrawRectangle(250, 150, 300, 300, RAYWHITE);
            DrawText("GAME PAUSED", 310, 180, 30, BLACK);

            // Nút RESUME
            Rectangle resumeBtn = { 300, 250, 200, 50 };
            DrawRectangleRec(resumeBtn, LIGHTGRAY);
            DrawText("RESUME", 355, 265, 20, BLACK);

            // Nút EXIT
            Rectangle exitBtn = { 300, 330, 200, 50 };
            DrawRectangleRec(exitBtn, RED);
            DrawText("EXIT", 375, 345, 20, WHITE);

            // Check click vào các nút trong Menu
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePos = GetMousePosition();
                if (CheckCollisionPointRec(mousePos, resumeBtn)) {
                    isPaused = false; // Chạy tiếp
                }
                if (CheckCollisionPointRec(mousePos, exitBtn)) {
                    break; // Thoát vòng lặp main -> Out game
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;

}
 