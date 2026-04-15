# PIXEL SURVIVOR - Naming Convention Guide

## 📋 Team Standard
- **Classes**: `PascalCase` (e.g., `Player`, `Enemy`)
- **Methods/Functions**: `camelCase` (e.g., `update()`, `checkCollision()`)
- **Variables/Members**: `camelCase` (e.g., `playerHealth`, `enemySpeed`)
- **Constants**: `UPPER_SNAKE_CASE` (e.g., `MAX_ENEMIES`, `PLAYER_HP`)
- **Enums**: `PascalCase` for name, `UPPER_SNAKE_CASE` for values

---

## 🎮 Core Classes

### Base Class
- **`Entity`** - Abstract base class
  - Members: `x`, `y`, `speed`, `radius`
  - Methods: `update()`, `draw()`, `getPosition()`, `setPosition()`

### Player
- **`Player`** - Extends Entity
  - Members:
    - `hp` / `maxHp`
    - `damage`
    - `attackCooldown`
    - `attackTimer`
    - `moveSpeed`
    - `xp` / `maxXp`
    - `level`
    - `score`
    - `isAlive`
  - Methods:
    - `move(dx, dy)`
    - `takeDamage(amount)`
    - `heal(amount)`
    - `attack()` / `shootBullets()`
    - `collectXp(amount)`
    - `levelUp()`
    - `applyUpgrade(upgradeType)`
    - `update()`
    - `draw()`
    - `getRect()`

### Enemy
- **`Enemy`** - Extends Entity
  - Members:
    - `hp` / `maxHp`
    - `damage`
    - `enemyType` (NORMAL, FAST, TANK)
    - `isAlive`
  - Methods:
    - `chase(player)`
    - `update()`
    - `draw()`
    - `takeDamage(amount)`
    - `getRect()`
    - `getEnemyType()`

### Bullet
- **`Bullet`** - Extends Entity
  - Members:
    - `dx` / `directionX`
    - `dy` / `directionY`
    - `damage`
    - `lifetime`
    - `isActive`
  - Methods:
    - `update()`
    - `draw()`
    - `getRect()`
    - `isOutOfBounds()`

### Item/Drop
- **`Item`** - For collectible items
  - Members:
    - `itemType` (HEALTH, SPEED_BOOST, etc.)
    - `x`, `y`
    - `radius`
    - `value`
    - `effectDuration`
  - Methods:
    - `update()`
    - `draw()`
    - `applyEffect(player)`
    - `getRect()`

### XP Drop
- **`XpDrop`** - Drops when enemy dies
  - Members:
    - `x`, `y`
    - `xpValue`
    - `moveSpeed`
    - `targetX`, `targetY`
  - Methods:
    - `moveTowards(target)`
    - `update()`
    - `draw()`

### Upgrade System
- **`UpgradeOption`** - Single upgrade choice
  - Members:
    - `upgradeId`
    - `title` (e.g., "Speed Boost")
    - `description`
    - `upgradeType`
    - `value` (percentage or amount)
  - Methods:
    - `getTitle()`
    - `getDescription()`

- **`LevelUpScreen`** - Level up UI
  - Members:
    - `options` (vector of UpgradeOption)
    - `selectedIndex`
  - Methods:
    - `generateUprades()`
    - `selectOption(index)`
    - `draw()`
    - `handleInput()`

### Game Manager
- **`Game`** - Main game controller (CRITICAL)
  - Members:
    - `player`
    - `enemies` (vector<Enemy>)
    - `bullets` (vector<Bullet>)
    - `items` (vector<Item>)
    - `xpDrops` (vector<XpDrop>)
    - `gameState` (MENU, PLAYING, LEVEL_UP, GAME_OVER)
    - `currentTime`
    - `elapsedTime`
    - `difficulty` (EASY, NORMAL, HARD)
    - `spawnRate`
    - `spawnTimer`
    - `highScore`
    - `isRunning`
  - Methods:
    - `update()`
    - `render()`
    - `handleInput(key)`
    - `spawnEnemy()`
    - `updateEnemySpawnRate()`
    - `checkCollisions()` / `updateCollisions()`
    - `checkBulletEnemyCollision()`
    - `checkEnemyPlayerCollision()`
    - `checkXpPlayerCollision()`
    - `checkItemPlayerCollision()`
    - `updateScore()`
    - `gameOver()`
    - `getCurrentScore()`
    - `getElapsedTime()`
    - `reset()`

