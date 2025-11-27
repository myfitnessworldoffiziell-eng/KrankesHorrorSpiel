#pragma once

#include <SDL2/SDL_mixer.h>
#include <string>
#include <unordered_map>
#include <memory>

/**
 * AudioManager
 * Verwaltet alle Sounds und Musik
 * Unterstützt progressive Corruption von Audio
 */
class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    bool initialize();
    void shutdown();

    // Musik
    bool loadMusic(const std::string& id, const std::string& filepath);
    void playMusic(const std::string& id, int loops = -1); // -1 = infinite
    void stopMusic();
    void pauseMusic();
    void resumeMusic();
    void setMusicVolume(int volume); // 0-128

    // Sound Effects
    bool loadSound(const std::string& id, const std::string& filepath);
    void playSound(const std::string& id, int volume = 128);

    // Horror-specific
    void distortMusic(float amount); // 0.0 - 1.0
    void playJumpscare();
    void playWhiteNoise(int duration_ms);

    // Update (für progressive effects)
    void update(float deltaTime, int corruptionLevel);

private:
    std::unordered_map<std::string, Mix_Music*> m_music;
    std::unordered_map<std::string, Mix_Chunk*> m_sounds;

    std::string m_currentMusicId;
    int m_musicVolume;
    bool m_initialized;

    // Corruption state
    float m_distortionLevel;
};
