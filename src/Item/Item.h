#pragma once
#include "../core/Entity.h"
#include "raylib.h"

class Item : public Entity {
private:
    int expValue;       // Amount of EXP this gem gives
    float timer;        // Current time the gem has existed
    float duration;     // Max lifetime (15.0f seconds)
    Color color;        // To distinguish different EXP values visually

public:
    // Constructor: Takes position and the EXP value it holds
    Item(float ix, float iy, int val);

    // Override base class methods
    void update() override;
    void draw() override;

    // Getters (Standard practice in your project)
    int getExpValue() const { return expValue; }
    bool isExpired() const { return timer >= duration; }
    
    // Setters (In case you want to modify gems later)
    void setExpValue(int val) { expValue = val; }
    void setDuration(float d) { duration = d; }
};