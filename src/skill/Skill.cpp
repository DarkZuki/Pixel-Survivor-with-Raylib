#include "Skill.h"
#include <cmath>

Skill::Skill(Player* p) : player(p), angle(0), radius(30) {
    x = p->getX();
    y = p->getY();
}

void Skill::update() {
    angle += 0.05f;
    x = player->getX() + cos(angle) * radius;
    y = player->getY() + sin(angle) * radius;
}

void Skill::draw() {
    DrawCircle(x, y, 5, GREEN);
}
