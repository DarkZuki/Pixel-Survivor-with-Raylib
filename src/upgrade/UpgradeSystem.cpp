#include "UpgradeSystem.h"
#include "../weapon/level.h"
#include "../skill/SkillLevel.h"

void UpgradeSystem::showUpgradeMenu(std::vector<Weapon*>& weapons, std::vector<Skill*>& skills,
                                    int weaponCount, int maxWeapons,
                                    int skillCount, int maxSkills) {
    active = true;
    paused = true;
    delay = 1.5f;
    selected = {};

    boxes[0] = {204, 300, 480, 450};
    boxes[1] = {656, 300, 480, 450};
    boxes[2] = {1108, 300, 480, 450};

    for (int i = 0; i < 3; i++) options[i] = {};

    std::vector<UpgradeOption> candidates;

    if (weaponCount < maxWeapons) {
        for (int i = 0; i < (int)weapons.size(); i++) {
            if (weapons[i] != nullptr && weapons[i]->getLevel() <= 0) {
                UpgradeOption option;
                option.weaponType = i;
                option.isNewWeapon = true;
                candidates.push_back(option);
            }
        }
    }

    if (skillCount < maxSkills) {
        for (int i = 0; i < (int)skills.size(); i++) {
            if (skills[i] != nullptr && skills[i]->getLevel() <= 0) {
                UpgradeOption option;
                option.isSkill = true;
                option.skillType = i;
                option.isNewSkill = true;
                candidates.push_back(option);
            }
        }
    }

    for (int i = 0; i < (int)weapons.size(); i++) {
        if (weapons[i] != nullptr && weapons[i]->getLevel() > 0 && weapons[i]->getLevel() < 10) {
            UpgradeOption option;
            option.weaponType = i;
            option.upgradeLevel = weapons[i]->getLevel() + 1;
            option.weaponPtr = weapons[i];
            candidates.push_back(option);
        }
    }

    for (int i = 0; i < (int)skills.size(); i++) {
        if (skills[i] != nullptr && skills[i]->getLevel() > 0 && skills[i]->getLevel() < 10) {
            UpgradeOption option;
            option.isSkill = true;
            option.skillType = i;
            option.upgradeLevel = skills[i]->getLevel() + 1;
            option.skillPtr = skills[i];
            candidates.push_back(option);
        }
    }

    for (int i = 0; i < 3 && !candidates.empty(); i++) {
        int randomIndex = GetRandomValue(0, (int)candidates.size() - 1);
        options[i] = candidates[randomIndex];
        candidates[randomIndex] = candidates.back();
        candidates.pop_back();
    }

    if (options[0].weaponType < 0 && options[0].skillType < 0 && !weapons.empty()) {
        options[0].weaponType = 0;
        options[0].isNewWeapon = true;
    }
}

void UpgradeSystem::update() {
    if (!active) return;
    if (delay > 0) {
        delay -= GetFrameTime();
        return;
    }

    Vector2 mouse = GetMousePosition();
    if ((CheckCollisionPointRec(mouse, skipBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) || IsKeyPressed(KEY_ESCAPE)) {
        active = false;
        paused = false;
        selected = {};
        return;
    }

    for (int i = 0; i < 3; i++) {
        if ((options[i].weaponType >= 0 || options[i].skillType >= 0) &&
            CheckCollisionPointRec(mouse, boxes[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            selected = options[i];
            active = false;
            paused = false;
            return;
        }
    }
}

void UpgradeSystem::draw() {
    if (!active) return;
    const int upgradeTitleXOffset = 202;
    static Texture2D cardFrame = LoadTexture("Graphics/Upgrade Card Frame.png");
    Rectangle source = {0.0f, 0.0f, (float)cardFrame.width, (float)cardFrame.height};
    Rectangle frameRects[3] = {
        {160.0f, 310.0f, 700.0f, 500.0f},
        {610.0f, 310.0f, 700.0f, 500.0f},
        {1060.0f, 310.0f, 700.0f, 500.0f}
    };
    
    DrawRectangle(0, 0, 1920, 1040, Fade(BLACK, 0.8f));
    Texture2D upgradeFrame= LoadTexture("Graphics/Upgrade Frame.png");
    DrawTexture(upgradeFrame, 85, -30, WHITE);
    DrawText("CHOOSE UPGRADE", 700, 260, 54, GOLD);
    for (int i = 0; i < 3; i++) {
        if (options[i].weaponType < 0 && options[i].skillType < 0) continue;

        DrawTexturePro(cardFrame, source, frameRects[i], {0.0f, 0.0f}, 0.0f, WHITE);

        const char* title = options[i].isSkill ? getSkillLevelSkillName(options[i].skillType)
                                               : getWeaponLevelWeaponName(options[i].weaponType);
        DrawText(title, boxes[i].x + upgradeTitleXOffset, boxes[i].y + 96, 40, WHITE);
        DrawText(TextFormat("Level %d", options[i].upgradeLevel), boxes[i].x + 245, boxes[i].y + 198, 36, YELLOW);

        std::string upgradeName;
        if (options[i].isSkill) {
            upgradeName = options[i].isNewSkill ? "Unlock skill"
                                                : getSkillLevelData(options[i].skillType, options[i].upgradeLevel).name;
        } else {
            upgradeName = options[i].isNewWeapon ? "Unlock weapon"
                                                 : getWeaponLevelData(options[i].weaponType, options[i].upgradeLevel).name;
        }
        DrawText(upgradeName.c_str(), boxes[i].x + 205, boxes[i].y + 294, 29, WHITE);

        if (options[i].isNewWeapon || options[i].isNewSkill) {
            DrawText("NEW", boxes[i].x + 260, boxes[i].y + 392, 36, GREEN);
        }
    }

    if (delay > 0){
        DrawText("WAIT...", 908, 820, 36, LIGHTGRAY);
    }else{
        DrawText("SKIP", skipBox.x + 128, skipBox.y + -50, 36, RED);
    }
}
