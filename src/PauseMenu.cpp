#include "PauseMenu.h"
#include <iostream>

PauseMenu::PauseMenu()
    : m_result(PauseResult::NONE)
    , m_mouseX(0)
    , m_mouseY(0)
    , m_mouseClicked(false)
{
    // Create buttons
    UIHelper::Button resumeButton;
    resumeButton.rect = {300, 220, 200, 50};
    resumeButton.text = "RESUME";
    m_buttons.push_back(resumeButton);

    UIHelper::Button mainMenuButton;
    mainMenuButton.rect = {300, 290, 200, 50};
    mainMenuButton.text = "MAIN MENU";
    m_buttons.push_back(mainMenuButton);

    UIHelper::Button quitButton;
    quitButton.rect = {300, 360, 200, 50};
    quitButton.text = "QUIT";
    m_buttons.push_back(quitButton);
}

PauseMenu::~PauseMenu() {
}

void PauseMenu::handleInput(SDL_Event& event) {
    if (event.type == SDL_MOUSEMOTION) {
        m_mouseX = event.motion.x;
        m_mouseY = event.motion.y;
    } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        m_mouseClicked = true;
    }
}

PauseMenu::PauseResult PauseMenu::update(float deltaTime) {
    (void)deltaTime;
    m_result = PauseResult::NONE;

    if (UIHelper::renderButton(nullptr, m_buttons[0], m_mouseX, m_mouseY, m_mouseClicked)) {
        std::cout << "[PauseMenu] RESUME clicked" << std::endl;
        m_result = PauseResult::RESUME;
    }

    if (UIHelper::renderButton(nullptr, m_buttons[1], m_mouseX, m_mouseY, m_mouseClicked)) {
        std::cout << "[PauseMenu] MAIN MENU clicked" << std::endl;
        m_result = PauseResult::MAIN_MENU;
    }

    if (UIHelper::renderButton(nullptr, m_buttons[2], m_mouseX, m_mouseY, m_mouseClicked)) {
        std::cout << "[PauseMenu] QUIT clicked" << std::endl;
        m_result = PauseResult::QUIT;
    }

    m_mouseClicked = false;
    return m_result;
}

void PauseMenu::render(SDL_Renderer* renderer) {
    // Semi-transparent overlay
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect fullScreen = {0, 0, 800, 600};
    SDL_RenderFillRect(renderer, &fullScreen);

    // Title
    SDL_Color titleColor = {255, 255, 255, 255};
    UIHelper::renderText(renderer, "PAUSED", 400, 140, titleColor, 36, true);

    // Buttons
    for (auto& button : m_buttons) {
        UIHelper::renderButton(renderer, button, m_mouseX, m_mouseY, false);
    }
}
