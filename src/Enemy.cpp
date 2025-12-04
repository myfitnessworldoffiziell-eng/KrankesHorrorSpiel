#include "Enemy.h"
#include "AudioManager.h"
#include <cmath>
#include <random>

// ============================================
// BASE ENEMY CLASS
// ============================================

Enemy::Enemy(Type type, float x, float y, AudioManager* audioManager)
    : m_type(type)
    , m_x(x)
    , m_y(y)
    , m_width(32.0f)
    , m_height(32.0f)
    , m_alive(true)
    , m_damage(10)
    , m_canBeJumpedOn(true)
    , m_audioManager(audioManager)
{
}

Enemy::~Enemy() {
}

SDL_Rect Enemy::getBounds() const {
    return {
        static_cast<int>(m_x),
        static_cast<int>(m_y),
        static_cast<int>(m_width),
        static_cast<int>(m_height)
    };
}

void Enemy::kill() {
    m_alive = false;
    // TODO: Death animation/particle effect
}

// ============================================
// GREEN SLIME
// ============================================

GreenSlime::GreenSlime(float x, float y, AudioManager* audioManager)
    : Enemy(Type::GREEN_SLIME, x, y, audioManager)
    , m_patrolLeft(x - 100.0f)
    , m_patrolRight(x + 100.0f)
    , m_velocityX(50.0f)
    , m_jumpTimer(0.0f)
    , m_animationTimer(0.0f)
    , m_squishFactor(1.0f)
{
    m_damage = 10;
    m_canBeJumpedOn = true;
}

void GreenSlime::update(float deltaTime) {
    if (!m_alive) return;

    // Patrol movement
    m_x += m_velocityX * deltaTime;

    // Turn around at patrol bounds
    if (m_x <= m_patrolLeft) {
        m_x = m_patrolLeft;
        m_velocityX = 50.0f;
    } else if (m_x >= m_patrolRight) {
        m_x = m_patrolRight;
        m_velocityX = -50.0f;
    }

    // Occasional jump
    m_jumpTimer += deltaTime;
    if (m_jumpTimer > 3.0f) {
        m_jumpTimer = 0.0f;
        // Small hop animation (visual only, doesn't affect collision)
    }

    // Squish animation
    m_animationTimer += deltaTime * 3.0f;
    m_squishFactor = 1.0f + 0.1f * std::sin(m_animationTimer);
}

void GreenSlime::render(SDL_Renderer* renderer, float cameraX, float cameraY) {
    if (!m_alive) return;

    // Body (green blob with squish effect)
    int squishWidth = static_cast<int>(m_width * m_squishFactor);
    int squishHeight = static_cast<int>(m_height / m_squishFactor);

    SDL_Rect body = {
        static_cast<int>(m_x - cameraX),
        static_cast<int>(m_y - cameraY + (m_height - squishHeight)),
        squishWidth,
        squishHeight
    };

    // Green color
    SDL_SetRenderDrawColor(renderer, 50, 200, 50, 255);
    SDL_RenderFillRect(renderer, &body);

    // Dark outline
    SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
    SDL_RenderDrawRect(renderer, &body);

    // Eyes (two black dots)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect leftEye = {static_cast<int>(m_x - cameraX + 8), static_cast<int>(m_y - cameraY + 10), 4, 4};
    SDL_Rect rightEye = {static_cast<int>(m_x - cameraX + 20), static_cast<int>(m_y - cameraY + 10), 4, 4};
    SDL_RenderFillRect(renderer, &leftEye);
    SDL_RenderFillRect(renderer, &rightEye);

    // Smile (simple line)
    SDL_RenderDrawLine(renderer,
        static_cast<int>(m_x - cameraX + 10), static_cast<int>(m_y - cameraY + 22),
        static_cast<int>(m_x - cameraX + 22), static_cast<int>(m_y - cameraY + 22));
}

// ============================================
// BLUE BIRD
// ============================================

BlueBird::BlueBird(float x, float y, float leftBound, float rightBound, AudioManager* audioManager)
    : Enemy(Type::BLUE_BIRD, x, y, audioManager)
    , m_patrolLeft(leftBound)
    , m_patrolRight(rightBound)
    , m_velocityX(80.0f)
    , m_waveOffset(0.0f)
    , m_chirpTimer(0.0f)
{
    m_damage = 10;
    m_canBeJumpedOn = true;
}

