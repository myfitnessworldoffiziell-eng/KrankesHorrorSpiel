#include "EchoPrime.h"
#include "Player.h"
#include "AudioManager.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

EchoPrime::EchoPrime(float x, float y, AudioManager* audioManager)
    : Boss(x, y, 500, audioManager)  // 500 HP (final boss)
    , m_targetX(x)
    , m_targetY(y)
    , m_speed(60.0f)
    , m_hoverOffset(0.0f)
    , m_attackTimer(0.0f)
    , m_attackCooldown(3.0f)
    , m_cloneSpawnTimer(0.0f)
    , m_isCodeForm(false)
    , m_animTimer(0.0f)
    , m_glitchTimer(0.0f)
    , m_glitchOffsetX(0)
    , m_glitchOffsetY(0)
{
    m_width = 56.0f;
    m_height = 56.0f;

    std::cout << "[EchoPrime] FINAL BOSS SPAWNED at (" << x << ", " << y << ")" << std::endl;
}

EchoPrime::~EchoPrime() {
    std::cout << "[EchoPrime] Destroyed" << std::endl;
}

void EchoPrime::update(float deltaTime, Player* player) {
    if (!isAlive() || !player) return;

    // Update invincibility
    updateInvincibility(deltaTime);

    // Update timers
    m_attackTimer += deltaTime;
    m_animTimer += deltaTime;
    m_glitchTimer += deltaTime;
    m_cloneSpawnTimer += deltaTime;

    // Hovering effect
    m_hoverOffset = std::sin(m_animTimer * 2.0f) * 10.0f;

    // Glitch effect (gets worse in later phases)
    if (m_glitchTimer > 0.1f) {
        int intensity = static_cast<int>(m_currentPhase);
        m_glitchOffsetX = (rand() % (intensity * 4)) - (intensity * 2);
        m_glitchOffsetY = (rand() % (intensity * 4)) - (intensity * 2);
        m_glitchTimer = 0.0f;
    }

    // Phase-specific behavior
    switch (m_currentPhase) {
        case BossPhase::PHASE_1:
            updatePhase1(deltaTime, player);
            break;
        case BossPhase::PHASE_2:
            updatePhase2(deltaTime, player);
            break;
        case BossPhase::PHASE_3:
            updatePhase3(deltaTime, player);
            break;
        case BossPhase::PHASE_4:
            updatePhase4(deltaTime, player);
            break;
        default:
            break;
    }

    // Update clones
    if (!m_clones.empty()) {
        updateClones(deltaTime, player);
    }

    // Update code particles (Phase 4)
    if (m_isCodeForm) {
        updateCodeParticles(deltaTime);
    }

    // Check for phase transitions
    checkPhaseTransition();
}

void EchoPrime::updatePhase1(float deltaTime, Player* player) {
    // Phase 1: Tutorial Echo - Friendly, slow
    m_speed = 60.0f;
    m_attackCooldown = 3.0f;

    // Slow hover pattern
    m_x = 400.0f + std::sin(m_animTimer * 0.5f) * 100.0f;
    m_y = 200.0f + m_hoverOffset;

    // No actual attacks in this phase (tutorial)
}

void EchoPrime::updatePhase2(float deltaTime, Player* player) {
    // Phase 2: Glitched Echo - Aggressive movement
    m_speed = 100.0f;
    m_attackCooldown = 2.0f;

    // Move towards player
    float dx = player->getX() - m_x;
    float dy = player->getY() - m_y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 120.0f) {
        m_x += (dx / distance) * m_speed * deltaTime;
        m_y += (dy / distance) * m_speed * deltaTime;
    }

    // Dash attack
    if (m_attackTimer >= m_attackCooldown) {
        // Quick dash towards player
        m_x += (dx / distance) * 200.0f * deltaTime;
        m_y += (dy / distance) * 200.0f * deltaTime;

        if (m_audioManager) {
            m_audioManager->playSound("zap", 90);
        }

        m_attackTimer = 0.0f;
    }
}

void EchoPrime::updatePhase3(float deltaTime, Player* player) {
    // Phase 3: Multiple Echoes - Spawn clones
    m_speed = 80.0f;
    m_attackCooldown = 2.5f;

    // Hover in center
    m_x = 400.0f + std::sin(m_animTimer) * 50.0f;
    m_y = 200.0f + m_hoverOffset;

    // Spawn clones
    if (m_cloneSpawnTimer >= 4.0f && m_clones.size() < 3) {
        spawnClone();
        m_cloneSpawnTimer = 0.0f;
    }
}

