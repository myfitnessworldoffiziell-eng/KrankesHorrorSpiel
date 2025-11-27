#include "Player.h"
#include <SDL2/SDL.h>

Player::Player()
    : m_x(100.0f)
    , m_y(400.0f)
    , m_velocityX(0.0f)
    , m_velocityY(0.0f)
    , m_width(32.0f)
    , m_height(32.0f)
    , m_onGround(false)
{
}

Player::~Player() {
}

void Player::update(float deltaTime) {
    // Simple Platformer Physics (Placeholder)
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
    if ((keys[SDL_SCANCODE_SPACE] || keys[SDL_SCANCODE_UP]) && m_onGround) {
        m_velocityY = -400.0f;
        m_onGround = false;
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
}

void Player::render(SDL_Renderer* renderer) {
    // Einfaches Rechteck als Placeholder für Echo
    SDL_Rect rect;
    rect.x = static_cast<int>(m_x);
    rect.y = static_cast<int>(m_y);
    rect.w = static_cast<int>(m_width);
    rect.h = static_cast<int>(m_height);

    // Cyan Farbe (für niedlichen Charakter)
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);

    // Schwarzer Umriss
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
}