void BlueBird::update(float deltaTime) {
    if (!m_alive) return;

    // Horizontal movement
    m_x += m_velocityX * deltaTime;

    // Turn around
    if (m_x <= m_patrolLeft || m_x >= m_patrolRight) {
        m_velocityX = -m_velocityX;
    }

    // Sine wave vertical movement
    m_waveOffset += deltaTime * 2.0f;
    float baseY = 250.0f;  // Middle of screen
    m_y = baseY + 30.0f * std::sin(m_waveOffset);

    // Chirp sound occasionally
    m_chirpTimer += deltaTime;
    if (m_chirpTimer > 5.0f) {
        if (m_audioManager) {
            m_audioManager->playSound("chirp", 80);
        }
        m_chirpTimer = 0.0f;
    }
}

void BlueBird::render(SDL_Renderer* renderer, float cameraX, float cameraY) {
    if (!m_alive) return;

    // Body (blue oval)
    SDL_SetRenderDrawColor(renderer, 100, 150, 255, 255);
    SDL_Rect body = {
        static_cast<int>(m_x - cameraX),
        static_cast<int>(m_y - cameraY),
        static_cast<int>(m_width),
        static_cast<int>(m_height)
    };
    SDL_RenderFillRect(renderer, &body);

    // Wings (simple triangles - approximated with lines)
    SDL_SetRenderDrawColor(renderer, 70, 120, 200, 255);
    if (m_velocityX > 0) {
        // Facing right
        SDL_RenderDrawLine(renderer, static_cast<int>(m_x - cameraX), static_cast<int>(m_y - cameraY + 10),
                          static_cast<int>(m_x - cameraX - 8), static_cast<int>(m_y - cameraY + 16));
    } else {
        // Facing left
        SDL_RenderDrawLine(renderer, static_cast<int>(m_x - cameraX + 32), static_cast<int>(m_y - cameraY + 10),
                          static_cast<int>(m_x - cameraX + 40), static_cast<int>(m_y - cameraY + 16));
    }

    // Eyes
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect eye = {static_cast<int>(m_x - cameraX + 20), static_cast<int>(m_y - cameraY + 8), 5, 5};
    SDL_RenderFillRect(renderer, &eye);

    // Beak (yellow)
    SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
    SDL_Rect beak = {static_cast<int>(m_x - cameraX + 28), static_cast<int>(m_y - cameraY + 14), 6, 3};
    SDL_RenderFillRect(renderer, &beak);
}

// ============================================
// GLITCH SLIME
// ============================================

GlitchSlime::GlitchSlime(float x, float y, AudioManager* audioManager)
    : Enemy(Type::GLITCH_SLIME, x, y, audioManager)
    , m_teleportTimer(0.0f)
    , m_teleportCooldown(3.0f)
    , m_isTeleporting(false)
    , m_glitchTimer(0.0f)
    , m_offsetX(0)
    , m_offsetY(0)
{
    m_damage = 20;  // Double damage!
    m_canBeJumpedOn = false;  // Can't jump on it easily
}

void GlitchSlime::update(float deltaTime) {
    if (!m_alive) return;

    m_teleportTimer += deltaTime;
    m_glitchTimer += deltaTime;

    // Random pixel offset for glitch effect
    if (m_glitchTimer > 0.05f) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(-3, 3);
        m_offsetX = dis(gen);
        m_offsetY = dis(gen);
        m_glitchTimer = 0.0f;
    }

    // Teleport
    if (m_teleportTimer >= m_teleportCooldown && !m_isTeleporting) {
        m_isTeleporting = true;
        m_teleportTimer = 0.0f;

        // Play ZAP sound
        if (m_audioManager) {
            m_audioManager->playSound("zap", 110);
        }

        // TODO: Screen flash magenta

        // Random new position
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> disX(100, 700);
        std::uniform_int_distribution<> disY(200, 400);

        m_x = static_cast<float>(disX(gen));
        m_y = static_cast<float>(disY(gen));

        m_isTeleporting = false;
    }
}

