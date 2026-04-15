#pragma once
#include "../core/Entity.h"
#include "../player/Player.h"
#include "raylib.h"

class Skill : public Entity {
private:
    Player* player;
    float angle = 0;
    float radius = 30;
public:
    //Constructor
    Skill(Player* p);
    void update() override;
    void draw() override;
    
    // Getters
    float getAngle() const { return angle; }
    float getRadius() const { return radius; }
    Player* getPlayer() const { return player; }
    
    // Setters
    void setAngle(float newAngle) { angle = newAngle; }
    void setRadius(float newRadius) { radius = newRadius; }
    

};
