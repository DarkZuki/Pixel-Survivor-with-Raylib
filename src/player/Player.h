#pragma once
#include "../core/Entity.h"
#include "raylib.h"

class Player : public Entity {
private:
    int hp = 100;
    int level = 1;
    int exp = 0;
    float speed = 3.0f;
public:
    //Constructor
    Player();
    void update() override;
    void draw() override;
    
    // Getters
    int getHp() const { return hp; }
    int getLevel() const { return level; }
    int getExp() const { return exp; }
    float getSpeed() const { return speed; }
    
    // Setters
    void setHp(int newHp) { hp = newHp; }
    void setLevel(int newLevel) { level = newLevel; }
    void setExp(int newExp) { exp = newExp; }
    void setSpeed(float newSpeed) { speed = newSpeed; }
    void addExp(int amount) { exp += amount; }
    void takeDamage(int damage) { hp -= damage; }
    
};
