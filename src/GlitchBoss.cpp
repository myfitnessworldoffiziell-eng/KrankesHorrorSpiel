#include "GlitchBoss.h"
#include "Player.h"
#include "AudioManager.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

GlitchBoss::GlitchBoss(float x, float y, AudioManager* audioManager)
    : Boss(x, y, 300, audioManager)  // 300 HP
    , m_targetX(x)
    , m_targetY(y)
    , m_speed(50.0f)
    , m_attackTimer(0.0f)
    , m_attackCooldown(2.0f)  // 2 seconds between attacks
    , m_teleportTimer(0.0f)
    , m_teleportCooldown(5.0f)
    , m_isTeleporting(false)
    , m_teleportAnimTimer(0.0f)
    , m_glitchTimer(0.0f)
    , m_glitchOffsetX(0)
    , m_glitchOffsetY(0)
{
    m_width = 48.0f;
    m_height = 48.0f;

    std::cout << "[GlitchBoss] Spawned at (" << x << ", " << y << ")" << std::endl;
}

GlitchBoss::~GlitchBoss() {
    std::cout << "[GlitchBoss] Destroyed" << std::endl;
}

void GlitchBoss::update(float deltaTime, Player* player) {
    if (!isAlive() || !player) return;

    // Update invincibility
    updateInvincibility(deltaTime);

    // Update timers
    m_attackTimer += deltaTime;
    m_teleportTimer += deltaTime;
    m_glitchTimer += deltaTime;

    // Glitch effect (random offset)
    if (m_glitchTimer > 0.1f) {
        m_glitchOffsetX = (rand() % 7) - 3;  // -3 to +3
        m_glitchOffsetY = (rand() % 7) - 3;
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
        default:
            break;
    }

    // Update projectiles
    updateProjectiles(deltaTime);

    // Check for phase transitions
    checkPhaseTransition();
}

void GlitchBoss::updatePhase1(float deltaTime, Player* player) {
    // Phase 1: Slow movement, slow projectiles
    m_speed = 50.0f;
    m_attackCooldown = 2.0f;

    // Move towards player slowly
    float dx = player->getX() - m_x;
    float dy = player->getY() - m_y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 150.0f) {
        // Move closer
        m_x += (dx / distance) * m_speed * deltaTime;
        m_y += (dy / distance) * m_speed * deltaTime;
    }

    // Attack
    if (m_attackTimer >= m_attackCooldown) {
        shootProjectile(player, 1.0f);
        m_attackTimer = 0.0f;
    }
}

void GlitchBoss::updatePhase2(float deltaTime, Player* player) {
    // Phase 2: Faster movement, teleportation, faster projectiles
    m_speed = 80.0f;
    m_attackCooldown = 1.5f;

    // Teleport occasionally
    if (m_teleportTimer >= m_teleportCooldown && !m_isTeleporting) {
        teleportToRandomPosition();
        m_teleportTimer = 0.0f;
    }

    // Movement (hover pattern)
    m_x += std::sin(m_glitchTimer * 2.0f) * m_speed * deltaTime;

    // Attack
    if (m_attackTimer >= m_attackCooldown) {
        shootProjectile(player, 1.5f);  // Faster projectiles
        m_attackTimer = 0.0f;
    }
}

void GlitchBoss::updatePhase3(float deltaTime, Player* player) {
    // Phase 3: Aggressive, very fast attacks
    m_speed = 120.0f;
    m_attackCooldown = 1.0f;

    // Erratic movement
    m_x += std::sin(m_glitchTimer * 5.0f) * m_speed * deltaTime;
    m_y += std::cos(m_glitchTimer * 3.0f) * m_speed * 0.5f * deltaTime;

    // Keep in bounds
    if (m_x < 50.0f) m_x = 50.0f;
    if (m_x > 750.0f) m_x = 750.0f;
    if (m_y < 50.0f) m_y = 50.0f;
    if (m_y > 400.0f) m_y = 400.0f;

    // Rapid fire attacks
    if (m_attackTimer >= m_attackCooldown) {
        shootProjectile(player, 2.0f);  // Very fast projectiles
        m_attackTimer = 0.0f;
    }

    // Teleport more frequently
    if (m_teleportTimer >= 3.0f && !m_isTeleporting) {
        teleportToRandomPosition();
        m_teleportTimer = 0.0f;
    }
}

