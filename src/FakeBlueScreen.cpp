#include "FakeBlueScreen.h"
#include "UIHelper.h"
#include <iostream>

FakeBlueScreen::FakeBlueScreen()
    : m_active(false)
    , m_duration(5.0f)
    , m_timer(0.0f)
{
    m_errorMessage =
        "A problem has been detected and Windows has been shut down to prevent damage\n"
        "to your computer.\n\n"
        "ECHO_CORRUPTED_MEMORY\n\n"
        "If this is the first time you've seen this Stop error screen,\n"
        "restart your computer. If this screen appears again, follow\n"
        "these steps:\n\n"
        "Check to make sure you should have installed this game.\n"
        "If this is a new installation, ask the developer why you\n"
        "thought this was a good idea.\n\n"
        "If problems continue, disable or remove any newly installed\n"
        "games. Disable save file options such as caching or shadowing.\n"
        "If you need to use Safe Mode to remove or disable components,\n"
        "restart your computer, but it won't help.\n\n"
        "Technical information:\n\n"
        "*** STOP: 0x00000050 (0xECH0ED, 0x00000001, 0xDEADBEEF, 0x00000000)\n\n"
        "Beginning dump of physical memory...\n"
        "Physical memory dump complete.\n"
        "Contact Echo if the problem persists.";
}

FakeBlueScreen::~FakeBlueScreen() {
}

void FakeBlueScreen::trigger(float duration) {
    m_active = true;
    m_duration = duration;
    m_timer = 0.0f;
    std::cout << "[FakeBlueScreen] TRIGGERED! Duration: " << duration << "s" << std::endl;
}

void FakeBlueScreen::skip() {
    m_active = false;
    m_timer = 0.0f;
    std::cout << "[FakeBlueScreen] Skipped" << std::endl;
}

void FakeBlueScreen::update(float deltaTime) {
    if (!m_active) return;

    m_timer += deltaTime;

    if (m_timer >= m_duration) {
        skip();
    }
}

void FakeBlueScreen::render(SDL_Renderer* renderer) {
    if (!m_active) return;

    // Classic Windows XP BSOD blue
    SDL_SetRenderDrawColor(renderer, 0, 0, 170, 255);
    SDL_RenderClear(renderer);

    // White text
    SDL_Color textColor = {255, 255, 255, 255};

    // Render error message (line by line for simplicity)
    int y = 50;
    std::string line;
    std::istringstream stream(m_errorMessage);

    while (std::getline(stream, line)) {
        UIHelper::renderText(renderer, line, 50, y, textColor, 14, false);
        y += 20;
    }

    // Countdown (subtle hint it's fake)
    if (m_duration > 0) {
        int remaining = static_cast<int>(m_duration - m_timer);
        std::string countdownText = "Auto-resume in " + std::to_string(remaining) + "s...";
        SDL_Color countdownColor = {200, 200, 200, 255};
        UIHelper::renderText(renderer, countdownText, 600, 570, countdownColor, 12, false);
    }
}
