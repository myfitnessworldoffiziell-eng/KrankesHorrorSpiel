#pragma once
#include "SDL_Platform.h"

#include "UIHelper.h"
#include <vector>

/**
 * PauseMenu
 * Overlay-Menü während Pause
 */
class PauseMenu {
public:
    enum class PauseResult {
        NONE,
        RESUME,
        SETTINGS,
        MAIN_MENU,
        QUIT
    };

    PauseMenu();
    ~PauseMenu();

    void handleInput(SDL_Event& event);
    PauseResult update(float deltaTime);
    void render(SDL_Renderer* renderer);

private:
    std::vector<UIHelper::Button> m_buttons;
    PauseResult m_result;

    int m_mouseX, m_mouseY;
    bool m_mouseClicked;
};
