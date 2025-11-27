#include "MainMenu.h"
#include <iostream>

MainMenu::MainMenu()
    : m_result(MenuResult::NONE)
    , m_glitchTimer(0.0f)
    , m_shouldGlitch(false)
    , m_mouseX(0)
    , m_mouseY(0)
    , m_mouseClicked(false)
{
    // Create buttons
    UIHelper::Button startButton;
    startButton.rect = {300, 250, 200, 50};
    startButton.text = "START GAME";
    startButton.hovered = false;
    startButton.clicked = false;
    m_buttons.push_back(startButton);

    UIHelper::Button settingsButton;
    settingsButton.rect = {300, 320, 200, 50};
    settingsButton.text = "SETTINGS";
    settingsButton.hovered = false;
    settingsButton.clicked = false;
    m_buttons.push_back(settingsButton);

    UIHelper::Button quitButton;
    quitButton.rect = {300, 390, 200, 50};
    quitButton.text = "QUIT";
    quitButton.hovered = false;
    quitButton.clicked = false;
    m_buttons.push_back(quitButton);
}

MainMenu::~MainMenu() {
}

void MainMenu::handleInput(SDL_Event& event) {
    if (event.type == SDL_MOUSEMOTION) {
        m_mouseX = event.motion.x;
        m_mouseY = event.motion.y;
    } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        m_mouseClicked = true;
    }
}

MainMenu::MenuResult MainMenu::update(float deltaTime) {
    m_result = MenuResult::NONE;

    // Glitch effect timer (random glitches in menu)
    m_glitchTimer += deltaTime;
    if (m_glitchTimer > 5.0f) {
        m_shouldGlitch = true;
        m_glitchTimer = 0.0f;
    } else {
        m_shouldGlitch = false;
    }

    // Update buttons
    if (UIHelper::renderButton(nullptr, m_buttons[0], m_mouseX, m_mouseY, m_mouseClicked)) {
        std::cout << "[MainMenu] START GAME clicked" << std::endl;
        m_result = MenuResult::START_GAME;
    }

    if (UIHelper::renderButton(nullptr, m_buttons[1], m_mouseX, m_mouseY, m_mouseClicked)) {
        std::cout << "[MainMenu] SETTINGS clicked" << std::endl;
        m_result = MenuResult::SETTINGS;
    }

    if (UIHelper::renderButton(nullptr, m_buttons[2], m_mouseX, m_mouseY, m_mouseClicked)) {
        std::cout << "[MainMenu] QUIT clicked" << std::endl;
        m_result = MenuResult::QUIT;
    }

    m_mouseClicked = false; // Reset click state

    return m_result;
}

void MainMenu::render(SDL_Renderer* renderer) {
    // Background (dark)
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    // Title
    SDL_Color titleColor = m_shouldGlitch ? SDL_Color{255, 0, 0, 255} : SDL_Color{255, 255, 255, 255};
    std::string titleText = m_shouldGlitch ? "E̴C̷H̶O̸E̷S̸" : "ECHOES";
    UIHelper::renderText(renderer, titleText, 400, 100, titleColor, 48, true);

    // Subtitle
    SDL_Color subtitleColor = {150, 150, 150, 255};
    UIHelper::renderText(renderer, "The Forgotten Platformer", 400, 160, subtitleColor, 18, true);

    // Buttons
    for (auto& button : m_buttons) {
        UIHelper::renderButton(renderer, button, m_mouseX, m_mouseY, false);
    }

    // Warning text at bottom
    SDL_Color warningColor = {200, 0, 0, 255};
    UIHelper::renderText(renderer, "WARNING: This game uses meta-horror techniques", 400, 550, warningColor, 14, true);
}
