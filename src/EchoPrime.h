#pragma once

#include "Boss.h"
#include <vector>

class AudioManager;
class Player;

/**
 * Echo Clone
 * Clone of Echo created in Phase 3
 */
struct EchoClone {
    float x, y;
    float vx, vy;
    bool active;
    float animTimer;

    EchoClone(float px, float py)
        : x(px), y(py), vx(0), vy(0), active(true), animTimer(0) {}
};

/**
 * Code Particle
 * Visual effect for Phase 4 (Pure Code Form)
 */
struct CodeParticle {
    float x, y;
    float vx, vy;
    char character;
    float lifetime;

    CodeParticle(float px, float py, char c)
        : x(px), y(py), vx((rand() % 200) - 100.0f), vy((rand() % 200) - 100.0f),
          character(c), lifetime(2.0f) {}
};

/**
 * Echo Prime - Final Boss
 * The AI that became self-aware
 *
 * Phase 1 (100-75% HP): Tutorial Echo - Friendly, slow attacks
 * Phase 2 (75-50% HP):  Glitched Echo - Aggressive, corruption effects
 * Phase 3 (50-25% HP):  Multiple Echoes - Spawns clones
 * Phase 4 (25-0% HP):   Pure Code Form - Reality breaks, bullet hell
 */
class EchoPrime : public Boss {
public:
    EchoPrime(float x, float y, AudioManager* audioManager = nullptr);
    ~EchoPrime() override;

    void update(float deltaTime, Player* player) override;
    void render(SDL_Renderer* renderer) override;

    // Override phase transitions
    void transitionToPhase(BossPhase newPhase) override;

    // Getters
    const std::vector<EchoClone>& getClones() const { return m_clones; }
    std::string getName() const override { return "ECHO PRIME"; }

private:
    // Movement
    float m_targetX, m_targetY;
    float m_speed;
    float m_hoverOffset;

    // Attack
    float m_attackTimer;
    float m_attackCooldown;

    // Clones (Phase 3)
    std::vector<EchoClone> m_clones;
    float m_cloneSpawnTimer;

    // Code Form (Phase 4)
    std::vector<CodeParticle> m_codeParticles;
    bool m_isCodeForm;

    // Visual
    float m_animTimer;
    float m_glitchTimer;
    int m_glitchOffsetX, m_glitchOffsetY;

    // AI
    void updatePhase1(float deltaTime, Player* player);
    void updatePhase2(float deltaTime, Player* player);
    void updatePhase3(float deltaTime, Player* player);
    void updatePhase4(float deltaTime, Player* player);

    void spawnClone();
    void updateClones(float deltaTime, Player* player);
    void updateCodeParticles(float deltaTime);
    void emitCodeParticles();

    // Rendering helpers
    void renderFriendlyEcho(SDL_Renderer* renderer);
    void renderGlitchedEcho(SDL_Renderer* renderer);
    void renderCodeForm(SDL_Renderer* renderer);
};