### Camera
- **`Camera`** - Follows player
  - Members:
    - `targetX`, `targetY`
    - `x`, `y`
    - `screenWidth`, `screenHeight`
  - Methods:
    - `update(playerX, playerY)`
    - `getOffsetX()`, `getOffsetY()`

### High Score
- **`HighScoreManager`** (or `ScoreManager`)
  - Members:
    - `highScore`
    - `highScoreTime`
    - `scoreFilePath`
  - Methods:
    - `loadHighScore()`
    - `saveHighScore(score, time)`
    - `getHighScore()`
    - `isNewHighScore(score)`

### Difficulty
- **`DifficultySettings`** (or keep as constants/enum)
  - Members:
    - `spawnRateMultiplier`
    - `enemySpeedMultiplier`
    - `enemyHealthMultiplier`
    - `difficultyLevel`
  - Methods:
    - `getSettings(difficulty)`
    - `applyDifficulty(game)`

---

## 📊 Enums

```cpp
enum class EnemyType {
    NORMAL,      // Regular enemy
    FAST,        // Fast enemy
    TANK         // Tank/Heavy enemy
};

enum class ItemType {
    HEALTH_POTION,
    SPEED_BOOST,
    DAMAGE_BOOST,
    ATTACK_SPEED_BOOST
};

enum class UpgradeType {
    SPEED_UP,
    DAMAGE_UP,
    ATTACK_SPEED_UP,
    BULLET_COUNT,
    HP_UP,
    XP_MULTIPLIER
};

enum class GameState {
    MENU,
    PLAYING,
    LEVEL_UP,
    GAME_OVER,
    PAUSED
};

enum class DifficultyMode {
    EASY,
    NORMAL,
    HARD
};

enum class BulletPattern {
    STRAIGHT,
    SPREAD,
    CIRCLE
};
```

---

## 🔧 Global Constants

```cpp
// Screen
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int FPS = 60;

// Player
const int PLAYER_INITIAL_HP = 100;
const int PLAYER_MAX_HP = 200;
const float PLAYER_BASE_SPEED = 200.0f;
const float PLAYER_COLLISION_RADIUS = 20.0f;
const int PLAYER_BASE_DAMAGE = 10;

// Enemy
const float ENEMY_NORMAL_SPEED = 100.0f;
const float ENEMY_FAST_SPEED = 150.0f;
const float ENEMY_TANK_SPEED = 70.0f;

const int ENEMY_NORMAL_HP = 30;
const int ENEMY_FAST_HP = 20;
const int ENEMY_TANK_HP = 70;

const int ENEMY_NORMAL_DAMAGE = 5;
const int ENEMY_FAST_DAMAGE = 5;
const int ENEMY_TANK_DAMAGE = 10;

const float ENEMY_COLLISION_RADIUS = 15.0f;

// Bullet
const float BULLET_SPEED = 350.0f;
const float BULLET_COLLISION_RADIUS = 5.0f;
const int BULLET_BASE_DAMAGE = 10;

// Attack
const float ATTACK_COOLDOWN_INITIAL = 0.8f;
const float ATTACK_COOLDOWN_MIN = 0.2f;
const int BULLETS_PER_SHOT_INITIAL = 1;
const int BULLETS_PER_SHOT_MAX = 5;

// XP System
const int XP_ENEMY_NORMAL = 5;
const int XP_ENEMY_FAST = 7;
const int XP_ENEMY_TANK = 10;

const std::vector<int> LEVEL_UP_XP_REQUIRED = {20, 30, 45, 60, 80, 100, 130, 160, 200};

// Spawn
const float SPAWN_RATE_INITIAL = 2.0f; // 1 enemy per 2 seconds
const int MAX_ENEMIES = 100;
const int MAX_BULLETS = 50;
const int MAX_ITEMS = 30;

// Score
const int SCORE_ENEMY_NORMAL = 10;
const int SCORE_ENEMY_FAST = 15;
const int SCORE_ENEMY_TANK = 20;
const int SCORE_PER_SECOND = 1;

// Difficulty
const float DIFFICULTY_EASY_SPAWN_MULTIPLIER = 0.7f;
const float DIFFICULTY_EASY_SPEED_MULTIPLIER = 0.8f;
const float DIFFICULTY_HARD_SPAWN_MULTIPLIER = 1.3f;
const float DIFFICULTY_HARD_SPEED_MULTIPLIER = 1.2f;

// Item
const float ITEM_DROP_CHANCE = 0.2f; // 20%
const int ITEM_HEALTH_VALUE = 20;
const float ITEM_SPEED_BOOST_DURATION = 3.0f;
const float ITEM_COLLISION_RADIUS = 10.0f;

// Camera
const float CAMERA_SMOOTH_SPEED = 0.15f;
```

