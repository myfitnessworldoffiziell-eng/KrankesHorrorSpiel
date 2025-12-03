#pragma once
#include "SDL_Platform.h"


class AudioManager;
class Enemy;

/**
 * Player (Echo)
 * The main character - starts cute, becomes... something else
 */
class Player {
public:
    Player();
    ~Player();

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    // Health & Death
    void takeDamage(int damage);
    void kill();  // Instant death (spike ball, etc.)
    bool isAlive() const { return m_health > 0; }
    int getHealth() const { return m_health; }
    int getMaxHealth() const { return m_maxHealth; }

    // Position
    float getX() const { return m_x; }
    float getY() const { return m_y; }
    void setPosition(float x, float y) { m_x = x; m_y = y; }

    // Velocity (for boss jumping)
    float getVelocityY() const { return m_velocityY; }
    void setVelocityY(float vy) { m_velocityY = vy; }

    // Position setters (for platform collision)
    void setY(float y) { m_y = y; }
    void setOnGround(bool onGround) { m_onGround = onGround; }

    // Collision
    SDL_Rect getBounds() const;

    // Audio
    void setAudioManager(AudioManager* audio) { m_audioManager = audio; }

private:
    float m_x, m_y;
    float m_velocityX, m_velocityY;
    float m_width, m_height;

    // Health
    int m_health;
    int m_maxHealth;
    bool m_invulnerable;  // After taking damage
    float m_invulnerabilityTimer;

    // State
    bool m_onGround;

    // Animation
    float m_animationTimer;

    AudioManager* m_audioManager;
};
