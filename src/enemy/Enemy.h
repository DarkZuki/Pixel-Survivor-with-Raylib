#pragma once
#include "../core/Entity.h"
#include "../player/Player.h"
#include "raylib.h"

class Enemy : public Entity {
private:
    Player* player;
    int hp;
    float speed;
public:
    //Constructor
    Enemy(Player* p);
    void update() override;
    void draw() override;
    
    // Getters
    int getHp() const { return hp; }
    float getSpeed() const { return speed; }
    Player* getPlayer() const { return player; }
    
    // Setters
    void setHp(int newHp) { hp = newHp; }
    void setSpeed(float newSpeed) { speed = newSpeed; }
    void takeDamage(int damage) { hp -= damage; }
    
};