---

## 🗂️ File Structure (Recommended)

```
src/
├── core/
│   └── Entity.h
├── player/
│   ├── Player.h
│   └── Player.cpp
├── enemy/
│   ├── Enemy.h
│   └── Enemy.cpp
│   └── EnemyType.h (if separate)
├── bullet/
│   ├── Bullet.h
│   └── Bullet.cpp
├── item/
│   ├── Item.h
│   ├── Item.cpp
│   ├── XpDrop.h
│   └── XpDrop.cpp
├── ui/
│   ├── LevelUpScreen.h
│   ├── LevelUpScreen.cpp
│   ├── Camera.h
│   └── Camera.cpp
├── game/
│   ├── Game.h
│   ├── Game.cpp
│   ├── GameState.h
│   └── Constants.h
├── manager/
│   ├── HighScoreManager.h
│   ├── HighScoreManager.cpp
│   ├── DifficultySettings.h
│   └── DifficultySettings.cpp
├── utils/
│   ├── Math.h
│   └── Collision.h
└── main.cpp
```

---

## 📝 Function Naming Patterns

### Getters
```cpp
int getHealth() const;
float getSpeed() const;
EnemyType getEnemyType() const;
int getScore() const;
float getElapsedTime() const;
```

### Setters
```cpp
void setHealth(int hp);
void setSpeed(float speed);
void setDifficulty(DifficultyMode difficulty);
```

### Checkers
```cpp
bool isAlive() const;
bool isOutOfBounds() const;
bool canAttack() const;
bool hasLeveledUp() const;
bool isColliding(Entity* other) const;
```

### Updaters
```cpp
void updatePosition(float deltaTime);
void updateCollisions();
void updateScore();
void updateSpawnRate();
void updateDifficulty();
```

### Handlers
```cpp
void handleInput(int key);
void handleLevelUp();
void handleGameOver();
void handleCollision(Entity* other);
```

### Calculators
```cpp
float calculateDistance(float x1, float y1, float x2, float y2);
int calculateDamage(int baseDamage, float modifier);
int calculateXpNeeded(int level);
```

---

## 🎯 Additional Classes You'll Need

| Class | Purpose |
|-------|---------|
| `Collision` | Static collision detection utilities |
| `Math` | Utility math functions (distance, angle, etc.) |
| `InputHandler` | Centralized input management |
| `AnimationManager` | Handle sprite animations (future) |
| `ParticleEffect` | Death particles, hit effects |
| `WaveManager` | Manage enemy waves by time |
| `UpgradeFactory` | Generate random upgrades |

---

## ✅ Checklist for Team Members

When creating new classes/methods, ensure:
- [ ] Class names use `PascalCase`
- [ ] Method/function names use `camelCase`
- [ ] Member variables use `camelCase` with prefixes if needed
- [ ] Constants use `UPPER_SNAKE_CASE`
- [ ] Enum names use `PascalCase`, values use `UPPER_SNAKE_CASE`
- [ ] Methods have clear verb names (`get`, `set`, `check`, `update`, `handle`, etc.)
- [ ] Related classes grouped in same folders
- [ ] Comments explain non-obvious behavior
- [ ] Follow header/implementation separation (`.h` and `.cpp`)

---

## 🚀 Quick Reference Commands

```cpp
// Example: Creating a new enemy
Enemy enemy(x, y, EnemyType::NORMAL);
enemy.takeDamage(10);

// Example: Player attacking
player.attack();

// Example: Checking collision
if(collision.isCircleOverlap(player, enemy)) {
    // Handle collision
}

// Example: Level up
if(player.hasLeveledUp()) {
    levelUpScreen.generateUpgrades();
}
```

---

**Last Updated**: April 14, 2026
**Document Version**: 1.0
