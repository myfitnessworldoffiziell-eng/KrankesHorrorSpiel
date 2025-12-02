#pragma once
#include "SDL_Platform.h"

#include <string>

class AudioManager;

/**
 * Enemy Base Class
 * All enemies inherit from this
 */
class Enemy {
public:
    enum class Type {
        GREEN_SLIME,
        BLUE_BIRD,
        GLITCH_SLIME,
        SPIKE_BALL,
        CORRUPTED_NPC
    };

    Enemy(Type type, float x, float y, AudioManager* audioManager);
    virtual ~Enemy();

    virtual void update(float deltaTime) = 0;
    virtual void render(SDL_Renderer* renderer) = 0;

    // Collision
    SDL_Rect getBounds() const;
    bool isAlive() const { return m_alive; }
    void kill();

    // Getters
    Type getType() const { return m_type; }
    float getX() const { return m_x; }
    float getY() const { return m_y; }
    int getDamage() const { return m_damage; }
    bool canBeJumpedOn() const { return m_canBeJumpedOn; }

protected:
    Type m_type;
    float m_x, m_y;
    float m_width, m_height;
    bool m_alive;
    int m_damage;  // How much damage it deals to player
    bool m_canBeJumpedOn;  // Can player defeat by jumping on it?

    AudioManager* m_audioManager;
};

/**
 * Green Slime - Cute patrol enemy
 */
class GreenSlime : public Enemy {
public:
    GreenSlime(float x, float y, AudioManager* audioManager);

    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    float m_patrolLeft, m_patrolRight;
    float m_velocityX;
    float m_jumpTimer;

    // Animation
    float m_animationTimer;
    float m_squishFactor;
};

/**
 * Blue Bird - Flying enemy
 */
class BlueBird : public Enemy {
public:
    BlueBird(float x, float y, float leftBound, float rightBound, AudioManager* audioManager);

    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    float m_patrolLeft, m_patrolRight;
    float m_velocityX;
    float m_waveOffset;  // For sine wave movement
    float m_chirpTimer;
};

/**
 * Glitch Slime - Corrupted, teleporting enemy
 */
class GlitchSlime : public Enemy {
public:
    GlitchSlime(float x, float y, AudioManager* audioManager);

    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    float m_teleportTimer;
    float m_teleportCooldown;
    bool m_isTeleporting;

    // Visual glitch
    float m_glitchTimer;
    int m_offsetX, m_offsetY;
};

/**
 * Spike Ball - Instant kill hazard
 */
class SpikeBall : public Enemy {
public:
    SpikeBall(float x, float y, AudioManager* audioManager);

    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
    float m_rotation;
};
