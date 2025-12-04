#include "SDL_Platform.h"
#include "Player.h"
#include "AudioManager.h"
#include <algorithm>

Player::Player()
    : m_x(100.0f)
    , m_y(400.0f)
    , m_velocityX(0.0f)
    , m_velocityY(0.0f)
    , m_width(32.0f)
    , m_height(32.0f)
    , m_health(100)
    , m_maxHealth(100)
    , m_invulnerable(false)
    , m_invulnerabilityTimer(0.0f)
    , m_onGround(false)
    , m_animationTimer(0.0f)
    , m_audioManager(nullptr)
{
}

Player::~Player() {
}

void Player::update(float deltaTime) {
    // Invulnerability timer
    if (m_invulnerable) {
        m_invulnerabilityTimer -= deltaTime;
        if (m_invulnerabilityTimer <= 0.0f) {
            m_invulnerable = false;
        }
    }

    // Simple Platformer Physics
    const Uint8* keys = SDL_GetKeyboardState(NULL);

    // Horizontal Movement
    m_velocityX = 0.0f;
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
        m_velocityX = -200.0f;
    }
    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
        m_velocityX = 200.0f;
    }

    // Jump
    if ((keys[SDL_SCANCODE_SPACE] || keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) && m_onGround) {
        m_velocityY = -400.0f;
        m_onGround = false;

        // Play jump sound
        if (m_audioManager) {
            m_audioManager->playSound("jump", 100);
        }
    }

    // Gravity
    m_velocityY += 800.0f * deltaTime;

    // Update Position
    m_x += m_velocityX * deltaTime;
    m_y += m_velocityY * deltaTime;

    // Reset ground state (will be set by platform collision in Game.cpp)
    m_onGround = false;

    // Level Bounds (levels are 2400px wide)
    if (m_x < 0.0f) m_x = 0.0f;
    if (m_x > 2400.0f - m_width) m_x = 2400.0f - m_width;

    // Fallback: prevent falling off screen
    if (m_y > 600.0f) {
        m_y = 500.0f;
        m_velocityY = 0.0f;
        m_onGround = true;
    }

    // Animation
    m_animationTimer += deltaTime;
}

