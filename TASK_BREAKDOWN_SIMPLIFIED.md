# PIXEL SURVIVOR - Simplified Task Breakdown (Year 2, 3 Weeks)

**Team Size**: 4 members  
**Timeline**: 3 weeks  
**Available Time**: ~3h weekdays + 6h weekends  
**Learning Goals**: Basic OOP (inheritance, polymorphism) + Vector/DSA basics

---

## ✅ What You Already Have Done

The game **already works**! You have:
- ✅ Player movement & shooting
- ✅ Enemy spawning and AI
- ✅ Bullet-enemy collisions
- ✅ HP & score system
- ✅ Game over screen

---

## 🎯 Remaining 3 Weeks: Polish & Improvements

### WEEK 1: Fix & Polish (10 hours total)

#### Task 1.1: Fix Game Bugs [2 hours] - Person A
**What to do:**
- [ ] Ensure score doesn't count after death ✅ (DONE)
- [ ] Test all collisions work
- [ ] Fix any crashes or glitches

**Learning**: Debugging, testing

---

#### Task 1.2: Improve Visuals [3 hours] - Person B
**What to do:**
- [ ] Make enemies different sizes (3 types)
- [ ] Add colors to bullets
- [ ] Better UI text display
- [ ] Draw health bars

**Learning**: Basic graphics, arrays/vectors

**Code example:**
```cpp
// Different enemy types
enum class EnemyType { SMALL = 0, MEDIUM = 1, LARGE = 2 };
```

---

#### Task 1.3: Refactor Code [3 hours] - Person C
**What to do:**
- [ ] Move collision code to separate functions
- [ ] Make constants instead of magic numbers
- [ ] Clean up main.cpp
- [ ] Add comments

**Learning**: Code organization, functions, cleanup

**Target:** Get your codebase clean and maintainable

---

#### Task 1.4: Improve Enemy AI [2 hours] - Person D
**What to do:**
- [ ] Enemies should spread out (not all clump together)
- [ ] Different enemy types move at different speeds
- [ ] Random spawn positions

**Learning**: Behavior variation, vectors

---

### WEEK 2: Features (12 hours total)

#### Task 2.1: Power-ups System [3 hours] - Person A
**What to do:**
- [ ] Create simple drops that fall from dead enemies
- [ ] Pick-up to boost damage (red)
- [ ] Pick-up to boost speed (blue)
- [ ] Pick-up to heal (green)

**Learning**: Polymorphism (different item types), vectors of objects

**Code example:**
```cpp
class PowerUp {
    int type; // 0=damage, 1=speed, 2=health
    void apply(Player* p) { /* apply effect */ }
};
vector<PowerUp*> powerups;
```

---

#### Task 2.2: Waves/Difficulty [3 hours] - Person B
**What to do:**
- [ ] After every 30 seconds, increase spawn rate
- [ ] Display current wave number
- [ ] Enemies get faster over time
- [ ] Track playtime

**Learning**: Game state management, timing

---

#### Task 2.3: Sound & Effects [3 hours] - Person C
**What to do:**
- [ ] Add simple sound effects (raylib has easy sound)
- [ ] Shoot sound
- [ ] Enemy death sound
- [ ] Game over sound

**Learning**: Raylib audio API (very simple)

---

#### Task 2.4: Better UI [3 hours] - Person D
**What to do:**
- [ ] Display current wave
- [ ] Show enemy count
- [ ] Display playtime
- [ ] Better game over screen

**Learning**: String formatting, screen layout

---

### WEEK 3: Final Polish (8 hours total)

#### Task 3.1: Testing & Bug Fixes [3 hours] - Person A
**What to do:**
- [ ] Play for 10 minutes, find bugs
- [ ] Fix crashes
- [ ] Test all features
- [ ] Fix lag issues

---

#### Task 3.2: Stats & Tracking [2 hours] - Person B
**What to do:**
- [ ] Track best score
- [ ] Track playtime
- [ ] Can reset stats

**Learning**: File I/O basics

---

#### Task 3.3: Documentation [2 hours] - Person C
**What to do:**
- [ ] Comment all classes
- [ ] Write README explaining how it works
- [ ] Document each method

**Learning**: Technical writing, code clarity

---

#### Task 3.4: Optimization [1 hour] - Person D
**What to do:**
- [ ] Remove unused code
- [ ] Check for memory leaks
- [ ] Keep FPS smooth

---

## 📊 Time Estimate
- Phase 1 (Week 1): ~10 hours
- Phase 2 (Week 2): ~12 hours
- Phase 3 (Week 3): ~8 hours
- **Total**: ~30 hours for 4 people = **~7.5 hours per person** ✅ Realistic!

**Parallel work**: All tasks in each week can be done in parallel by different team members.

---

## 🎓 Learning Outcomes

By the end, you'll understand:
- ✅ **OOP Basics**: Classes, inheritance, polymorphism, virtual functions
- ✅ **DSA Basics**: Vector management, dynamic object creation/deletion
- ✅ **Game Design**: Game loops, collision detection, state management
- ✅ **C++ Practices**: Encapsulation, memory management, code organization

---

## 📋 Simple Checklist

### Week 1
- [ ] All bugs fixed
- [ ] Visual improvements done
- [ ] Code refactored & clean
- [ ] Enemy AI improved

### Week 2
- [ ] Power-ups working
- [ ] Wave system working
- [ ] Sound effects added
- [ ] Better UI

### Week 3
- [ ] No crashes
- [ ] Stats tracked
- [ ] Code documented
- [ ] Runs smoothly

---

## 💡 Pro Tips

1. **Work in parallel simultaneously** - Each person takes one task
2. **Use git branches** - Avoid conflicts
3. **Test often** - Catch bugs early
4. **Keep it simple** - Don't add complex features
5. **Help each other** - You have 4 people, use that!

**Good luck! You've got this! 🚀**