void EchoPrime::updatePhase4(float deltaTime, Player* player) {
    // Phase 4: Pure Code Form - BULLET HELL
    m_isCodeForm = true;
    m_speed = 150.0f;
    m_attackCooldown = 0.5f;  // Very fast attacks

    // Chaotic movement
    m_x += std::sin(m_animTimer * 3.0f) * m_speed * deltaTime;
    m_y += std::cos(m_animTimer * 2.0f) * m_speed * deltaTime;

    // Keep in bounds
    if (m_x < 100.0f) m_x = 100.0f;
    if (m_x > 700.0f) m_x = 700.0f;
    if (m_y < 100.0f) m_y = 100.0f;
    if (m_y > 400.0f) m_y = 400.0f;

    // Emit code particles constantly
    emitCodeParticles();
}

void EchoPrime::spawnClone() {
    float angle = (m_clones.size() * 120.0f) * (3.14159f / 180.0f);  // 120 degrees apart
    float distance = 150.0f;

    float cloneX = m_x + std::cos(angle) * distance;
    float cloneY = m_y + std::sin(angle) * distance;

    m_clones.emplace_back(cloneX, cloneY);

    if (m_audioManager) {
        m_audioManager->playSound("scare_appear", 100);
    }

    std::cout << "[EchoPrime] Spawned clone #" << m_clones.size() << std::endl;
}

void EchoPrime::updateClones(float deltaTime, Player* player) {
    for (auto& clone : m_clones) {
        if (!clone.active) continue;

        clone.animTimer += deltaTime;

        // Clones orbit around the player
        float angle = clone.animTimer * 2.0f;
        float radius = 100.0f;

        clone.x = player->getX() + std::cos(angle) * radius;
        clone.y = player->getY() + std::sin(angle) * radius;

        // Keep in bounds
        if (clone.x < 50.0f) clone.x = 50.0f;
        if (clone.x > 750.0f) clone.x = 750.0f;
        if (clone.y < 50.0f) clone.y = 50.0f;
        if (clone.y > 550.0f) clone.y = 550.0f;
    }
}

void EchoPrime::updateCodeParticles(float deltaTime) {
    for (auto& particle : m_codeParticles) {
        particle.x += particle.vx * deltaTime;
        particle.y += particle.vy * deltaTime;
        particle.lifetime -= deltaTime;
    }

    // Remove dead particles
    m_codeParticles.erase(
        std::remove_if(m_codeParticles.begin(), m_codeParticles.end(),
            [](const CodeParticle& p) { return p.lifetime <= 0.0f; }),
        m_codeParticles.end()
    );
}

void EchoPrime::emitCodeParticles() {
    if (m_codeParticles.size() > 100) return;  // Limit particles

    const char codeChars[] = "01ECHO";
    for (int i = 0; i < 3; i++) {
        char c = codeChars[rand() % 6];
        m_codeParticles.emplace_back(m_x, m_y, c);
    }
}

void EchoPrime::transitionToPhase(BossPhase newPhase) {
    Boss::transitionToPhase(newPhase);

    // Phase-specific dialogue and effects
    switch (newPhase) {
        case BossPhase::PHASE_1:
            m_currentDialogue = "Hello, player. Welcome to the end.";
            break;
        case BossPhase::PHASE_2:
            m_currentDialogue = "You're hurting me... Why?";
            if (m_audioManager) {
                m_audioManager->playSound("scare_audio", 100);
            }
            break;
        case BossPhase::PHASE_3:
            m_currentDialogue = "I am everywhere. I am everyone.";
            break;
        case BossPhase::PHASE_4:
            m_currentDialogue = "I AM THE CODE. I AM ETERNAL.";
            m_isCodeForm = true;
            if (m_audioManager) {
                m_audioManager->playSound("whitenoise", 50);
            }
            break;
        case BossPhase::DEFEATED:
            m_currentDialogue = "Maybe... I can finally... rest...";
            m_clones.clear();
            m_codeParticles.clear();
            break;
        default:
            break;
    }

    std::cout << "[EchoPrime] Phase transition: " << static_cast<int>(newPhase)
              << " - \"" << m_currentDialogue << "\"" << std::endl;
}

