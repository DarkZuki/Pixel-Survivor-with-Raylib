#include "Skill.h"
#include <cmath> // 

Skill::Skill(Player* p) : player(p) {
    type = SkillType::AUTO_BALLS;
    damage = 15.0f;     // damage basic
    radius = 70.0f;     // rudimentary radius for the orbiting balls
    angle = 0.0f;
    num_particles = 1;  // start with 1 ball, will increase as player gains EXP
}

void Skill::update() {
    // update position to follow the player
    x = player->getX();
    y = player->getY();
    // 1000ex[p] = +1 ball, max 5 balls at 4000exp
    num_particles = 1 + (player->getExp() / 1000);
    if (num_particles > 5) num_particles = 5;

    // Rotate the balls around the player
    angle += 2.5f * GetFrameTime(); 
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

            
            DrawCircle((int)p_x, (int)p_y, 6, GREEN); 
            DrawCircleLines((int)p_x, (int)p_y, 8, LIME);
        }
    }
}
