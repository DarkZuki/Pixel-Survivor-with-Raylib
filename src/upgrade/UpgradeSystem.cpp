#include "UpgradeSystem.h"
#include "../weapon/level.h"

// Hàm hiển thị menu với các thông tin nâng cấp
void UpgradeSystem::showUpgradeMenu(std::vector<Weapon*>& weapons, int weaponCount, int maxWeapons) {
    active = true;
    paused = true;
    delay = 1.5;
    selected = {};

    boxes[0] = {85, 160, 200, 250};
    boxes[1] = {315, 160, 200, 250};
    boxes[2] = {545, 160, 200, 250};

    for (int i = 0; i < 3; i++) options[i] = {};

    int k = 0;
    
    // Kiểm tra số lượng vũ khí
    if (weaponCount < maxWeapons) {
        for (int i = 0; i < 4 && k < 3; i++) {
            if (i >= (int)weapons.size() || weapons[i] == nullptr || weapons[i]->getLevel() <= 0) {
                options[k].weaponType = i;
                options[k].isNewWeapon = true;
                k++;
            }
        }
    }

    for (int i = 0; i < 4 && k < 3; i++) {
        
        // Kiểm tra số lượng, có hay ko, level trên 0 và dưới 10
        if (i < (int)weapons.size() && weapons[i] != nullptr && weapons[i]->getLevel() > 0 && weapons[i]->getLevel() < 10) {
            options[k].weaponType = i;
            options[k].upgradeLevel = weapons[i]->getLevel() + 1;
            options[k].weaponPtr = weapons[i];
            k++;
        }
    }

    //  Kiểm tra có vũ khí
    if (options[0].weaponType < 0) {
        options[0].weaponType = 0;
        options[0].isNewWeapon = true;
    }
}

// Hàm check click chuột khi bấm nâng cấp và bấm skip
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
        if (options[i].weaponType >= 0 && CheckCollisionPointRec(mouse, boxes[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            selected = options[i];
            active = false;
            paused = false;
            return;
        }
    }
}

// Hàm vẽ giao diện nâng cấp
void UpgradeSystem::draw() {
    if (!active) return;

    DrawRectangle(0, 0, 800, 600, Fade(BLACK, 0.8f));
    DrawText("CHOOSE UPGRADE", 250, 70, 30, GOLD);

    for (int i = 0; i < 3; i++) {

        // Kiểm tra có loại vũ khí ko
        if (options[i].weaponType < 0) continue;

        Color color = options[i].isNewWeapon ? Color{90, 70, 130, 255} : Color{70, 110, 70, 255};
        DrawRectangleRec(boxes[i], color);
        DrawRectangleLinesEx(boxes[i], 2, WHITE);
        DrawText(getWeaponLevelWeaponName(options[i].weaponType), boxes[i].x + 25, boxes[i].y + 25, 22, WHITE);
        DrawText(TextFormat("Level %d", options[i].upgradeLevel), boxes[i].x + 55, boxes[i].y + 85, 20, YELLOW);
        DrawText(options[i].isNewWeapon ? "Unlock weapon" : getWeaponLevelData(options[i].weaponType, options[i].upgradeLevel).name.c_str(),
                 boxes[i].x + 15, boxes[i].y + 145, 16, WHITE);

        // Kiểm tra vũ khí mới 
        if (options[i].isNewWeapon) DrawText("NEW", boxes[i].x + 75, boxes[i].y + 205, 20, GREEN);
    }

    DrawRectangleRec(skipBox, DARKGREEN);
    DrawRectangleLinesEx(skipBox, 2, WHITE);
    DrawText("SKIP", skipBox.x + 45, skipBox.y + 15, 20, WHITE);

    // Kiểm tra thời gian đợi khi mới vào menu cho mỗi lần giảm dần
    if (delay > 0) DrawText("WAIT...", 360, 460, 20, LIGHTGRAY);
}
