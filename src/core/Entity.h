#pragma once
#include <vector>

class Entity {
protected:
    float x, y;
public:
    //Constructor
    Entity() : x(0), y(0) {}
    
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual ~Entity() {}
    
    // Getters
    float getX() const { return x; }
    float getY() const { return y; }
    
    // Setters
    void setX(float newX) { x = newX; }
    void setY(float newY) { y = newY; }
    void setPosition(float newX, float newY) { x = newX; y = newY; }

};

float distance(float x1, float y1, float x2, float y2);
void removeEntity(std::vector<Entity*>& entities, Entity* entity);
