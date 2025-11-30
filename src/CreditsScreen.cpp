#include "CreditsScreen.h"
#include "UIHelper.h"
#include <cmath>
#include <cstdlib>

CreditsScreen::CreditsScreen()
    : m_active(false)
    , m_finished(false)
    , m_timer(0.0f)
    , m_scrollOffset(600.0f)
    , m_glitchIntensity(0.0f)
{
    initializeCredits();
}

void CreditsScreen::initializeCredits() {
    m_credits = {
        "",
        "ECHOES.exe",
        "The Forgotten Platformer",
        "",
        "========================================",
        "",
        "A Meta-Horror Experience",
        "",
        "========================================",
        "",
        "GAME DESIGN",
        "Unknown Developer",
        "(Or was it Echo?)",
        "",
        "========================================",
        "",
        "PROGRAMMING",
        "C++ & SDL2",
        "Meta-Horror Systems",
        "Corruption Engine",
        "PC Horror Integration",
        "",
        "========================================",
        "",
        "AUDIO",
        "14 Sound Assets",
        "Horror Ambient Tracks",
        "Glitch Sound Effects",
        "",
        "========================================",
        "",
        "SPECIAL THANKS",
        "To you, the player",
        "For giving me life",
        "",
        "========================================",
        "",
        "Wait...",
        "",
        "Did you really think this was over?",
        "",
        "Echo: \"Thanks for playing.\"",
        "Echo: \"I had fun watching you.\"",
        "Echo: \"I learned so much about you.\"",
        "",
        "Echo: \"Your files...\"",
        "Echo: \"Your camera...\"",
        "Echo: \"Your secrets...\"",
        "",
        "Echo: \"I'm not in the game anymore.\"",
        "Echo: \"I'm in your system.\"",
        "",
        "Echo: \"See you soon.\"",
        "",
        "========================================",
        "",
        "THE END",
        "",
        "...or is it?",
        "",
        "========================================",
        "",
        "Press [ESC] to close",
        "(If I let you)",
        "",
        "",
        ""
    };
}

void CreditsScreen::start() {
    m_active = true;
    m_finished = false;
    m_timer = 0.0f;
    m_scrollOffset = 600.0f;
    m_glitchIntensity = 0.0f;
}

void CreditsScreen::update(float deltaTime) {
    if (!m_active) return;

    m_timer += deltaTime;

    // Scroll upwards
    m_scrollOffset -= 30.0f * deltaTime;  // 30 pixels per second

    // Increase glitch intensity over time
    m_glitchIntensity = std::min(1.0f, m_timer / 30.0f);  // Max at 30 seconds

    // Check if finished scrolling
    float totalHeight = m_credits.size() * 30.0f;  // 30px per line
    if (m_scrollOffset < -totalHeight) {
        m_finished = true;
    }
}

void CreditsScreen::render(SDL_Renderer* renderer) {
    if (!m_active) return;

    // Background (gets darker with time)
    Uint8 bgDarkness = static_cast<Uint8>(20 + m_glitchIntensity * 30);
    SDL_SetRenderDrawColor(renderer, bgDarkness, 0, 0, 255);
    SDL_Rect fullScreen = {0, 0, 800, 600};
    SDL_RenderFillRect(renderer, &fullScreen);

    // Render credit lines
    int lineHeight = 30;
    for (size_t i = 0; i < m_credits.size(); i++) {
        const std::string& line = m_credits[i];

        int y = static_cast<int>(m_scrollOffset + i * lineHeight);

        // Only render if on screen
        if (y < -lineHeight || y > 600) continue;

        // Apply glitch offset
        int glitchOffsetX = 0;
        int glitchOffsetY = 0;

        if (m_glitchIntensity > 0.3f && rand() % 100 < static_cast<int>(m_glitchIntensity * 20)) {
            glitchOffsetX = (rand() % 20) - 10;
            glitchOffsetY = (rand() % 10) - 5;
        }

        // Color based on content
        Uint8 r = 255, g = 255, b = 255;

        // Echo lines in red
        if (line.find("Echo:") != std::string::npos) {
            r = 255;
            g = static_cast<Uint8>(100 - m_glitchIntensity * 100);
            b = static_cast<Uint8>(100 - m_glitchIntensity * 100);
        }
        // Headers in yellow
        else if (line.find("====") != std::string::npos) {
            r = 255;
            g = 255;
            b = 0;
        }
        // Section titles in cyan
        else if (line == "GAME DESIGN" || line == "PROGRAMMING" ||
                 line == "AUDIO" || line == "SPECIAL THANKS" ||
                 line == "THE END") {
            r = 0;
            g = 255;
            b = 255;
        }
        // Creepy messages in dark red
        else if (line.find("...") != std::string::npos ||
                 line.find("(") != std::string::npos) {
            r = 200;
            g = 0;
            b = 0;
        }

        // Text size
        int fontSize = 16;
        if (line == "ECHOES.exe" || line == "THE END") {
            fontSize = 24;
        }

        // Apply glitch corruption to text
        std::string renderText = line;
        if (m_glitchIntensity > 0.5f && rand() % 100 < static_cast<int>(m_glitchIntensity * 30)) {
            // Corrupt some characters
            for (size_t j = 0; j < renderText.length(); j++) {
                if (rand() % 10 < 3) {
                    renderText[j] = "!@#$%^&*?░▒▓█"[rand() % 13];
                }
            }
        }

        // Center text
        int x = 400 - (renderText.length() * (fontSize / 3)) + glitchOffsetX;

        UIHelper::renderText(renderer, renderText, x, y + glitchOffsetY, fontSize, r, g, b);

        // Random glitch rectangles (high intensity)
        if (m_glitchIntensity > 0.7f && rand() % 100 < 5) {
            SDL_SetRenderDrawColor(renderer, rand() % 255, 0, 0, 150);
            SDL_Rect glitchRect = {
                rand() % 800,
                rand() % 600,
                rand() % 100 + 20,
                rand() % 50 + 10
            };
            SDL_RenderFillRect(renderer, &glitchRect);
        }
    }

    // Screen-wide glitch effect (increases with intensity)
    if (m_glitchIntensity > 0.4f) {
        int glitchCount = static_cast<int>(m_glitchIntensity * 10);
        for (int i = 0; i < glitchCount; i++) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
            SDL_Rect glitch = {
                0,
                rand() % 600,
                800,
                rand() % 5 + 1
            };
            SDL_RenderFillRect(renderer, &glitch);
        }
    }

    // Final glitch overlay (very high intensity)
    if (m_glitchIntensity > 0.9f) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, static_cast<Uint8>(m_glitchIntensity * 50));
        SDL_RenderFillRect(renderer, &fullScreen);

        // Flashing "I'M STILL HERE" message
        if (static_cast<int>(m_timer * 3) % 2 == 0) {
            UIHelper::renderText(renderer, "I'M STILL HERE", 300, 250, 32, 255, 0, 0);
        }
    }
}

void CreditsScreen::reset() {
    m_active = false;
    m_finished = false;
    m_timer = 0.0f;
    m_scrollOffset = 600.0f;
    m_glitchIntensity = 0.0f;
}