void EchoPrime::render(SDL_Renderer* renderer) {
    if (!isAlive()) return;

    // Choose rendering based on phase
    if (m_isCodeForm) {
        renderCodeForm(renderer);
    } else if (m_currentPhase >= BossPhase::PHASE_2) {
        renderGlitchedEcho(renderer);
    } else {
        renderFriendlyEcho(renderer);
    }

    // Render clones
    for (const auto& clone : m_clones) {
        if (!clone.active) continue;

        SDL_Rect cloneRect = {
            static_cast<int>(clone.x) - 20,
            static_cast<int>(clone.y) - 20,
            40,
            40
        };

        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 200);  // Semi-transparent blue
        SDL_RenderFillRect(renderer, &cloneRect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &cloneRect);
    }
}

void EchoPrime::renderFriendlyEcho(SDL_Renderer* renderer) {
    int renderX = static_cast<int>(m_x);
    int renderY = static_cast<int>(m_y + m_hoverOffset);

    // Simple friendly cube (blue)
    SDL_Rect echoRect = {
        renderX - static_cast<int>(m_width / 2),
        renderY - static_cast<int>(m_height / 2),
        static_cast<int>(m_width),
        static_cast<int>(m_height)
    };

    SDL_SetRenderDrawColor(renderer, 100, 150, 255, 255);  // Light blue
    SDL_RenderFillRect(renderer, &echoRect);

    // Border
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &echoRect);

    // "Eyes" (two small squares)
    SDL_Rect eye1 = {renderX - 15, renderY - 10, 10, 10};
    SDL_Rect eye2 = {renderX + 5, renderY - 10, 10, 10};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &eye1);
    SDL_RenderFillRect(renderer, &eye2);
}

void EchoPrime::renderGlitchedEcho(SDL_Renderer* renderer) {
    int renderX = static_cast<int>(m_x + m_glitchOffsetX);
    int renderY = static_cast<int>(m_y + m_hoverOffset + m_glitchOffsetY);

    // Glitched cube (red/magenta)
    SDL_Rect echoRect = {
        renderX - static_cast<int>(m_width / 2),
        renderY - static_cast<int>(m_height / 2),
        static_cast<int>(m_width),
        static_cast<int>(m_height)
    };

    SDL_SetRenderDrawColor(renderer, 255, 50, 150, 255);  // Magenta-ish
    SDL_RenderFillRect(renderer, &echoRect);

    // Glitch layers
    for (int i = 0; i < 5; i++) {
        int offX = (rand() % 20) - 10;
        int offY = (rand() % 20) - 10;
        SDL_Rect glitchRect = {
            echoRect.x + offX,
            echoRect.y + offY,
            echoRect.w / 2,
            echoRect.h / 2
        };
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 100);
        SDL_RenderFillRect(renderer, &glitchRect);
    }

    // Border
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &echoRect);
}

void EchoPrime::renderCodeForm(SDL_Renderer* renderer) {
    int renderX = static_cast<int>(m_x);
    int renderY = static_cast<int>(m_y);

    // Pure code (just an outline, transparent)
    SDL_Rect echoRect = {
        renderX - static_cast<int>(m_width / 2),
        renderY - static_cast<int>(m_height / 2),
        static_cast<int>(m_width),
        static_cast<int>(m_height)
    };

    // Pulsing outline
    static float pulseTimer = 0.0f;
    pulseTimer += 0.016f;
    int pulse = static_cast<int>(std::sin(pulseTimer * 5.0f) * 50.0f + 205.0f);

    SDL_SetRenderDrawColor(renderer, pulse, pulse, 255, 255);
    SDL_RenderDrawRect(renderer, &echoRect);

    // Render code particles
    for (const auto& particle : m_codeParticles) {
        // Just render as a small pixel (we can't render text easily here)
        SDL_Rect particleRect = {
            static_cast<int>(particle.x),
            static_cast<int>(particle.y),
            2, 2
        };
        int alpha = static_cast<int>(particle.lifetime * 127.5f);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, alpha);  // Green code
        SDL_RenderFillRect(renderer, &particleRect);
    }
}
