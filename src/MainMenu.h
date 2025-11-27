#pragma once

#include <SDL2/SDL.h>
#include "UIHelper.h"
#include <vector>

/**
 * MainMenu
 * Hauptmenü des Spiels
 */
class MainMenu {
public:
    enum class MenuResult {
        NONE,
        START_GAME,
        SETTINGS,
        QUIT
    };

    MainMenu();
    ~MainMenu();

    void handleInput(SDL_Event& event);
    MenuResult update(float deltaTime);
    void render(SDL_Renderer* renderer);

private:
    std::vector<UIHelper::Button> m_buttons;
    MenuResult m_result;

    // Visual effects
    float m_glitchTimer;
    bool m_shouldGlitch;

    // Mouse state
    int m_mouseX, m_mouseY;
    bool m_mouseClicked;
};