void Player::render(SDL_Renderer* renderer, float cameraX, float cameraY) {
    // Player rectangle (Cyan for Echo)
    SDL_Rect rect;
    rect.x = static_cast<int>(m_x - cameraX);
    rect.y = static_cast<int>(m_y - cameraY);
    rect.w = static_cast<int>(m_width);
    rect.h = static_cast<int>(m_height);

    // Blink if invulnerable
    if (m_invulnerable) {
        int blinkPhase = static_cast<int>(m_invulnerabilityTimer * 10) % 2;
        if (blinkPhase == 0) {
            return;  // Don't render (blinking effect)
        }
    }

    // ===== COOLER PLAYER DESIGN =====

    // Main body - gradient effect with multiple layers
    SDL_Rect bodyLayer1 = {rect.x + 2, rect.y + 2, rect.w - 4, rect.h - 4};
    SDL_Rect bodyLayer2 = {rect.x + 4, rect.y + 4, rect.w - 8, rect.h - 8};

    // Outer glow (animated)
    int glowPulse = static_cast<int>(m_animTimer * 3.0f) % 2;
    if (glowPulse == 0) {
        SDL_SetRenderDrawColor(renderer, 0, 200, 255, 100);
        SDL_Rect glow = {rect.x - 2, rect.y - 2, rect.w + 4, rect.h + 4};
        SDL_RenderFillRect(renderer, &glow);
    }

    // Main body - cyan with darker gradient
    SDL_SetRenderDrawColor(renderer, 0, 220, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 180, 220, 255);
    SDL_RenderFillRect(renderer, &bodyLayer1);
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_RenderFillRect(renderer, &bodyLayer2);

    // Cool outline with corners
    SDL_SetRenderDrawColor(renderer, 0, 50, 80, 255);
    SDL_RenderDrawRect(renderer, &rect);

    // Corner highlights
    SDL_SetRenderDrawColor(renderer, 100, 255, 255, 255);
    SDL_RenderDrawPoint(renderer, rect.x + 1, rect.y + 1);
    SDL_RenderDrawPoint(renderer, rect.x + rect.w - 2, rect.y + 1);
    SDL_RenderDrawPoint(renderer, rect.x + 1, rect.y + rect.h - 2);
    SDL_RenderDrawPoint(renderer, rect.x + rect.w - 2, rect.y + rect.h - 2);

    // Animated eyes with glow
    int eyeAnim = static_cast<int>(m_animTimer * 10.0f) % 20;
    bool eyeBlink = (eyeAnim == 0);

    if (!eyeBlink) {
        // Glowing eyes
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 200);
        SDL_Rect leftEyeGlow = {rect.x + 7, rect.y + 9, 6, 6};
        SDL_Rect rightEyeGlow = {rect.x + 19, rect.y + 9, 6, 6};
        SDL_RenderFillRect(renderer, &leftEyeGlow);
        SDL_RenderFillRect(renderer, &rightEyeGlow);

        // Eye pupils
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect leftEye = {rect.x + 9, rect.y + 11, 2, 2};
        SDL_Rect rightEye = {rect.x + 21, rect.y + 11, 2, 2};
        SDL_RenderFillRect(renderer, &leftEye);
        SDL_RenderFillRect(renderer, &rightEye);
    } else {
        // Blink - just a line
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(renderer, rect.x + 8, rect.y + 12, rect.x + 12, rect.y + 12);
        SDL_RenderDrawLine(renderer, rect.x + 20, rect.y + 12, rect.x + 24, rect.y + 12);
    }

    // Mouth - changes based on movement
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    if (m_velocityY < -50.0f) {
        // Surprised mouth (jumping)
        SDL_Rect mouth = {rect.x + 14, rect.y + 22, 4, 5};
        SDL_RenderFillRect(renderer, &mouth);
    } else if (fabs(m_velocityX) > 50.0f) {
        // Happy mouth (moving)
        for (int i = 0; i < 6; ++i) {
            SDL_RenderDrawPoint(renderer, rect.x + 12 + i, rect.y + 22 + (i < 3 ? 0 : -1));
        }
    } else {
        // Normal mouth
        SDL_RenderDrawLine(renderer, rect.x + 12, rect.y + 22, rect.x + 18, rect.y + 22);
    }

    // Digital particles around player (moving effect)
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 150);
    for (int i = 0; i < 3; ++i) {
        int particleOffset = static_cast<int>(m_animTimer * 5.0f + i * 2.0f) % 8;
        SDL_RenderDrawPoint(renderer, rect.x - 4 + particleOffset, rect.y + 5 + i * 10);
        SDL_RenderDrawPoint(renderer, rect.x + rect.w + 4 - particleOffset, rect.y + 8 + i * 8);
    }
}

void Player::takeDamage(int damage) {
    if (m_invulnerable || m_health <= 0) return;

    m_health -= damage;
    if (m_health < 0) m_health = 0;

    // Play damage sound
    if (m_audioManager) {
        if (m_health <= 0) {
            m_audioManager->playSound("death", 120);
        } else {
            m_audioManager->playSound("hit", 100);
        }
    }

    // Invulnerability for 1 second
    m_invulnerable = true;
    m_invulnerabilityTimer = 1.0f;

    // Knockback (simple)
    m_velocityX = (m_velocityX > 0) ? -100.0f : 100.0f;
    m_velocityY = -200.0f;
}

void Player::kill() {
    m_health = 0;
    if (m_audioManager) {
        m_audioManager->playSound("death", 120);
    }
}

void Player::reset() {
    // Reset health to maximum
    m_health = m_maxHealth;

    // Reset velocities
    m_velocityX = 0.0f;
    m_velocityY = 0.0f;

    // Reset state flags
    m_onGround = false;
    m_invulnerable = false;
    m_invulnerabilityTimer = 0.0f;
    m_animationTimer = 0.0f;
}

SDL_Rect Player::getBounds() const {
    return {
        static_cast<int>(m_x),
        static_cast<int>(m_y),
        static_cast<int>(m_width),
        static_cast<int>(m_height)
    };
}
