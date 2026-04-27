#include "CollisionMap.h"
#include <cstdio>

CollisionMap::CollisionMap() : pixelData(nullptr), width(0), height(0), loaded(false) {
}

void CollisionMap::unload() {
    if (collisionImage.data != nullptr) {
        UnloadImage(collisionImage);
    }

    collisionImage = { 0 };
    pixelData = nullptr;
    width = 0;
    height = 0;
    loaded = false;
}

CollisionMap::~CollisionMap() {
    unload();
}

bool CollisionMap::load(const std::string& filename) {
    if (collisionImage.data != nullptr) {
        unload();
    }

    collisionImage = LoadImage(filename.c_str());
    if (collisionImage.data == nullptr) {
        collisionImage = { 0 };
        pixelData = nullptr;
        width = 0;
        height = 0;
        loaded = false;
        TraceLog(LOG_ERROR, "CollisionMap: Failed to load image: %s", filename.c_str());
        return false;
    }

    // Convert to RGBA format for easy pixel access
    ImageFormat(&collisionImage, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    width = collisionImage.width;
    height = collisionImage.height;
    pixelData = (Color*)collisionImage.data;
    loaded = true;

    TraceLog(LOG_INFO, "CollisionMap: Loaded %s (%dx%d)", filename.c_str(), width, height);
    return true;
}

bool CollisionMap::isWalkable(float worldX, float worldY, float playerRadius) const {
    if (!loaded || pixelData == nullptr) {
        // If no collision map loaded, allow movement everywhere
        return true;
    }
    
    // Check multiple points around the player to prevent crossing borders
    // We check the center and 4 points around the player's radius
    const int checkPoints = 5;
    const float checkOffsets[5][2] = {
        {0.0f, 0.0f},   // Center
        {playerRadius, 0.0f},   // Right
        {-playerRadius, 0.0f},  // Left
        {0.0f, playerRadius},   // Down
        {0.0f, -playerRadius}   // Up
    };
    
    for (int i = 0; i < checkPoints; i++) {
        float checkX = worldX + checkOffsets[i][0];
        float checkY = worldY + checkOffsets[i][1];
        
        // Clamp to image bounds
        int pixelX = (int)checkX;
        int pixelY = (int)checkY;
        
        if (pixelX < 0 || pixelX >= width || pixelY < 0 || pixelY >= height) {
            // Out of bounds - treat as obstacle
            return false;
        }
        
        // Get pixel color
        int index = pixelY * width + pixelX;
        Color pixel = pixelData[index];
        
        // Calculate brightness (simple average of RGB)
        float brightness = (pixel.r + pixel.g + pixel.b) / 3.0f;
        
        // If pixel is dark (black or near black), it's an obstacle
        // Threshold: if brightness < 128, consider it an obstacle
        if (brightness < 128) {
            return false;
        }
    }
    
    return true;
}

Color CollisionMap::getPixelColor(float worldX, float worldY) const {
    if (!loaded || pixelData == nullptr) {
        return BLANK;
    }
    
    int pixelX = (int)worldX;
    int pixelY = (int)worldY;
    
    if (pixelX < 0 || pixelX >= width || pixelY < 0 || pixelY >= height) {
        return BLANK;
    }
    
    int index = pixelY * width + pixelX;
    return pixelData[index];
}

// Global collision map instance
CollisionMap gCollisionMap;

void InitCollisionMap(const std::string& filename) {
    gCollisionMap.load(filename);
}

bool IsPositionWalkable(float x, float y, float playerRadius) {
    return gCollisionMap.isWalkable(x, y, playerRadius);
}
