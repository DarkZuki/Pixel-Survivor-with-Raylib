#include <cassert>
#include <iostream>
#include <stdexcept>

#include "../src/wave/WaveManager.h"
#include "../src/player/Player.h"

// File test này dùng assert thuần để đáp ứng yêu cầu

void testWaveProgression() {
    WaveManager manager;

    // Mốc wave được suy ra từ internal timer, nên test này khóa lại các mốc chuyển wave cơ bản.
    manager.setInternalTimer(0.0f);
    assert(manager.getCurrentWaveNumber() == 1);

    manager.setInternalTimer(31.0f);
    assert(manager.getCurrentWaveNumber() == 2);

    manager.setInternalTimer(120.0f);
    assert(manager.getCurrentWaveNumber() == 5);
}

void testWaveCapAtTwenty() {
    WaveManager manager;
    // Dù timer tăng rất lớn, wave hiện tại vẫn phải bị chặn ở trần thiết kế.
    manager.setInternalTimer(9999.0f);
    assert(manager.getCurrentWaveNumber() == MAX_WAVE_NUMBER);
}

void testDifficultyMultipliers() {
    WaveManager manager;

    // Các assert này bảo vệ balance cơ bản giữa 3 mức khó.
    manager.setDifficulty(0);
    assert(manager.getDifficultyHPMultiplier() == 0.8f);
    assert(manager.getDifficultySpeedMultiplier() == 0.8f);

    manager.setDifficulty(1);
    assert(manager.getDifficultyHPMultiplier() == 2.0f);
    assert(manager.getDifficultySpeedMultiplier() == 1.1f);

    manager.setDifficulty(2);
    assert(manager.getDifficultyHPMultiplier() == 5.0f);
    assert(manager.getDifficultySpeedMultiplier() == 1.4f);
}

void testDifficultyThrowsOnInvalidInput() {
    WaveManager manager;
    bool threw = false;

    // Input ngoài khoảng hợp lệ cần bị chặn rõ ràng thay vì âm thầm rơi về default.
    try {
        manager.setDifficulty(99);
    } catch (const std::invalid_argument&) {
        threw = true;
    }

    assert(threw && "setDifficulty(99) must throw invalid_argument");
}

void testBossSpawnRule() {
    WaveManager manager;
    manager.setInternalTimer(570.0f); // wave 20

    // Boss chỉ nên spawn một lần ở điều kiện hợp lệ.
    assert(manager.shouldSpawnBoss() == true);

    manager.markBossSpawned();
    assert(manager.shouldSpawnBoss() == false);
}

void testPlayerScoreRules() {
    Player player;
    assert(player.getScore() == 0);

    // Score tăng bình thường với số dương...
    player.addScore(25);
    assert(player.getScore() == 25);

    // ...và phải ném lỗi nếu có code nào cố cộng điểm âm.
    bool threw = false;
    try {
        player.addScore(-5);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw && "addScore(-5) must throw invalid_argument");
}

int main() {
    testWaveProgression();
    testWaveCapAtTwenty();
    testDifficultyMultipliers();
    testDifficultyThrowsOnInvalidInput();
    testBossSpawnRule();
    testPlayerScoreRules();

    std::cout << "All basic unit tests passed.\n";
    return 0;
}