void GlitchSlime::render(SDL_Renderer* renderer, float cameraX, float cameraY) {
    if (!m_alive) return;

    // === ENHANCED GLITCH SLIME ===
    SDL_Rect body = {
        static_cast<int>(m_x - cameraX + m_offsetX),
        static_cast<int>(m_y - cameraY + m_offsetY),
        static_cast<int>(m_width),
        static_cast<int>(m_height)
    };

    // Glitch aura (pulsing)
    static float glitchPulse = 0.0f;
    glitchPulse += 0.1f;
    int pulseOffset = static_cast<int>(sin(glitchPulse) * 3);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 100);
    SDL_Rect aura = {body.x - 4 + pulseOffset, body.y - 4 + pulseOffset, body.w + 8, body.h + 8};
    SDL_RenderFillRect(renderer, &aura);

    // Main body - gradient effect
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderFillRect(renderer, &body);
    SDL_SetRenderDrawColor(renderer, 200, 0, 200, 255);
    SDL_Rect innerBody = {body.x + 2, body.y + 2, body.w - 4, body.h - 4};
    SDL_RenderFillRect(renderer, &innerBody);

    // Triple outline for intense glitch effect
    SDL_SetRenderDrawColor(renderer, 255, 100, 255, 255);
    SDL_Rect outline1 = {body.x - 1, body.y - 1, body.w + 2, body.h + 2};
    SDL_RenderDrawRect(renderer, &outline1);
    SDL_SetRenderDrawColor(renderer, 200, 0, 200, 255);
    SDL_Rect outline2 = {body.x - 2, body.y - 2, body.w + 4, body.h + 4};
    SDL_RenderDrawRect(renderer, &outline2);
    SDL_SetRenderDrawColor(renderer, 150, 0, 150, 255);
    SDL_Rect outline3 = {body.x - 3, body.y - 3, body.w + 6, body.h + 6};
    SDL_RenderDrawRect(renderer, &outline3);

    // Void eyes with glow (black holes)
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    SDL_SetRenderDrawColor(renderer, 100, 0, 100, 150);
    SDL_Rect leftEyeGlow = {static_cast<int>(m_x - cameraX + 4), static_cast<int>(m_y - cameraY + 6), 10, 10};
    SDL_Rect rightEyeGlow = {static_cast<int>(m_x - cameraX + 18), static_cast<int>(m_y - cameraY + 6), 10, 10};
    SDL_RenderFillRect(renderer, &leftEyeGlow);
    SDL_RenderFillRect(renderer, &rightEyeGlow);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect leftEye = {static_cast<int>(m_x - cameraX + 6), static_cast<int>(m_y - cameraY + 8), 6, 6};
    SDL_Rect rightEye = {static_cast<int>(m_x - cameraX + 20), static_cast<int>(m_y - cameraY + 8), 6, 6};
    SDL_RenderFillRect(renderer, &leftEye);
    SDL_RenderFillRect(renderer, &rightEye);

    // Digital particles around slime
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 200);
    for (int i = 0; i < 5; ++i) {
        int px = static_cast<int>(m_x - cameraX + (rand() % 32));
        int py = static_cast<int>(m_y - cameraY + (rand() % 32));
        SDL_RenderDrawPoint(renderer, px, py);
    }
}

// ============================================
// SPIKE BALL
// ============================================

SpikeBall::SpikeBall(float x, float y, AudioManager* audioManager)
    : Enemy(Type::SPIKE_BALL, x, y, audioManager)
    , m_rotation(0.0f)
{
    m_width = 40.0f;
    m_height = 40.0f;
    m_damage = 999;  // Instant kill!
    m_canBeJumpedOn = false;
}

void SpikeBall::update(float deltaTime) {
    // Rotate
    m_rotation += deltaTime * 30.0f;  // 30 degrees per second
    if (m_rotation >= 360.0f) m_rotation -= 360.0f;
}

void SpikeBall::render(SDL_Renderer* renderer, float cameraX, float cameraY) {
    // Gray metallic ball
    SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);

    int centerX = static_cast<int>(m_x - cameraX + m_width / 2);
    int centerY = static_cast<int>(m_y - cameraY + m_height / 2);
    int radius = static_cast<int>(m_width / 2);

    // Draw circle (approximated)
    SDL_Rect ball = {
        static_cast<int>(m_x - cameraX),
        static_cast<int>(m_y - cameraY),
        static_cast<int>(m_width),
        static_cast<int>(m_height)
    };
    SDL_RenderFillRect(renderer, &ball);

    // Spikes (8 spikes around the ball)
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    for (int i = 0; i < 8; ++i) {
        float angle = (i * 45.0f + m_rotation) * (M_PI / 180.0f);
        int spikeX = centerX + static_cast<int>(radius * std::cos(angle));
        int spikeY = centerY + static_cast<int>(radius * std::sin(angle));

        int spikeEndX = centerX + static_cast<int>((radius + 12) * std::cos(angle));
        int spikeEndY = centerY + static_cast<int>((radius + 12) * std::sin(angle));

        // Draw spike
        SDL_RenderDrawLine(renderer, spikeX, spikeY, spikeEndX, spikeEndY);
        SDL_RenderDrawLine(renderer, spikeX - 1, spikeY, spikeEndX - 1, spikeEndY);
        SDL_RenderDrawLine(renderer, spikeX + 1, spikeY, spikeEndX + 1, spikeEndY);
    }
}
