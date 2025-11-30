#include "Boss.h"
#include "AudioManager.h"
#include "Player.h"
#include <iostream>

Boss::Boss(float x, float y, int maxHP, AudioManager* audioManager)
    : m_x(x)
    , m_y(y)
    , m_width(64.0f)
    , m_height(64.0f)
    , m_hp(maxHP)
    , m_maxHP(maxHP)
    , m_alive(true)
    , m_currentPhase(BossPhase::PHASE_1)
    , m_audioManager(audioManager)
    , m_name("Boss")
    , m_currentDialogue("")
    , m_invulnerable(false)
    , m_invulnerabilityTimer(0.0f)
    , m_animTimer(0.0f)
{
}

Boss::~Boss() {
}

void Boss::takeDamage(int damage) {
    if (!m_alive || m_invulnerable) return;

    m_hp -= damage;
    if (m_hp < 0) m_hp = 0;

    std::cout << "[Boss] " << m_name << " took " << damage << " damage! HP: " << m_hp << "/" << m_maxHP << std::endl;

    // Set invincibility frames (1 second)
    m_invulnerable = true;
    m_invulnerabilityTimer = 1.0f;

    if (m_audioManager) {
        m_audioManager->playSound("hit", 120);
    }

    if (m_hp <= 0) {
        kill();
    } else {
        // Check for phase transitions
        checkPhaseTransition();
    }
}

void Boss::kill() {
    m_alive = false;
    m_currentPhase = BossPhase::DEFEATED;

    std::cout << "[Boss] " << m_name << " defeated!" << std::endl;

    if (m_audioManager) {
        m_audioManager->playSound("death", 150);
    }
}

void Boss::updateInvincibility(float deltaTime) {
    if (m_invulnerable) {
        m_invulnerabilityTimer -= deltaTime;
        if (m_invulnerabilityTimer <= 0.0f) {
            m_invulnerable = false;
            m_invulnerabilityTimer = 0.0f;
        }
    }
}

void Boss::transitionToPhase(BossPhase newPhase) {
    if (m_currentPhase == newPhase) return;

    std::cout << "[Boss] Transitioning to phase: " << static_cast<int>(newPhase) << std::endl;

    m_currentPhase = newPhase;
    m_invulnerable = true;
    m_invulnerabilityTimer = 2.0f; // Invulnerable during transition

    // Play transition sound
    if (m_audioManager) {
        m_audioManager->playSound("zap", 130);
    }
}

bool Boss::checkPhaseTransition() {
    float hpPercent = (static_cast<float>(m_hp) / static_cast<float>(m_maxHP)) * 100.0f;

    // Standard phase thresholds
    if (hpPercent <= 75.0f && m_currentPhase == BossPhase::PHASE_1) {
        transitionToPhase(BossPhase::PHASE_2);
        return true;
    } else if (hpPercent <= 50.0f && m_currentPhase == BossPhase::PHASE_2) {
        transitionToPhase(BossPhase::PHASE_3);
        return true;
    } else if (hpPercent <= 25.0f && m_currentPhase == BossPhase::PHASE_3) {
        transitionToPhase(BossPhase::PHASE_4);
        return true;
    }

    return false;
}

SDL_Rect Boss::getBounds() const {
    return {
        static_cast<int>(m_x),
        static_cast<int>(m_y),
        static_cast<int>(m_width),
        static_cast<int>(m_height)
    };
}
