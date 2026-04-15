#include "Player.h"

Player::Player() {
    x = 400;
    y = 300;
    hp = 100;
    level = 1;
    exp = 0;
}

void Player::update() {
    if (IsKeyDown(KEY_W)) y -= speed;
    if (IsKeyDown(KEY_S)) y += speed;
    if (IsKeyDown(KEY_A)) x -= speed;
    if (IsKeyDown(KEY_D)) x += speed;
}

void Player::draw() {
    DrawCircle(x, y, 10, BLUE);
    DrawText(TextFormat("HP: %d", hp), x - 20, y - 30, 10, WHITE);
}
