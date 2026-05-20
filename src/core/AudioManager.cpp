#include "AudioManager.h"

#include "raylib.h"

namespace {
Sound g_fireBall = {};
Sound g_explosion = {};
Sound g_hammerSmash = {};
Sound g_knife = {};
Sound g_laser = {};
Sound g_magicWand = {};
Sound g_shieldImpact = {};
Sound g_thunder = {};
Music g_mainMenuMusic = {};
Music g_inGameMusic = {};
Music g_gameOverMusic = {};
Music g_victoryMusic = {};
Music* g_currentMusic = nullptr;
AudioManager::MusicState g_currentMusicState = AudioManager::MusicState::MainMenu;
bool g_hasMusicState = false;
bool g_initialized = false;

void PlayLoadedSound(const Sound& sound) {
    if (!g_initialized || sound.frameCount == 0) return;
    PlaySound(sound);
}

Music* GetMusicByState(AudioManager::MusicState state) {
    switch (state) {
        case AudioManager::MusicState::MainMenu: return &g_mainMenuMusic;
        case AudioManager::MusicState::InGame: return &g_inGameMusic;
        case AudioManager::MusicState::GameOver: return &g_gameOverMusic;
        case AudioManager::MusicState::Victory: return &g_victoryMusic;
    }
    return nullptr;
}
}

namespace AudioManager {
void Initialize() {
    if (g_initialized) return;

    InitAudioDevice();
    g_fireBall = LoadSound("Sound/FireBall.mp3");
    g_explosion = LoadSound("Sound/Explosion.mp3");
    g_hammerSmash = LoadSound("Sound/HammerSmash.mp3");
    g_knife = LoadSound("Sound/Knife.mp3");
    g_laser = LoadSound("Sound/Laser.mp3");
    g_magicWand = LoadSound("Sound/MagicWand.mp3");
    g_shieldImpact = LoadSound("Sound/ShieldImpact.mp3");
    g_thunder = LoadSound("Sound/Thunder.mp3");
    g_mainMenuMusic = LoadMusicStream("Sound/Mainmenusound.mp3");
    g_inGameMusic = LoadMusicStream("Sound/Ingamemusic.mp3");
    g_gameOverMusic = LoadMusicStream("Sound/GameOver.mp3");
    g_victoryMusic = LoadMusicStream("Sound/GameVictory.mp3");
    g_initialized = true;
    SetMusicState(MusicState::MainMenu);
}

void Shutdown() {
    if (!g_initialized) return;

    if (g_currentMusic != nullptr) {
        StopMusicStream(*g_currentMusic);
        g_currentMusic = nullptr;
    }

    UnloadSound(g_fireBall);
    UnloadSound(g_explosion);
    UnloadSound(g_hammerSmash);
    UnloadSound(g_knife);
    UnloadSound(g_laser);
    UnloadSound(g_magicWand);
    UnloadSound(g_shieldImpact);
    UnloadSound(g_thunder);
    UnloadMusicStream(g_mainMenuMusic);
    UnloadMusicStream(g_inGameMusic);
    UnloadMusicStream(g_gameOverMusic);
    UnloadMusicStream(g_victoryMusic);
    CloseAudioDevice();
    g_hasMusicState = false;
    g_initialized = false;
}

void Update() {
    if (!g_initialized || g_currentMusic == nullptr) return;

    UpdateMusicStream(*g_currentMusic);
    float musicLength = GetMusicTimeLength(*g_currentMusic);
    if (musicLength > 0.0f && GetMusicTimePlayed(*g_currentMusic) >= musicLength - 0.05f) {
        StopMusicStream(*g_currentMusic);
        PlayMusicStream(*g_currentMusic);
        return;
    }
    if (!IsMusicStreamPlaying(*g_currentMusic)) {
        PlayMusicStream(*g_currentMusic);
    }
}

void SetMusicState(MusicState state) {
    if (!g_initialized) return;
    if (g_currentMusic != nullptr && g_hasMusicState && g_currentMusicState == state) return;

    if (g_currentMusic != nullptr) {
        StopMusicStream(*g_currentMusic);
    }

    g_currentMusicState = state;
    g_hasMusicState = true;
    g_currentMusic = GetMusicByState(state);
    if (g_currentMusic != nullptr) {
        SeekMusicStream(*g_currentMusic, 0.0f);
        PlayMusicStream(*g_currentMusic);
        UpdateMusicStream(*g_currentMusic);
    }
}

void PlayFireBall() { PlayLoadedSound(g_fireBall); }
void PlayExplosion() { PlayLoadedSound(g_explosion); }
void PlayHammerSmash() { PlayLoadedSound(g_hammerSmash); }
void PlayKnife() { PlayLoadedSound(g_knife); }
void PlayLaser() { PlayLoadedSound(g_laser); }
void PlayMagicWand() { PlayLoadedSound(g_magicWand); }
void PlayShieldImpact() { PlayLoadedSound(g_shieldImpact); }
void PlayThunder() { PlayLoadedSound(g_thunder); }
}