void GlitchBoss::shootProjectile(Player* player, float speedMultiplier) {
    if (!player) return;

    // Calculate direction to player
    float dx = player->getX() - m_x;
    float dy = player->getY() - m_y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 0) {
        float speed = 150.0f * speedMultiplier;
        float vx = (dx / distance) * speed;
        float vy = (dy / distance) * speed;

        m_projectiles.emplace_back(m_x, m_y, vx, vy);

        if (m_audioManager) {
            m_audioManager->playSound("zap", 80);
        }

        std::cout << "[GlitchBoss] Fired projectile!" << std::endl;
    }
}

void GlitchBoss::teleportToRandomPosition() {
    m_isTeleporting = true;
    m_teleportAnimTimer = 0.3f;  // 0.3 second teleport animation

    // Random position
    m_x = 100.0f + (rand() % 600);
    m_y = 100.0f + (rand() % 300);

    if (m_audioManager) {
        m_audioManager->playSound("zap", 100);
    }

    std::cout << "[GlitchBoss] Teleported to (" << m_x << ", " << m_y << ")" << std::endl;
}

void GlitchBoss::updateProjectiles(float deltaTime) {
    for (auto& proj : m_projectiles) {
        if (!proj.active) continue;

        // Move projectile
        proj.x += proj.vx * deltaTime;
        proj.y += proj.vy * deltaTime;

        // Deactivate if out of bounds
        if (proj.x < 0 || proj.x > 800 || proj.y < 0 || proj.y > 600) {
            proj.active = false;
        }
    }

    // Remove inactive projectiles
    m_projectiles.erase(
        std::remove_if(m_projectiles.begin(), m_projectiles.end(),
            [](const Projectile& p) { return !p.active; }),
        m_projectiles.end()
    );
}

void GlitchBoss::transitionToPhase(BossPhase newPhase) {
    Boss::transitionToPhase(newPhase);

    // Phase-specific dialogue and effects
    switch (newPhase) {
        case BossPhase::PHASE_1:
            m_currentDialogue = "You dare challenge ME?";
            break;
        case BossPhase::PHASE_2:
            m_currentDialogue = "I am becoming... unstable...";
            m_teleportCooldown = 4.0f;  // Teleport more often
            break;
        case BossPhase::PHASE_3:
            m_currentDialogue = "REALITY IS BREAKING!";
            break;
        case BossPhase::DEFEATED:
            m_currentDialogue = "Impossible... you... won...";
            m_projectiles.clear();
            break;
        default:
            break;
    }

    std::cout << "[GlitchBoss] Phase transition: " << static_cast<int>(newPhase) << std::endl;
}

void GlitchBoss::render(SDL_Renderer* renderer) {
    if (!isAlive()) return;

    int renderX = static_cast<int>(m_x + m_glitchOffsetX);
    int renderY = static_cast<int>(m_y + m_glitchOffsetY);

    // Boss body (glitchy cube)
    SDL_Rect bossRect = {
        renderX - static_cast<int>(m_width / 2),
        renderY - static_cast<int>(m_height / 2),
        static_cast<int>(m_width),
        static_cast<int>(m_height)
    };

    // Color based on phase
    Uint8 r = 255, g = 0, b = 255;  // Magenta by default
    if (m_currentPhase == BossPhase::PHASE_2) {
        r = 255; g = 100; b = 255;  // Lighter magenta
    } else if (m_currentPhase == BossPhase::PHASE_3) {
        r = 255; g = 255; b = 0;  // Yellow (unstable)
    }

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderFillRect(renderer, &bossRect);

    // Glitch overlay (random rectangles)
    for (int i = 0; i < 3; i++) {
        int offsetX = (rand() % 20) - 10;
        int offsetY = (rand() % 20) - 10;
        SDL_Rect glitchRect = {
            bossRect.x + offsetX,
            bossRect.y + offsetY,
            bossRect.w / 2,
            bossRect.h / 2
        };
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 150);  // Cyan, transparent
        SDL_RenderFillRect(renderer, &glitchRect);
    }

    // Border
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &bossRect);

    // Render projectiles
    for (const auto& proj : m_projectiles) {
        if (!proj.active) continue;

        SDL_Rect projRect = {
            static_cast<int>(proj.x) - 5,
            static_cast<int>(proj.y) - 5,
            10,
            10
        };

        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);  // Magenta
        SDL_RenderFillRect(renderer, &projRect);
    }
}
