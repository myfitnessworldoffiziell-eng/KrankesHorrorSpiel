#pragma once

#include <SDL2/SDL.h>
#include <random>

class AudioManager;

/**
 * JumpscareSystem
 * Verwaltet Jumpscares (visual + audio)
 */
class JumpscareSystem {
public:
    enum class ScareType {
        FLASH,           // Screen Flash + Sound
        SCREAMER_IMAGE,  // Echo face fills screen
        SUDDEN_APPEAR,   // Enemy suddenly appears
        AUDIO_SPIKE      // Sudden loud sound
    };

    JumpscareSystem(AudioManager* audioManager);
    ~JumpscareSystem();

    // Trigger scares
    void triggerScare(ScareType type);
    void triggerRandomScare();

    // Auto-scare based on corruption
    void update(float deltaTime, int corruptionLevel);
    void render(SDL_Renderer* renderer);

    bool isActive() const { return m_active; }

private:
    AudioManager* m_audioManager;

    bool m_active;
    ScareType m_currentType;
    float m_scareTimer;
    float m_scareDuration;

    // Random
    std::mt19937 m_rng;
    float m_timeSinceLastScare;

    // Flash effect
    Uint8 m_flashAlpha;
};
