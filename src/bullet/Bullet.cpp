#include "Bullet.h"
#include <cmath>

Bullet::Bullet(float sx, float sy, float tx, float ty) {
    x = sx;
    y = sy;
    
    float dx = tx - sx;
    float dy = ty - sy;
    float dist = sqrt(dx*dx + dy*dy);
    
    if (dist > 0) {
        vx = (dx / dist) * speed;
        vy = (dy / dist) * speed;
    } else {
        vx = vy = 0;
    }
}

void Bullet::update() {
    x += vx;
    y += vy;
}

void Bullet::draw() {
    DrawCircle(x, y, 3, YELLOW);
}
