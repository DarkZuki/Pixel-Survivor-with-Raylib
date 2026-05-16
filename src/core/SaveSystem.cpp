#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

#include "../player/Player.h"
#include "../skill/Skill.h"
#include "../wave/WaveManager.h"
#include "../weapon/weapon.h"

using namespace std;

bool saveGame(const char* fileName, Player& player, WaveManager& waveSystem, float gameTimer, int currentDiffID,
              const vector<Weapon*>& allWeapons, const vector<Weapon*>& weaponInventory, Weapon* currentWeapon,
              const vector<Skill*>& allSkills, const vector<Skill*>& skillInventory) {
    ofstream out(fileName);
    if (!out.is_open()) return false;

    out << fixed << setprecision(2);
    out << "difficulty: " << currentDiffID << '\n';
    out << "wave_timer: " << waveSystem.getInternalTimer() << '\n';
    out << "game_timer: " << gameTimer << '\n';
    out << "player_x: " << player.getX() << '\n';
    out << "player_y: " << player.getY() << '\n';
    out << "hp: " << player.getHp() << '\n';
    out << "max_hp: " << player.getMaxHp() << '\n';
    out << "level: " << player.getLevel() << '\n';
    out << "exp: " << player.getExp() << '\n';
    out << "score: " << player.getScore() << '\n';

    int currentWeaponIndex = -1, weapon0Type = -1, weapon1Type = -1, skill0Type = -1, skill1Type = -1, skill2Type = -1;
    for (size_t i = 0; i < allWeapons.size(); i++) {
        if (allWeapons[i] == currentWeapon) currentWeaponIndex = (int)i;
        if (weaponInventory.size() > 0 && allWeapons[i] == weaponInventory[0]) weapon0Type = (int)i;
        if (weaponInventory.size() > 1 && allWeapons[i] == weaponInventory[1]) weapon1Type = (int)i;
    }
    for (size_t i = 0; i < allSkills.size(); i++) {
        if (skillInventory.size() > 0 && allSkills[i] == skillInventory[0]) skill0Type = (int)i;
        if (skillInventory.size() > 1 && allSkills[i] == skillInventory[1]) skill1Type = (int)i;
        if (skillInventory.size() > 2 && allSkills[i] == skillInventory[2]) skill2Type = (int)i;
    }

    out << "current_weapon_index: " << currentWeaponIndex << '\n';
    out << "weapon_0_type: " << weapon0Type << '\n';
    out << "weapon_0_level: " << (weaponInventory.size() > 0 ? weaponInventory[0]->getLevel() : 0) << '\n';
    out << "weapon_1_type: " << weapon1Type << '\n';
    out << "weapon_1_level: " << (weaponInventory.size() > 1 ? weaponInventory[1]->getLevel() : 0) << '\n';
    out << "skill_0_type: " << skill0Type << '\n';
    out << "skill_0_level: " << (skillInventory.size() > 0 ? skillInventory[0]->getLevel() : 0) << '\n';
    out << "skill_1_type: " << skill1Type << '\n';
    out << "skill_1_level: " << (skillInventory.size() > 1 ? skillInventory[1]->getLevel() : 0) << '\n';
    out << "skill_2_type: " << skill2Type << '\n';
    out << "skill_2_level: " << (skillInventory.size() > 2 ? skillInventory[2]->getLevel() : 0) << '\n';
    return true;
}

