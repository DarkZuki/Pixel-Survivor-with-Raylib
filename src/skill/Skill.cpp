
#include "Skill.h"
#include <cmath>
#include "../enemy/Enemy.h"
#include <algorithm>
#include "raymath.h"


Skill::Skill(Player* p, SkillType skillType) : player(p), type(skillType) {
    x = p->getX();
    y = p->getY();
    level = 1;
    
    // Load Textures (Giữ nguyên assets của mày)
    shurikenTexture = LoadTexture("Graphics/shuriken.png");
    thunderTexture = LoadTexture("Graphics/thunderdungdung.png");
    shieldTexture = LoadTexture("Graphics/khiendoitruongmy.png");
    hammerTexture = LoadTexture("Graphics/buathor.png");
    axeTexture = LoadTexture("Graphics/riuthor.png");

    // Khởi tạo các timer cơ bản
    is_laser_active = false;
    laser_timer = 0.0f;
    laser_cooldown_timer = 0.0f;
    thunder_timer = 0.0f;
    shield_timer = 0.0f;
    hammer_timer = 0.0f;
    angle = 0.0f;
    selfRotation = 0.0f;

    // Cập nhật chỉ số lần đầu tiên
    updateSkillStats();
}

Skill::~Skill() {
    UnloadTexture(shurikenTexture);
    UnloadTexture(thunderTexture);
    UnloadTexture(shieldTexture);
    UnloadTexture(hammerTexture);
    UnloadTexture(axeTexture);
}

void Skill::levelUp() {
    if (level < 10) {
        level++;
        updateSkillStats();
    }
}

void Skill::updateSkillStats() {
    // Reset chỉ số cơ bản trước khi áp dụng bonus từ level
    damage = 10.0f;
    radius = 50.0f;
    num_particles = 1;
    laser_length = 400.0f;
    laser_width = 15.0f;
    cooldown = 5.0f;
    is_dual_laser = false;
    is_hammer_riu = false;
    num_shields = 1;
    thunder_chain_count = 0;
    thunder_cooldown = 5.0f;

    switch (type) {
        case SkillType::LASER_BEAM:
            switch (level) {
                case 10: is_dual_laser = true;
                case 9:  cooldown *= 0.7f;
                case 8:  laser_length += 300.0f;
                case 7:  damage += 30.0f;
                case 6:  laser_length += 200.0f;
                case 5:  laser_width += 10.0f;
                case 4:  cooldown *= 0.8f;
                case 3:  laser_length += 100.0f;
                case 2:  damage += 20.0f;
                case 1:  damage += 5.0f;
                    break;
            }
            break;

        case SkillType::HAMMER:
            damage = 20.0f;        // Búa cơ bản dame to hơn bi xoay
            switch (level) {
                case 10: is_hammer_riu = true; damage += 50.0f;
                case 9:  damage += 20.0f;
                case 8:  cooldown *= 0.7f;
                case 7:  damage += 15.0f;
                case 6:  damage += 10.0f;
                case 5:  cooldown *= 0.8f;
                case 4:  damage += 10.0f;
                case 3:  cooldown *= 0.9f;
                case 2:  damage += 5.0f;
                case 1:  damage += 5.0f;
                    break;
            }
            break;

        case SkillType::SHIELD:
            // Chỉ số mặc định Level 0
            damage = 15.0f;
            cooldown = 7.0f;
            radius = 20.0f; // Bán kính va chạm của khiên
            this->num_shields = 1;
            switch (level) {
                case 10: num_shields += 1;
                case 9:  damage += 20.0f;
                case 8:  cooldown *= 0.7f;
                case 7:  radius += 10.0f;
                case 6:  num_shields += 1;
                case 5:  cooldown *= 0.8f;
                case 4:  radius += 5.0f;
                case 3:  num_shields += 1;
                case 2:  cooldown *= 0.9f;
                case 1:  damage += 5.0f;
                    break;
            }
            break;

        case SkillType::THUNDER_STRIKE:
            // Chỉ số mặc định Level 0
            damage = 30.0f;
           
            switch (level) {
                case 10: this->thunder_chain_count = 3;
                        this->num_particles = 2;
                case 9:  thunder_cooldown *= 0.6f;
                case 8:  damage += 40.0f;
                case 7:  thunder_cooldown *= 0.7f;
                case 6:  damage += 30.0f;
                case 5:  thunder_cooldown *= 0.8f;
                case 4:  damage += 20.0f;
                case 3:  thunder_cooldown *= 0.9f;
                case 2:  damage += 10.0f;
                case 1:  damage += 10.0f;
                    break;
            }
            break;

        case SkillType::SHURIKEN:
        case SkillType::AUTO_BALLS:
            damage = 10.0f;
            num_particles = 1; 
            radius = 50.0f;   // Khoảng cách xoay quanh người

            switch (level) {
                case 10:
                    num_particles += 2; // Cấp cuối thêm hẳn 2 cái cho máu
                case 9:
                    damage += 15.0f;
                case 8:
                    radius += 20.0f;    // Xoay vòng rộng hơn
                case 7:
                    num_particles += 1;
                case 6:
                    damage += 10.0f;
                case 5:
                    num_particles += 1;
                case 4:
                    damage += 10.0f;
                case 3:
                    radius += 10.0f;
                case 2:
                    num_particles += 1;
                case 1:
                    damage += 5.0f;
                    break;
                default:
                    break;
            }
            break;
}
}
// --- HÀM PHỤ TRỢ ---
Enemy* Skill::findNearestEnemy(const std::vector<Enemy*>& enemies) {
    Enemy* nearest = nullptr;
    float minD = 9999.0f;
    for (auto e : enemies) {
        float d = Vector2Distance({player->getX(), player->getY()}, {e->getX(), e->getY()});
        if (d < minD) { minD = d; nearest = e; }
    }
    return nearest;
}

