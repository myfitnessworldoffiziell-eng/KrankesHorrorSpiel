#pragma once

#include <SDL2/SDL.h>
#include <vector>

/**
 * Level
 * Simple Level mit Platforms
 */
class Level {
public:
    Level();
    ~Level();

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

private:
    struct Platform {
        float x, y, width, height;
    };

    std::vector<Platform> m_platforms;
    float m_backgroundColor[3]; // RGB
};
