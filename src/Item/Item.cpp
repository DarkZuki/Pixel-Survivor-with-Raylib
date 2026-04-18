#include "Item.h"

// Constructor
Item::Item(float ix, float iy, int val) :  expValue(val), timer(0.0f), duration(15.0f) {
    this->x = ix;
    this->y = iy;
    // Assign color based on EXP value
    if (expValue <= 10) {
        color = RED;    // Common gems
    } else if (expValue <= 20) {
        color = YELLOW; // Rare gems
    } else {
        color = GREEN;  // Epic gems
    }
}

// Update 
void Item::update() {
    timer += GetFrameTime(); // Precise time tracking
}

// Draw
void Item::draw() {
    // 1. Calculate blink logic for the last 3 seconds
    bool shouldDraw = true;
    if (timer >= duration - 3.0f) {
        if ((int)(timer * 10) % 2 != 0) {
            shouldDraw = false; // Toggle visibility for blinking effect
        }
    }

    // 2. Draw both circle and outline only if it's in a "visible" blink state
    if (shouldDraw) {
        DrawCircle(x, y, 4, color);        // Solid core (Reduced size to 4)
        DrawCircleLines(x, y, 4, WHITE);   // Consistent outline
    }
}