#pragma once
#include "SDL_Platform.h"

#include <vector>
#include <random>

class AudioManager;

/**
 * CorruptionSystem
 * Visuelle Glitch-Effekte und Bildschirm-Manipulationen
 */
class CorruptionSystem {
public:
    CorruptionSystem(SDL_Renderer* renderer);
    ~CorruptionSystem();

    void update(float deltaTime, int corruptionLevel);
    void render(SDL_Renderer* renderer, int corruptionLevel);

    // Manueller Glitch-Trigger
    void forceGlitch();

    // Audio
    void setAudioManager(AudioManager* audio) { m_audioManager = audio; }

private:
    void renderStaticNoise(SDL_Renderer* renderer, int intensity);
    void renderScreenShake(SDL_Renderer* renderer);
    void renderColorDistortion(SDL_Renderer* renderer);
    void renderScanlines(SDL_Renderer* renderer);

    SDL_Renderer* m_renderer;
    AudioManager* m_audioManager;

    // Glitch State
    bool m_isGlitching;
    float m_glitchTimer;
    float m_glitchDuration;

    // Random
    std::mt19937 m_rng;

    // Screen Shake
    int m_shakeOffsetX;
    int m_shakeOffsetY;
};
