#pragma once

#include <SDL2/SDL.h>

/**
 * Player (Echo)
 * Simple Platformer Character
 */
class Player {
public:
    Player();
    ~Player();

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

private:
    float m_x, m_y;
    float m_velocityX, m_velocityY;
    float m_width, m_height;

    // State
    bool m_onGround;
};
