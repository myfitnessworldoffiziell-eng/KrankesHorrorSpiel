#include "Level.h"

Level::Level() {
    // Hintergrundfarbe: Helles Blau (freundlich am Anfang)
    m_backgroundColor[0] = 135.0f;
    m_backgroundColor[1] = 206.0f;
    m_backgroundColor[2] = 235.0f;

    // Einfache Platforms erstellen
    m_platforms.push_back({0.0f, 550.0f, 800.0f, 50.0f});      // Boden
    m_platforms.push_back({200.0f, 450.0f, 150.0f, 20.0f});    // Platform 1
    m_platforms.push_back({450.0f, 350.0f, 150.0f, 20.0f});    // Platform 2
    m_platforms.push_back({100.0f, 250.0f, 100.0f, 20.0f});    // Platform 3
}

Level::~Level() {
}

void Level::update(float deltaTime) {
    // TODO: Animationen, bewegende Platforms, etc.
}

void Level::render(SDL_Renderer* renderer) {
    // Hintergrund
    SDL_SetRenderDrawColor(renderer,
        static_cast<int>(m_backgroundColor[0]),
        static_cast<int>(m_backgroundColor[1]),
        static_cast<int>(m_backgroundColor[2]),
        255);
    SDL_RenderClear(renderer);

    // Platforms zeichnen
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // Grau
    for (const auto& platform : m_platforms) {
        SDL_Rect rect;
        rect.x = static_cast<int>(platform.x);
        rect.y = static_cast<int>(platform.y);
        rect.w = static_cast<int>(platform.width);
        rect.h = static_cast<int>(platform.height);

        SDL_RenderFillRect(renderer, &rect);

        // Umriss
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    }
}
