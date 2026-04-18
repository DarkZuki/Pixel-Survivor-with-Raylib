#include "Skill.h"
#include <cmath> // 

static constexpr float LASER_MAX_CD = 5.0f;

Skill::Skill(Player* p) : player(p) {
    type = SkillType::AUTO_BALLS;
    damage = 10.0f;     // damage basic
    radius = 50.0f;     // rudimentary radius for the orbiting balls
    angle = 0.0f;
    num_particles = 1;  // start with 1 ball, will increase as player gains EXP
    selfRotation=0.0f;
    shurikenTexture=LoadTexture("./shuriken.png");
    //laser
    is_laser_active = false;
    laser_timer=0.0f;
    laser_length=400.0f;
    laser_cooldown=0.0f;
}

void Skill::activateLaser(Vector2 mousePos) {
    // Chỉ kích hoạt nếu tia laser cũ đã hết và đã hết thời gian hồi chiêu (5s)
    if (!is_laser_active && laser_cooldown <= 0) {
        is_laser_active = true;
        laser_timer = 0.2f;   // Laser tồn tại 0.2s
        laser_cooldown = LASER_MAX_CD; // Reset cooldown về 5 giây
        
        // Tính toán hướng bắn
        float dx = mousePos.x - x;
        float dy = mousePos.y - y;
        float mag = sqrt(dx*dx + dy*dy);
        laser_direction = { dx / mag, dy / mag };
        // Nếu đang trong thời gian hồi chiêu, có thể hiển thị thông báo hoặc hiệu ứng nào đó
        // Ví dụ: DrawText("Laser on cooldown!", 10, 10, 20, RED);      
    }
}
void Skill::update() {
    // 1. Giữ nguyên logic bám theo người chơi và Auto_ball
    x = player->getX();
    y = player->getY();
    num_particles = 1 + (player->getExp() / 1000);
    angle += 2.5f * GetFrameTime();
    selfRotation += 15.0f * GetFrameTime(); // Tốc độ tự quay của skill

    // 2. Cập nhật thời gian tồn tại của Laser (nếu đang bắn)
    if (is_laser_active) {
        laser_timer -= GetFrameTime();
        if (laser_timer <= 0) is_laser_active = false;
    }

    // 3. Giảm thời gian hồi chiêu (Cooldown) mỗi khung hình
    if (laser_cooldown > 0) {
        laser_cooldown -= GetFrameTime();
    }
}

void Skill::draw() {
    if (type == SkillType::AUTO_BALLS) {
        for (int i = 0; i < num_particles; i++) {
            // 
            float p_angle = angle + (i * 2.0f * PI / num_particles);
            //Divide evenly 360 degrees by num_particles to get the angle for each ball
            // Calculate the position of each ball based on the angle and radius
            float p_x = x + cos(p_angle) * radius;
            float p_y = y + sin(p_angle) * radius;
            //check texture load
            if (shurikenTexture.id == 0) {
                DrawCircle((int)p_x, (int)p_y, 5, RED); // Fallback: draw a red circle if texture fails to load
            } else {
                // Draw the shuriken texture at the calculated position with rotation
                DrawTexturePro(shurikenTexture, 
                    { 0, 0, (float)shurikenTexture.width, (float)shurikenTexture.height },
                    { p_x, p_y, 24, 24 }, 
                    { 12, 12 }, 
                    selfRotation * 57.29f, // Chuyển Radian sang Độ
                    WHITE);
                }
        }
            //draw laser
            if(is_laser_active){
                DrawLineEx({x, y}, {x + laser_direction.x * laser_length, y + laser_direction.y * laser_length}, 15, SKYBLUE);
                DrawLineEx({x, y}, {x + laser_direction.x * laser_length, y + laser_direction.y * laser_length}, 5, WHITE); // Hiệu ứng lõi trắng

            }
        }
    }

