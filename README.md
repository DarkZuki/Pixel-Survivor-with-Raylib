# PIXEL SURVIVOR - Year 2 University Project

A 2D survivor game with **skills, weapons, leveling, enemy types, rarity system, difficulty modes, and a menu** - built by 4 electronics/telecom students in 3 weeks.

**Learning**: Basic OOP (inheritance, polymorphism, enums) + Simple DSA (vectors, arrays)

---

## 🎮 Game Features

### ⚔️ Skills System (2 types, max 4 active)
- **Auto Aura**: Automatically attacks around player
- **Key Activated**: Triggers on key press (1-4 keys)
- **Leveling**: Skills can level up (1-4 max)
- **Effects**: Damage, speed, cooldown, projectile count

### 🔫 Weapons System
- **4+ Weapons**: Gun, Sword, Laser, Shotgun
- **Rarity Drops**: Common, Rare, Epic (affects damage/firerate)
- **Weapon Switching**: Press E to switch
- **Different Mechanics**: Each weapon fires differently

### 👾 Enemy System
- **4 Enemy Types**: Normal, Fast, Tank, Ranged
- **Stat Variation**: Different HP, speed, behavior
- **Rarity Drops**: Drop better loot when rare/epic
- **Visual Difference**: Size/color indicates type

### 🌟 Rarity System
- **Common**: Normal stats, white color
- **Rare**: +50% stats, blue color
- **Epic**: +100% stats, purple color
- Applies to: Weapons, Skills, Enemies

### 📊 Leveling & Progression
- **Skill Levels**: 1-4 (player can choose which to level)
- **Player HP**: Increases with level
- **Damage Growth**: +5% per level
- **Stat Increases**: Level up through enemy kills

### ⚙️ Difficulty System (3 preset modes)
- **Easy**: Slow spawn, weak enemies, more player HP
- **Normal**: Balanced challenge
- **Hard**: Fast spawn, strong enemies, less player HP
- Set from **main menu** before starting

### 📱 Main Menu
- **Start Game**: Launch with selected difficulty
- **Settings**: Choose difficulty (Easy/Normal/Hard)
- **Clean UI**: Simple text buttons

### 🎮 Core Mechanics
- Player movement (WASD) + aiming
- Click to fire main weapon
- Skill keys (1-4) to activate skills
- Press E to switch weapons
- Dodge incoming enemies
- Survive as long as possible!

---

## 📁 Code Structure

```
src/
├── main.cpp              # Game loop + state management
├── core/
│   └── Entity.h          # Base class for all game objects
├── player/
│   ├── Player.h/.cpp     # Player class
│   └── Skill.h/.cpp      # NEW: Skill system
├── weapon/
│   └── Weapon.h/.cpp     # NEW: Weapon system
├── enemy/
│   ├── Enemy.h/.cpp      # Modified: Enemy types
│   └── EnemyType.h       # NEW: Enums for types
├── ui/
│   ├── Menu.h/.cpp       # NEW: Main menu
│   └── HUD.h/.cpp        # NEW: Game UI display
├── bullet/
│   └── Bullet.h/.cpp     # Bullet projectiles
└── skill/
    └── Skill.h/.cpp      # (also used in player folder)
```

---

## 🚀 How to Build & Run

```bash
# Build debug version
Ctrl+Shift+P → Run Task → "build debug"

# Or press F5 to build and run

# Build release version  
Ctrl+Shift+P → Run Task → "build release"
```

---

## 📋 Project Timeline (3 Weeks)

### Week 1: Core Systems (24-40 hours)
- [ ] Skill class with 2 types & leveling
- [ ] Weapon class with rarity
- [ ] 4 enemy types implementation
- [ ] Main menu setup

### Week 2: Game Mechanics (24-40 hours)
- [ ] Skill level-up system
- [ ] Weapon switching & drops
- [ ] 3 difficulty levels
- [ ] Player HP/stat growth

### Week 3: Polish (24-40 hours)
- [ ] Testing & bug fixes
- [ ] Balancing skills/weapons/enemies
- [ ] Visual feedback & UI
- [ ] Code documentation

**Total**: ~75 hours for 4 people ÷ 4 = ~19 hours per person (spread over 3 weeks ✅)

See `TASK_BREAKDOWN.md` for detailed task distribution.

---

## 🎓 Learning Concepts

### Object-Oriented Programming ✅
- **Classes & Inheritance**: `Entity` → `Player`, `Enemy`, `Skill`, `Weapon`
- **Polymorphism**: Virtual `update()` and `draw()` methods
- **Encapsulation**: Private data, public getters/setters
- **Enums**: `SkillType`, `WeaponType`, `EnemyType`, `Rarity`, `Difficulty`
- **Composition**: Player HAS skills, HAS weapon, HAS stats

