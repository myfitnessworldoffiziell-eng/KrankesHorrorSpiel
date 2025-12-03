#pragma once
#include "SDL_Platform.h"

#include "UIHelper.h"
#include <vector>

/**
 * MainMenu
 * Hauptmenü des Spiels - mit Horror-Atmosphäre
 */
class MainMenu {
public:
    enum class MenuResult {
        NONE,
        START_GAME,
        SETTINGS,
        QUIT
    };

    MainMenu();
    ~MainMenu();

    void handleInput(SDL_Event& event);
    MenuResult update(float deltaTime);
    void render(SDL_Renderer* renderer);

private:
    std::vector<UIHelper::Button> m_buttons;
    MenuResult m_result;

    // Visual horror effects
    float m_glitchTimer;
    bool m_shouldGlitch;
    float m_pulseTimer;              // Breathing/pulsing effect
    float m_corruptionTimer;         // Corruption animation
    int m_glitchIntensity;          // 0-10 glitch strength

    // Background horror particles
    struct HorrorParticle {
        float x, y;
        float vx, vy;  // velocity
        float size;
        int type;      // 0=shadow, 1=glitch, 2=eye
        float life;    // lifetime
        float alpha;   // transparency
    };
    std::vector<HorrorParticle> m_particles;
    float m_particleSpawnTimer;

    // Background decorations (creepy elements)
    struct BackgroundElement {
        float x, y;
        float width, height;
        int type;      // 0=shadow, 1=crack, 2=corruption
        float pulsePhase;
    };
    std::vector<BackgroundElement> m_backgroundElements;

    // Screen shake
    float m_shakeIntensity;
    float m_shakeX, m_shakeY;

    // Mouse state
    int m_mouseX, m_mouseY;
    bool m_mouseClicked;

    // Horror effect helpers
    void updateHorrorEffects(float deltaTime);
    void updateParticles(float deltaTime);
    void spawnRandomParticle();
    void renderHorrorBackground(SDL_Renderer* renderer);
    void renderParticles(SDL_Renderer* renderer);
    void renderGlitchEffects(SDL_Renderer* renderer);
};
