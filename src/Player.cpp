#include "Player.h"
#include "AudioManager.h"
#include <SDL2/SDL.h>
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

    // Simple Ground Collision (y = 500)
    if (m_y >= 500.0f) {
        m_y = 500.0f;
        m_velocityY = 0.0f;
        m_onGround = true;
    }

    // Screen Bounds
    if (m_x < 0.0f) m_x = 0.0f;
    if (m_x > 800.0f - m_width) m_x = 800.0f - m_width;

    // Animation
    m_animationTimer += deltaTime;
}

void Player::render(SDL_Renderer* renderer) {
    // Player rectangle (Cyan for Echo)
    SDL_Rect rect;
    rect.x = static_cast<int>(m_x);
    rect.y = static_cast<int>(m_y);
    rect.w = static_cast<int>(m_width);
    rect.h = static_cast<int>(m_height);

    // Blink if invulnerable
    if (m_invulnerable) {
        int blinkPhase = static_cast<int>(m_invulnerabilityTimer * 10) % 2;
        if (blinkPhase == 0) {
            return;  // Don't render (blinking effect)
        }
    }

    // Cyan color (cute Echo)
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);

    // Black outline
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);

    // Simple face (two eyes)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect leftEye = {rect.x + 8, rect.y + 10, 4, 4};
    SDL_Rect rightEye = {rect.x + 20, rect.y + 10, 4, 4};
    SDL_RenderFillRect(renderer, &leftEye);
    SDL_RenderFillRect(renderer, &rightEye);
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

SDL_Rect Player::getBounds() const {
    return {
        static_cast<int>(m_x),
        static_cast<int>(m_y),
        static_cast<int>(m_width),
        static_cast<int>(m_height)
    };
}
