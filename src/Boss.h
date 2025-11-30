#pragma once

#include <SDL2/SDL.h>
#include <string>

class AudioManager;
class Player;

/**
 * Boss Phase
 */
enum class BossPhase {
    INTRO,      // Boss introduction/dialogue
    PHASE_1,    // First phase
    PHASE_2,    // Second phase
    PHASE_3,    // Third phase
    PHASE_4,    // Fourth phase (optional)
    DEFEATED    // Boss defeated
};

/**
 * Boss
 * Base class for all boss enemies
 */
class Boss {
public:
    Boss(float x, float y, int maxHP, AudioManager* audioManager = nullptr);
    virtual ~Boss();

    // Core functions (must be overridden)
    virtual void update(float deltaTime, Player* player) = 0;
    virtual void render(SDL_Renderer* renderer) = 0;

    // Boss-specific
    virtual void takeDamage(int damage);
    virtual void kill();
    bool isAlive() const { return m_alive; }
    bool isDefeated() const { return m_currentPhase == BossPhase::DEFEATED; }
    bool isInvulnerable() const { return m_invulnerable; }
    void updateInvincibility(float deltaTime);

    // Phase management
    BossPhase getCurrentPhase() const { return m_currentPhase; }
    virtual void transitionToPhase(BossPhase newPhase);

    // Getters
    SDL_Rect getBounds() const;
    int getHP() const { return m_hp; }
    int getMaxHP() const { return m_maxHP; }
    float getX() const { return m_x; }
    float getY() const { return m_y; }
    virtual std::string getName() const { return m_name; }
    BossPhase getPhase() const { return m_currentPhase; }

    // Dialogue
    bool hasDialogue() const { return !m_currentDialogue.empty(); }
    std::string getCurrentDialogue() const { return m_currentDialogue; }
    void clearDialogue() { m_currentDialogue = ""; }

protected:
    // Position & size
    float m_x, m_y;
    float m_width, m_height;

    // Stats
    int m_hp;
    int m_maxHP;
    bool m_alive;
    BossPhase m_currentPhase;

    // Audio
    AudioManager* m_audioManager;

    // Visual
    std::string m_name;
    std::string m_currentDialogue;

    // Invulnerability (during transitions)
    bool m_invulnerable;
    float m_invulnerabilityTimer;

    // Animation
    float m_animTimer;

    // Helper functions for derived classes
    void setDialogue(const std::string& dialogue) { m_currentDialogue = dialogue; }
    bool checkPhaseTransition();  // Check HP thresholds for phase changes
};
