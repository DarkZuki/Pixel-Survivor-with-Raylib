#include "Entity.h"
#include <algorithm>
#include <cmath>

float distance(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

void removeEntity(std::vector<Entity*>& entities, Entity* entity) {
    entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
}
