#pragma once
#include "raylib.h"
#include <string>

class CollisionMap {
private:
    Image collisionImage = { 0 };  // Initialize to prevent garbage data
    Color* pixelData;
    int width;
    int height;
    bool loaded;

    void unload();

public:
    CollisionMap();
    ~CollisionMap();
    CollisionMap(const CollisionMap&) = delete;
    CollisionMap& operator=(const CollisionMap&) = delete;
    CollisionMap(CollisionMap&&) = delete;
    CollisionMap& operator=(CollisionMap&&) = delete;
    
    // Load collision map from image file
    bool load(const std::string& filename);
    
    // Check if a position is walkable (returns true if walkable)
    // Uses simple threshold: dark pixels (black) are obstacles, light pixels (white) are walkable
    bool isWalkable(float worldX, float worldY, float playerRadius = 18.0f) const;
    
    // Get image dimensions
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
    // Check if collision map is loaded
    bool isLoaded() const { return loaded; }
    
    // Get pixel color at world position (for debugging)
    Color getPixelColor(float worldX, float worldY) const;
};

// Global collision map instance
extern CollisionMap gCollisionMap;

// Initialize the global collision map
void InitCollisionMap(const std::string& filename);

// Check if a world position is walkable
bool IsPositionWalkable(float x, float y, float playerRadius = 18.0f);
