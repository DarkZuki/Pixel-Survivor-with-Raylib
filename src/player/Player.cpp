#include "Player.h"
#include "raylib.h"
#include <cmath>
#include "../core/CollisionMap.h"

Player::Player() {
    x = 960;
    y = 520;
    hp = 100;
    maxHp = 100;
    level = 1;
    exp = 0;
    score = 0;
    speed = 3.0f;
    damage = 10;
    
    // Initialize camera to follow the player
    camera.target = (Vector2){ x, y };
    camera.offset = (Vector2){ 960, 520 }; // Center of 1920x1040 screen
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

int Player::getExpToNextLevel() const {
    // Simple formula: level * 100 EXP needed for next level
    return level * 100;
}

void Player::addExp(int amount) {
    exp += amount;
    // Check for level up after adding EXP
    while (checkLevelUp()) {
        // Keep checking in case we gained enough EXP for multiple levels
    }
}

bool Player::checkLevelUp() {
    int expNeeded = getExpToNextLevel();
    if (exp >= expNeeded) {
        exp -= expNeeded; // Consume the EXP needed for level up
        levelUp();
        return true;
    }
    return false;
}

void Player::levelUp() {
    level++;
    
    // Increase stats on level up
    maxHp += 20;           // +20 max HP per level
    hp = maxHp;            // Fully heal on level up
    speed += 0.2f;         // +0.2 speed per level
    damage += 5;           // +5 damage per level
}

void Player::update() {
    // Movement with collision detection
    Vector2 moveDir = {0, 0};
    Vector2 newPos = {x, y};
    
    if (IsKeyDown(KEY_W)) { newPos.y -= speed; moveDir.y = -1; }
    if (IsKeyDown(KEY_S)) { newPos.y += speed; moveDir.y = 1; }
    if (IsKeyDown(KEY_A)) { newPos.x -= speed; moveDir.x = -1; }
    if (IsKeyDown(KEY_D)) { newPos.x += speed; moveDir.x = 1; }
    
    // Check collision before moving
    // Try moving in both X and Y directions
    if (IsPositionWalkable(newPos.x, newPos.y, 18.0f)) {
        x = newPos.x;
        y = newPos.y;
    } else {
        // If can't move diagonally, try moving in each direction separately
        bool canMoveX = IsPositionWalkable(newPos.x, y, 18.0f);
        bool canMoveY = IsPositionWalkable(x, newPos.y, 18.0f);
        
        if (canMoveX && canMoveY) {
            // Can move in either direction individually, try both
            x = newPos.x;
            y = newPos.y;
        } else if (canMoveX) {
            x = newPos.x;
        } else if (canMoveY) {
            y = newPos.y;
        }
        // If neither direction works, don't move (blocked by obstacle)
    }
    
    // Update camera to follow player
    camera.target = (Vector2){ x, y };
    
    // Update facing direction
    if (moveDir.x != 0 || moveDir.y != 0) {
        float len = sqrtf(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
        if (len > 0) {
            facingDir.x = moveDir.x / len;
            facingDir.y = moveDir.y / len;
        }
    }
}

void Player::draw() {
    DrawCircle(x, y, 18, BLUE);
    DrawText(TextFormat("HP: %d/%d", hp, maxHp), x - 45, y - 54, 18, WHITE);
    DrawText(TextFormat("LV: %d", level), x - 27, y - 76, 18, YELLOW);
}
