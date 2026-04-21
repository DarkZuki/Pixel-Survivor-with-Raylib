#pragma once
#include "../core/Entity.h"
#include "../player/Player.h"
#include "../enemy/Enemy.h"
#include "raylib.h"
#include <vector>

// Simple projectile structure
struct WeaponBullet {
    Vector2 pos;
    Vector2 vel;
    float lifetime;
    float radius;
    int damage;
    Color color;
    int type;         // 0: Normal, 1: Sword slash, 2: Explosion
    float angle;      // For sword direction or explosion radius
};

// Simple Weapon class with all weapons in one class
class Weapon {
private:
    float cooldown;      // Time between attacks
    int damage;          // Base damage
    float speed;         // Projectile speed
    float range;         // Attack range
    int weaponType;      // 0: Sword, 1: MagicWand, 2: Knife, 3: SpellBook
    float timer;         // Current cooldown timer

public:
    // Constructor - just pass the weapon type
    Weapon(int type);
    
    // Update weapon (handles cooldown and attacks)
    void update(Player& player, const std::vector<Enemy*>& enemies, 
                std::vector<WeaponBullet>& bullets, Vector2 targetPos, bool isAttacking);
    
    // Getters
    int getWeaponType() const { return weaponType; }
    const char* getName() const;
    
private:
    // Internal attack method
    void attack(Player& player, const std::vector<Enemy*>& enemies,
                std::vector<WeaponBullet>& bullets, Vector2 targetPos);
};

// Global functions for bullet management
void updateBullets(Player& player, std::vector<WeaponBullet>& bullets, 
                   std::vector<Enemy*>& enemies, float dt);
void drawBullets(const std::vector<WeaponBullet>& bullets);