bool loadGame(const char* fileName, Player& player, WaveManager& waveSystem, float& gameTimer, int& currentDiffID,
              bool& shouldShowUpgrade, int& previousLevel, const vector<Weapon*>& allWeapons,
              vector<Weapon*>& weaponInventory, Weapon*& currentWeapon, const vector<Skill*>& allSkills,
              vector<Skill*>& skillInventory) {
    ifstream in(fileName);
    if (!in.is_open()) return false;

    float playerX, playerY, waveTimer;
    int hp, maxHp, level, exp, score, selectedWeaponIndex;
    int weapon0Type, weapon0Level, weapon1Type, weapon1Level;
    int skill0Type, skill0Level, skill1Type, skill1Level, skill2Type, skill2Level;
    string key;
    char colon;

    if (!(in >> key >> colon >> currentDiffID) || key != "difficulty" || colon != ':') return false;
    if (!(in >> key >> colon >> waveTimer) || key != "wave_timer" || colon != ':') return false;
    if (!(in >> key >> colon >> gameTimer) || key != "game_timer" || colon != ':') return false;
    if (!(in >> key >> colon >> playerX) || key != "player_x" || colon != ':') return false;
    if (!(in >> key >> colon >> playerY) || key != "player_y" || colon != ':') return false;
    if (!(in >> key >> colon >> hp) || key != "hp" || colon != ':') return false;
    if (!(in >> key >> colon >> maxHp) || key != "max_hp" || colon != ':') return false;
    if (!(in >> key >> colon >> level) || key != "level" || colon != ':') return false;
    if (!(in >> key >> colon >> exp) || key != "exp" || colon != ':') return false;
    if (!(in >> key >> colon >> score) || key != "score" || colon != ':') return false;
    if (!(in >> key >> colon >> selectedWeaponIndex) || key != "current_weapon_index" || colon != ':') return false;
    if (!(in >> key >> colon >> weapon0Type) || key != "weapon_0_type" || colon != ':') return false;
    if (!(in >> key >> colon >> weapon0Level) || key != "weapon_0_level" || colon != ':') return false;
    if (!(in >> key >> colon >> weapon1Type) || key != "weapon_1_type" || colon != ':') return false;
    if (!(in >> key >> colon >> weapon1Level) || key != "weapon_1_level" || colon != ':') return false;
    if (!(in >> key >> colon >> skill0Type) || key != "skill_0_type" || colon != ':') return false;
    if (!(in >> key >> colon >> skill0Level) || key != "skill_0_level" || colon != ':') return false;
    if (!(in >> key >> colon >> skill1Type) || key != "skill_1_type" || colon != ':') return false;
    if (!(in >> key >> colon >> skill1Level) || key != "skill_1_level" || colon != ':') return false;
    if (!(in >> key >> colon >> skill2Type) || key != "skill_2_type" || colon != ':') return false;
    if (!(in >> key >> colon >> skill2Level) || key != "skill_2_level" || colon != ':') return false;

    player.setPosition(playerX, playerY);
    player.setMaxHp(maxHp);
    player.setHp(hp);
    player.setLevel(level);
    player.setExp(exp);
    player.setScore(score);
    waveSystem.setDifficulty(currentDiffID);
    waveSystem.setInternalTimer(waveTimer);

    for (auto weapon : allWeapons) weapon->setLevel(0);
    for (auto skill : allSkills) skill->setLevel(0);

    weaponInventory.clear();
    skillInventory.clear();
    currentWeapon = nullptr;

    int weaponTypes[] = {weapon0Type, weapon1Type}, weaponLevels[] = {weapon0Level, weapon1Level};
    int skillTypes[] = {skill0Type, skill1Type, skill2Type}, skillLevels[] = {skill0Level, skill1Level, skill2Level};

    for (int i = 0; i < 2; i++) if (weaponTypes[i] >= 0 && weaponTypes[i] < (int)allWeapons.size()) {
        allWeapons[weaponTypes[i]]->setLevel(weaponLevels[i]);
        weaponInventory.push_back(allWeapons[weaponTypes[i]]);
        if (weaponTypes[i] == selectedWeaponIndex) currentWeapon = allWeapons[weaponTypes[i]];
    }
    for (int i = 0; i < 3; i++) if (skillTypes[i] >= 0 && skillTypes[i] < (int)allSkills.size()) {
        allSkills[skillTypes[i]]->setLevel(skillLevels[i]);
        skillInventory.push_back(allSkills[skillTypes[i]]);
    }

    if (currentWeapon == nullptr && !weaponInventory.empty()) currentWeapon = weaponInventory[0];

    shouldShowUpgrade = false;
    previousLevel = player.getLevel();
    return true;
}
