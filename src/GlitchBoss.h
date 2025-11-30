#pragma once

#include "Boss.h"
#include <vector>

class AudioManager;
class Player;

/**
 * Projectile
 * Simple projectile fired by the boss
 */
struct Projectile {
    float x, y;
    float vx, vy;  // Velocity
    bool active;

    Projectile(float px, float py, float velocityX, float velocityY)
        : x(px), y(py), vx(velocityX), vy(velocityY), active(true) {}
};

/**
 * The Glitch Boss
 * Appears in Level 6 and Level 8
 *
 * Phase 1 (100-75% HP): Slow movement, shoots slow projectiles
 * Phase 2 (75-50% HP):  Faster movement, teleportation, faster projectiles
 * Phase 3 (50-0% HP):   Aggressive, screen corruption, very fast attacks
 */
class GlitchBoss : public Boss {
public:
    GlitchBoss(float x, float y, AudioManager* audioManager = nullptr);
    ~GlitchBoss() override;

    void update(float deltaTime, Player* player) override;
    void render(SDL_Renderer* renderer) override;

    // Override phase transitions
    void transitionToPhase(BossPhase newPhase) override;

    // Getters
    const std::vector<Projectile>& getProjectiles() const { return m_projectiles; }
    std::string getName() const override { return "THE GLITCH"; }

private:
    // Movement
    float m_targetX, m_targetY;
    float m_speed;

    // Attack
    float m_attackTimer;
    float m_attackCooldown;  // Changes per phase
    std::vector<Projectile> m_projectiles;

    // Teleport
    float m_teleportTimer;
    float m_teleportCooldown;
    bool m_isTeleporting;
    float m_teleportAnimTimer;

    // Visual
    float m_glitchTimer;
    int m_glitchOffsetX, m_glitchOffsetY;

    // AI
    void updatePhase1(float deltaTime, Player* player);
    void updatePhase2(float deltaTime, Player* player);
    void updatePhase3(float deltaTime, Player* player);

    void shootProjectile(Player* player, float speedMultiplier = 1.0f);
    void teleportToRandomPosition();
    void updateProjectiles(float deltaTime);
};