void Skill::update() {
    float dt = GetFrameTime();
    x = player->getX();
    y = player->getY();

    angle += 2.5f * dt;
    selfRotation += 15.0f * dt;

    if (is_laser_active) {
        laser_timer -= dt;
        if (laser_timer <= 0) is_laser_active = false;
    }
    if (laser_cooldown_timer > 0) laser_cooldown_timer -= dt;
    thunder_timer += dt; // QUAN TRỌNG: Cần tăng timer để sét đánh
    shield_timer += dt;
    hammer_timer += dt;
    if (shield_timer >= cooldown) {
    // Chạy vòng lặp tạo khiên dựa trên số lượng đã tính ở updateSkillStats
    for (int i = 0; i < this->num_shields; i++) {
        float randomAngle = GetRandomValue(0, 360) * (PI / 180.0f);
        float speedVal = (level >= 5) ? 400.0f : 350.0f;
        
        activeShields.push_back({
            {x, y}, 
            {(float)cos(randomAngle) * speedVal, (float)sin(randomAngle) * speedVal}, 
            0, true, radius, 0.0f
        });
    }
    shield_timer = 0.0f;
    }
    for (auto& s : activeShields) {
        s.pos = Vector2Add(s.pos, Vector2Scale(s.speed, dt));
        s.rotation += 500.0f * dt;
    }
    for (auto& h : activeHammers) {
        h.pos = Vector2Add(h.pos, Vector2Scale(h.speed, dt));
    }
}

// Các hàm Trigger sát thương
void Skill::triggerLaser(std::vector<Enemy*>& enemies) {
    if (is_laser_active || laser_cooldown_timer > 0 || enemies.empty()) return;
    Enemy* target = findNearestEnemy(enemies);
    if (target) {
        laser_direction = Vector2Normalize(Vector2Subtract({target->getX(), target->getY()}, {x, y}));
        is_laser_active = true;
        laser_timer = 0.2f;
        laser_cooldown_timer = 5.0f; // Sẽ scale theo level sau
    }
}

void Skill::triggerThunder(std::vector<Enemy*>& enemies) {
    if (enemies.empty() || thunder_timer < thunder_cooldown) return;

    std::vector<Enemy*> hitList; // Danh sách các con đã bị sét đánh để không nảy trùng

    // 1. Số tia sét đánh từ trời xuống (1 hoặc 2 tia tùy level)
    for (int i = 0; i < this->num_particles; i++) {
        if (enemies.empty()) break;

        // Chọn quái ngẫu nhiên chưa bị trúng trong đợt này
        int idx = GetRandomValue(0, (int)enemies.size() - 1);
        Enemy* firstTarget = enemies[idx];
        
        firstTarget->takeDamage((int)damage);
        hitList.push_back(firstTarget);

        // Vẽ tia sét từ trời xuống
        DrawTexturePro(thunderTexture, { 0, 0, (float)thunderTexture.width, (float)thunderTexture.height }, 
                       { firstTarget->getX() - 15, firstTarget->getY() - 120, 30, 120 }, { 0, 0 }, 0, WHITE);

        // 2. Nếu đạt Level 10 thì kích hoạt Sét Lan từ mỗi tia sét chính
        if (this->thunder_chain_count > 0) {
            Enemy* lastTarget = firstTarget;

            for (int j = 0; j < this->thunder_chain_count; j++) {
                Enemy* nextTarget = nullptr;
                float minDistance = 150.0f; // Bán kính nảy sét

                for (auto e : enemies) {
                    // Kiểm tra xem quái đã có trong danh sách trúng đòn chưa
                    bool alreadyHit = false;
                    for (auto hit : hitList) if (e == hit) alreadyHit = true;
                    if (alreadyHit) continue;

                    float d = Vector2Distance({lastTarget->getX(), lastTarget->getY()}, {e->getX(), e->getY()});
                    if (d < minDistance) {
                        minDistance = d;
                        nextTarget = e;
                    }
                }

                if (nextTarget) {
                    nextTarget->takeDamage((int)damage * 0.7f); // Sét nảy gây 70% dame
                    
                    // Vẽ tia điện nối giữa 2 con quái
                    DrawLineEx({lastTarget->getX(), lastTarget->getY()}, {nextTarget->getX(), nextTarget->getY()}, 3, YELLOW);
                    
                    hitList.push_back(nextTarget);
                    lastTarget = nextTarget;
                } else break;
            }
        }
    }

    thunder_timer = 0.0f;
}

