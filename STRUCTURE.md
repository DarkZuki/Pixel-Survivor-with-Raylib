# Game Foundation Structure

A clean, organized Raylib C++ game framework with modular architecture.

## Project Structure

```
├── main.cpp              # Main game loop (root level)
├── Makefile              # Build configuration
├── README.md
└── src/
    ├── core/            # Core base classes
    │   └── Entity.h     # Base entity class
    ├── player/          # Player module
    │   ├── Player.h
    │   └── Player.cpp
    ├── enemy/           # Enemy module
    │   ├── Enemy.h
    │   └── Enemy.cpp
    ├── bullet/          # Bullet/Projectile module
    │   ├── Bullet.h
    │   └── Bullet.cpp
    └── skill/           # Skill/Effect module
        ├── Skill.h
        └── Skill.cpp
```

## Building

### Debug Build
```bash
mingw32-make build debug
```

### Release Build
```bash
mingw32-make build release
```

### Clean
```bash
mingw32-make clean
```

## Controls

- **WASD** - Move player
- **Mouse Click** - Shoot bullets at mouse position
- **Enemies** - Automatically spawn and move toward player
- **Collision** - Bullets destroy enemies, enemies damage player

## Adding New Features

Each module is self-contained. To add new features:

1. Create a new folder in `src/` (e.g., `src/powerup/`)
2. Add `.h` header file with class definition inheriting from `Entity`
3. Add `.cpp` implementation file
4. Include the header in `main.cpp`
5. Add instances to the entities vector

Example:
```cpp
#pragma once
#include "../core/Entity.h"

class PowerUp : public Entity {
public:
    PowerUp();
    void update() override;
    void draw() override;
};
```

## Architecture Notes

- All game objects inherit from `Entity` base class
- Each system (player, enemy, bullet, skill) is independent
- Main game loop handles updates, collisions, and rendering
- Easy to extend with new entity types
- Simple and maintainable code structure
