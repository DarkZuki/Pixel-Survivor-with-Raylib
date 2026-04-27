#ifndef WEAPON_H
#define WEAPON_H

#include "raylib.h"
#include "../player/Player.h"
#include "../enemy/Enemy.h"
#include <vector>

struct WeaponStats {
    int damage;
    float cooldown;
    float range;
    float speed;
    int count;
    float explosionRadius;
    bool doubleHit;
};

// Tạo struct với các thuộc tính của vật thể bay
struct WeaponProjectile {
    Vector2 position;
    Vector2 velocity;
    float lifeTime;
    float radius;
    float damage;
    Color color;
    int type;       // 0 = ko có gì, 1 = hiệu ứng búa đập, 2 = đạn nổ
    float angle;    // Bán kính của vụ nổ khi = 2
};

// Tạo class vũ khí với các chỉ số 
class Weapon {
private:
    int weaponType;
    int weaponLevel;
    WeaponStats stats;
    float currentCooldownTimer;

    void updateWeaponStats();
public:
    // Các hàm constructor và getter, setter
    Weapon(int type);
    const char* getName() const;
    int getLevel() const;
    void setLevel(int newLevel);

    // Hàm tấn công và cập nhật
    void attack(Player& player, const std::vector<Enemy*>& enemies,
                std::vector<WeaponProjectile>& projectiles, Vector2 targetPosition);
    void update(Player& player, const std::vector<Enemy*>& enemies,
                std::vector<WeaponProjectile>& projectiles, Vector2 targetPosition, bool isAttacking);
};

// Các hàm liên quan tới đạn
void updateProjectiles(std::vector<WeaponProjectile>& projectiles, std::vector<Enemy*>& enemies, float deltaTime);
void drawProjectiles(const std::vector<WeaponProjectile>& projectiles);

#endif // WEAPON_H