void Skill::triggerShieldCollision(std::vector<Enemy*>& enemies) { 
    for (auto& s : activeShields) {
        if (!s.active) continue;

        for (auto e : enemies) {
            // Kiểm tra va chạm vòng tròn giữa Khiên và Quái
            if (CheckCollisionCircles(s.pos, s.radius, {e->getX(), e->getY()}, 15)) {
                
                // Gây sát thương
                e->takeDamage((int)damage);

                // Logic nảy ngược lại của mày
                s.speed.x *= -1.0f;
                s.speed.y *= -1.0f;
                s.bounces++;

                // Số lần nảy tối đa tăng theo level
                int maxB = (level >= 5) ? 6 : 3;
                if (s.bounces >= maxB) s.active = false;

                break; // Một khiên chỉ chạm 1 con mỗi lần nảy
            }
        }
    }
    // Dọn dẹp khiên đã hết số lần nảy
    activeShields.erase(std::remove_if(activeShields.begin(), activeShields.end(),
        [](const Shield& s) { return !s.active; }), activeShields.end());
}

void Skill::triggerHammerCollision(std::vector<Enemy*>& enemies) {
     for (auto& h : activeHammers) {
        if (!h.active) continue;

        for (auto e : enemies) {
            // Tính khoảng cách bình phương cho nhanh (giống code cũ của mày)
            float dx = h.pos.x - e->getX();
            float dy = h.pos.y - e->getY();
            float distSq = dx * dx + dy * dy;

            // Hitbox búa to hơn bi xoay (40.0f)
            if (distSq < 40.0f * 40.0f) {
                // Búa nện đau hơn, và nếu là Rìu (lv10) thì dame cực to
                float hammerDamage = h.isRiu ? damage * 5 : damage * 2;
                e->takeDamage((int)hammerDamage);
                
                // Lưu ý: Búa bay xuyên thấu nên không set h.active = false ở đây
            }
        }
    }
}

void Skill::draw() {
    if (type == SkillType::AUTO_BALLS) {
        for (int i = 0; i < num_particles; i++) {
            float p_angle = angle + (i * 2.0f * PI / num_particles);
            float p_x = x + cos(p_angle) * radius;
            float p_y = y + sin(p_angle) * radius;
            if (shurikenTexture.id != 0) {
                DrawTexturePro(shurikenTexture, { 0, 0, (float)shurikenTexture.width, (float)shurikenTexture.height },
                    { p_x, p_y, 24, 24 }, { 12, 12 }, selfRotation * 57.29f, WHITE);
            }
        }
    }
   if (is_laser_active) {
        // Tia chính (hướng về phía quái)
        Vector2 endPos = Vector2Add({x, y}, Vector2Scale(laser_direction, laser_length));
        DrawLineEx({x, y}, endPos, laser_width, SKYBLUE);
        DrawLineEx({x, y}, endPos, laser_width / 3.0f, WHITE); // Lõi trắng

        // Nếu đạt Level 10: Vẽ thêm tia đối nghịch
        if (is_dual_laser) {
            // Đảo ngược hướng bằng cách nhân với -1
            Vector2 reverseDir = Vector2Scale(laser_direction, -1.0f);
            Vector2 endPosReverse = Vector2Add({x, y}, Vector2Scale(reverseDir, laser_length));
            
            DrawLineEx({x, y}, endPosReverse, laser_width, SKYBLUE);
            DrawLineEx({x, y}, endPosReverse, laser_width / 3.0f, WHITE);
        }
    }
    for (auto& s : activeShields) {
        if (s.active && shieldTexture.id > 0) {
            float drawSize = s.radius * 3.0f;
            DrawTexturePro(shieldTexture, {0, 0, (float)shieldTexture.width, (float)shieldTexture.height},
                {s.pos.x, s.pos.y, drawSize, drawSize}, {drawSize / 2.0f, drawSize / 2.0f}, s.rotation, WHITE);
        }
    }
    for (auto& h : activeHammers) {
        if (h.active) {
            Texture2D currentTex = h.isRiu ? axeTexture : hammerTexture;
            float dw = h.isRiu ? 60.0f : 40.0f;
            float dh = dw * ((float)currentTex.height / currentTex.width);
            DrawTexturePro(currentTex, { 0, 0, (float)currentTex.width, (float)currentTex.height },
                { h.pos.x, h.pos.y, dw, dh }, { dw / 2, dh / 2 }, (h.rotation * 57.29f) + 90.0f, WHITE);
        }
    }
}