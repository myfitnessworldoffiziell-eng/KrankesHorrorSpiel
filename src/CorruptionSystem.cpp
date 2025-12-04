#include "CorruptionSystem.h"
#include "AudioManager.h"
#include <iostream>

CorruptionSystem::CorruptionSystem(SDL_Renderer* renderer)
    : m_renderer(renderer)
    , m_audioManager(nullptr)
    , m_isGlitching(false)
    , m_glitchTimer(0.0f)
    , m_glitchDuration(0.0f)
    , m_shakeOffsetX(0)
    , m_shakeOffsetY(0)
{
    std::random_device rd;
    m_rng.seed(rd());
}

CorruptionSystem::~CorruptionSystem() {
}

void CorruptionSystem::update(float deltaTime, int corruptionLevel) {
    // Glitch Timer
    if (m_isGlitching) {
        m_glitchTimer += deltaTime;
        if (m_glitchTimer >= m_glitchDuration) {
            m_isGlitching = false;
            m_glitchTimer = 0.0f;
        }
    } else {
        // Random Glitch basierend auf corruption level
        std::uniform_int_distribution<> dis(0, 1000);
        int chance = dis(m_rng);

        // Je höher corruption, desto öfter glitches
        if (chance < corruptionLevel / 2) {
            forceGlitch();
        }
    }

    // Screen Shake Update
    if (m_isGlitching) {
        std::uniform_int_distribution<> shakeDis(-5, 5);
        m_shakeOffsetX = shakeDis(m_rng);
        m_shakeOffsetY = shakeDis(m_rng);
    } else {
        m_shakeOffsetX = 0;
        m_shakeOffsetY = 0;
    }
}

void CorruptionSystem::render(SDL_Renderer* renderer, int corruptionLevel) {
    if (!m_isGlitching && corruptionLevel < 10) {
        return; // Keine Effekte bei niedrigem corruption
    }

    // Scanlines (immer leicht sichtbar)
    renderScanlines(renderer);

    if (m_isGlitching) {
        // Static Noise
        renderStaticNoise(renderer, 50);

        // Color Distortion
        renderColorDistortion(renderer);
    } else if (corruptionLevel > 30) {
        // Leichtes Rauschen auch ohne aktiven Glitch
        renderStaticNoise(renderer, corruptionLevel / 10);
    }
}

void CorruptionSystem::forceGlitch() {
    m_isGlitching = true;
    m_glitchTimer = 0.0f;

    // Shorter, more intense glitches (0.05-0.15 seconds)
    std::uniform_real_distribution<> durationDis(0.05, 0.15);
    m_glitchDuration = static_cast<float>(durationDis(m_rng));

    std::cout << "[Corruption] 👻 GLITCH! Duration: " << m_glitchDuration << "s" << std::endl;

    // Play creepy glitch sound effect
    if (m_audioManager) {
        m_audioManager->playSound("glitch", 120);  // Loud!
    }
}

void CorruptionSystem::renderStaticNoise(SDL_Renderer* renderer, int intensity) {
    std::uniform_int_distribution<> posDis(0, 800);
    std::uniform_int_distribution<> colorDis(0, 255);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // More intense static noise (20x instead of 10x)
    for (int i = 0; i < intensity * 20; ++i) {
        int x = posDis(m_rng);
        int y = posDis(m_rng);
        int gray = colorDis(m_rng);

        // More opaque for more visible static (150 instead of 100)
        SDL_SetRenderDrawColor(renderer, gray, gray, gray, 150);
        SDL_RenderDrawPoint(renderer, x, y);
    }
}

void CorruptionSystem::renderColorDistortion(SDL_Renderer* renderer) {
    // Intense RGB-Shift with varied colors
    std::uniform_int_distribution<> posDis(0, 800);
    std::uniform_int_distribution<> sizeDis(10, 100);  // Larger rectangles
    std::uniform_int_distribution<> colorChoice(0, 5);  // 6 different color types

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);

    // More rectangles for more intense effect (15 instead of 5)
    for (int i = 0; i < 15; ++i) {
        SDL_Rect rect;
        rect.x = posDis(m_rng);
        rect.y = posDis(m_rng);
        rect.w = sizeDis(m_rng);
        rect.h = sizeDis(m_rng);

        // Random color choice for variety
        int color = colorChoice(m_rng);
        switch (color) {
            case 0: SDL_SetRenderDrawColor(renderer, 255, 0, 0, 80);   break; // Red
            case 1: SDL_SetRenderDrawColor(renderer, 0, 255, 0, 80);   break; // Green
            case 2: SDL_SetRenderDrawColor(renderer, 0, 0, 255, 80);   break; // Blue
            case 3: SDL_SetRenderDrawColor(renderer, 255, 255, 0, 80); break; // Yellow
            case 4: SDL_SetRenderDrawColor(renderer, 255, 0, 255, 80); break; // Magenta
            case 5: SDL_SetRenderDrawColor(renderer, 0, 255, 255, 80); break; // Cyan
        }
        SDL_RenderFillRect(renderer, &rect);

        // Add offset shifted version for chromatic aberration effect
        rect.x += (i % 2 == 0) ? 5 : -5;
        rect.y += (i % 2 == 0) ? -3 : 3;

        // Different color for the shifted version
        int color2 = (color + 3) % 6;
        switch (color2) {
            case 0: SDL_SetRenderDrawColor(renderer, 255, 0, 0, 60);   break;
            case 1: SDL_SetRenderDrawColor(renderer, 0, 255, 0, 60);   break;
            case 2: SDL_SetRenderDrawColor(renderer, 0, 0, 255, 60);   break;
            case 3: SDL_SetRenderDrawColor(renderer, 255, 255, 0, 60); break;
            case 4: SDL_SetRenderDrawColor(renderer, 255, 0, 255, 60); break;
            case 5: SDL_SetRenderDrawColor(renderer, 0, 255, 255, 60); break;
        }
        SDL_RenderFillRect(renderer, &rect);
    }
}

void CorruptionSystem::renderScanlines(SDL_Renderer* renderer) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 30);

    for (int y = 0; y < 600; y += 4) {
        SDL_RenderDrawLine(renderer, 0, y, 800, y);
    }
}
