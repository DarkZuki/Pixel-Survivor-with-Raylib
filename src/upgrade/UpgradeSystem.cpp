#include "UpgradeSystem.h"
#include "../weapon/level.h"

// Hàm hiển thị menu với các thông tin nâng cấp
void UpgradeSystem::showUpgradeMenu(std::vector<Weapon*>& weapons, int weaponCount, int maxWeapons) {
    active = true;
    paused = true;
    delay = 1.5;
    selected = {};

    boxes[0] = {204, 268, 480, 450};
    boxes[1] = {756, 268, 480, 450};
    boxes[2] = {1308, 268, 480, 450};

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

    static Texture2D cardFrame = LoadTexture("Graphics/Upgrade Card Frame.png");
    Rectangle source = {0.0f, 0.0f, (float)cardFrame.width, (float)cardFrame.height};
    Rectangle frameRects[3] = {
        {10.0f, 268.0f, 700.0f, 500.0f},
        {562.0f, 268.0f, 700.0f, 500.0f},
        {1114.0f, 268.0f, 700.0f, 500.0f}
    };

    DrawRectangle(0, 0, 1920, 1040, Fade(BLACK, 0.8f));
    DrawText("CHOOSE UPGRADE", 600, 106, 54, GOLD);

    for (int i = 0; i < 3; i++) {

        // Kiểm tra có loại vũ khí ko
        if (options[i].weaponType < 0) continue;

        DrawTexturePro(cardFrame, source, frameRects[i], {0.0f, 0.0f}, 0.0f, WHITE);
        DrawText(getWeaponLevelWeaponName(options[i].weaponType), boxes[i].x + 60, boxes[i].y + 45, 40, WHITE);
        DrawText(TextFormat("Level %d", options[i].upgradeLevel), boxes[i].x + 132, boxes[i].y + 153, 36, YELLOW);
        DrawText(options[i].isNewWeapon ? "Unlock weapon" : getWeaponLevelData(options[i].weaponType, options[i].upgradeLevel).name.c_str(),
                 boxes[i].x + 36, boxes[i].y + 261, 29, WHITE);

        // Kiểm tra vũ khí mới 
        if (options[i].isNewWeapon) DrawText("NEW", boxes[i].x + 180, boxes[i].y + 369, 36, GREEN);
    }

    DrawRectangleRec(skipBox, DARKGREEN);
    DrawRectangleLinesEx(skipBox, 4, WHITE);
    DrawText("SKIP", skipBox.x + 108, skipBox.y + 27, 36, WHITE);

    // Kiểm tra thời gian đợi khi mới vào menu cho mỗi lần giảm dần
    if (delay > 0) DrawText("WAIT...", 864, 788, 36, LIGHTGRAY);
}