### Data Structures & Algorithms ✅
- **Arrays**: `skills[4]` for max 4 skills
- **Vectors**: Dynamic storage of enemies, bullets, weapons
- **Enums**: Discrete type values
- **Structs**: `DifficultyStats` grouping related data
- **Dynamic Memory**: `new`/`delete` for game objects
- **Collision Detection**: Circle-based collision algorithm

---

## 👥 Team Roles

| Person | Week 1 | Week 2 | Week 3 |
|--------|--------|--------|--------|
| A | Skill class | Skill leveling | Testing |
| B | Weapon class | Weapon switching | Balancing |
| C | Enemy types | Difficulty levels | UI/Visual |
| D | Main menu | Player HP system | Documentation |

**All work in parallel** - tasks are independent!

---

## 💻 Tech Stack

- **Language**: C++17
- **Graphics**: Raylib 5.0
- **Compiler**: MinGW (GCC)
- **IDE**: Visual Studio Code
- **OS**: Windows 10/11

---

## 📊 Code Metrics

| Metric | Target | Status |
|--------|--------|--------|
| Total Lines | < 1000 | ✅ |
| New Classes | 6-8 | ✅ |
| Inheritance Depth | 2 levels | ✅ |
| Vector Usage | Yes | ✅ |
| Enum Usage | Yes | ✅ |
| Comments | Basic but clear | ✅ |

---

## 🎮 Game Balance

### Skills
```
Level 1: Base damage = 10
Level 2: 10 * 1.2 = 12 damage
Level 3: 10 * 1.4 = 14 damage
Level 4: 10 * 1.6 = 16 damage

Rarity multipliers:
Common = 1.0x, Rare = 1.5x, Epic = 2.0x
```

### Weapons
```
Gun (balanced)
- Damage: 15, FireRate: 0.5 sec

Sword (fast)
- Damage: 20, FireRate: 0.2 sec

Shotgun (powerful)
- Damage: 40 (5 bullets), FireRate: 1.0 sec

Laser (precise)
- Damage: 25, FireRate: 0.3 sec
```

### Enemies
```
Normal: HP 30, Speed 1.0
Fast:   HP 15, Speed 2.0
Tank:   HP 100, Speed 0.5
Ranged: HP 40, Speed 1.5
```

---

## 🔧 Building & Debugging

**Build Commands**:
```
make RAYLIB_PATH=C:/raylib/raylib PROJECT_NAME=main OBJS=src/*.cpp BUILD_MODE=DEBUG
```

**Debugging Tips**:
- Press Ctrl+Shift+D in VS Code to attach debugger
- Set breakpoints by clicking line numbers
- Check console output for errors

---

## 📝 Code Quality

- ✅ Classes properly encapsulated
- ✅ Virtual functions used correctly
- ✅ Vectors managed with smart memory allocation
- ✅ Collision detection working
- ✅ No memory leaks (delete all new'd objects)
- ✅ Code commented and organized

---

## 🎯 Success Criteria

For your teacher:
- ✅ Game runs without crashes (5-10 min gameplay)
- ✅ All 3 difficulties working
- ✅ Skills, weapons, enemies all implement
- ✅ Menu system working
- ✅ Code shows OOP understanding (inheritance, polymorphism)
- ✅ Code shows DSA understanding (vectors, enums, dynamic memory)
- ✅ Clean, documented code
- ✅ Balanced and fun gameplay

---

## 💡 Pro Tips

1. **Test constantly** - Don't wait until week 3
2. **Keep it simple** - Resist feature creep
3. **Balance matters** - Playtest and adjust numbers
4. **Comment as you go** - Don't leave for week 3
5. **Use git branches** - One per task, merge when done
6. **Help each other** - Team of 4 is your strength!
7. **Version control** - Commit every 30 minutes

---

## 🚀 Getting Started

1. Read `TASK_BREAKDOWN.md` for detailed task descriptions
2. Each person picks a Week 1 task
3. Follow the code examples provided
4. Keep classes under 150 lines each
5. Test as you build

---

## 📚 Resources

- Raylib Documentation: https://www.raylib.com/
- C++ Enums: https://en.cppreference.com/w/cpp/language/enum
- OOP Concepts: https://en.cppreference.com/w/cpp/language/class
- Vector Reference: https://en.cppreference.com/w/cpp/container/vector

---

**Project Start**: Week 1  
**Project End**: Week 3  
**Status**: 🔨 In Development

**Good luck! You've got this! 🎮**

