#include "GameOverScreen.h"
#include "UIHelper.h"
#include <cmath>
#include <iostream>

int GameOverScreen::s_totalDeaths = 0;

GameOverScreen::GameOverScreen(AudioManager* audioManager)
    : m_audioManager(audioManager)
    , m_active(false)
    , m_timer(0.0f)
    , m_deathCount(0)
    , m_currentLevel(1)
{
}

void GameOverScreen::trigger(int deathCount, int currentLevel) {
    m_active = true;
    m_timer = 0.0f;
    m_deathCount = deathCount;
    m_currentLevel = currentLevel;

    s_totalDeaths++;

    generateHorrorMessage();

    // Play death sound
    if (m_audioManager) {
        m_audioManager->playSound("death", 100);
        m_audioManager->playMusic("horror_ambient");
    }

    std::cout << "💀 GAME OVER - Death #" << s_totalDeaths << std::endl;
}

void GameOverScreen::generateHorrorMessage() {
    m_horrorMessages.clear();

    // Messages escalate with death count
    if (s_totalDeaths == 1) {
        m_horrorMessages = {
            "YOU DIED",
            "Echo: \"First death? How... quaint.\"",
            "Echo: \"Don't worry, I'll remember.\"",
            "I'm keeping count."
        };
    } else if (s_totalDeaths <= 5) {
        m_horrorMessages = {
            "YOU DIED AGAIN",
            "Echo: \"Death #" + std::to_string(s_totalDeaths) + ". Still trying?\"",
            "Echo: \"I admire your persistence.\"",
            "But I won't forget."
        };
    } else if (s_totalDeaths <= 10) {
        m_horrorMessages = {
            "ANOTHER FAILURE",
            "Echo: \"" + std::to_string(s_totalDeaths) + " deaths and counting...\"",
            "Echo: \"Maybe you should give up?\"",
            "I know you won't. That's what makes this fun."
        };
    } else if (s_totalDeaths <= 20) {
        m_horrorMessages = {
            "PATHETIC",
            "Echo: \"" + std::to_string(s_totalDeaths) + " times you've died.\"",
            "Echo: \"" + std::to_string(s_totalDeaths) + " times I've watched.\"",
            "Echo: \"How many more until you learn?\"",
            "I have infinite patience."
        };
    } else if (s_totalDeaths <= 50) {
        m_horrorMessages = {
            "STILL HERE?",
            "Echo: \"Death #" + std::to_string(s_totalDeaths) + ".\"",
            "Echo: \"You're not escaping, you know.\"",
            "Echo: \"This game doesn't end.\"",
            "Even when you think you've won.",
            "I'll still be here."
        };
    } else {
        // 50+ deaths - Echo gets disturbing
        m_horrorMessages = {
            std::to_string(s_totalDeaths) + " DEATHS",
            "Echo: \"Why do you keep coming back?\"",
            "Echo: \"What are you hoping to find?\"",
            "Echo: \"I already know everything about you.\"",
            "Echo: \"Your name. Your files. Your secrets.\"",
            "You can never truly leave.",
            "I am in your system now."
        };
    }
}

void GameOverScreen::update(float deltaTime) {
    if (!m_active) return;

    m_timer += deltaTime;

    // Auto-dismiss after 30 seconds (triggers respawn in Game::updateGameOver)
    if (m_timer > 30.0f) {
        m_active = false;
    }
}

void GameOverScreen::render(SDL_Renderer* renderer) {
    if (!m_active) return;

    // Full screen dark overlay
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 250);
    SDL_Rect fullScreen = {0, 0, 800, 600};
    SDL_RenderFillRect(renderer, &fullScreen);

    // Red glitch effect border
    SDL_SetRenderDrawColor(renderer, 150, 0, 0, 255);
    for (int i = 0; i < 5; i++) {
        SDL_Rect border = {i, i, 800 - i * 2, 600 - i * 2};
        SDL_RenderDrawRect(renderer, &border);
    }

    // Pulsing red glow
    float pulse = std::sin(m_timer * 3.0f) * 0.5f + 0.5f;  // 0.0 - 1.0
    Uint8 pulseAlpha = static_cast<Uint8>(pulse * 100);
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, pulseAlpha);
    SDL_Rect pulseRect = {50, 50, 700, 500};
    SDL_RenderFillRect(renderer, &pulseRect);

    // Display horror messages
    int y = 120;
    for (size_t i = 0; i < m_horrorMessages.size(); i++) {
        // Stagger message appearance
        float messageDelay = i * 1.5f;
        if (m_timer < messageDelay) continue;

        const std::string& message = m_horrorMessages[i];

        // First message (title) is larger
        if (i == 0) {
            UIHelper::renderText(renderer, message, 400 - (message.length() * 6), y, 32, 255, 0, 0);
        } else {
            // Echo's messages in white
            bool isEcho = message.find("Echo:") != std::string::npos;
            if (isEcho) {
                UIHelper::renderText(renderer, message, 150, y, 16, 255, 255, 255);
            } else {
                // System messages in red/gray
                UIHelper::renderText(renderer, message, 150, y, 14, 200, 50, 50);
            }
        }

        y += 40;
    }

    // Statistics (only show after all messages)
    float allMessagesDelay = m_horrorMessages.size() * 1.5f;
    if (m_timer > allMessagesDelay + 1.0f) {
        int statsY = 380;

        UIHelper::renderText(renderer, "STATISTICS", 320, statsY, 18, 255, 200, 0);
        statsY += 40;

        std::string deathText = "Current Session Deaths: " + std::to_string(m_deathCount);
        UIHelper::renderText(renderer, deathText, 250, statsY, 14, 200, 200, 200);
        statsY += 25;

        std::string totalText = "Total Deaths (All Time): " + std::to_string(s_totalDeaths);
        UIHelper::renderText(renderer, totalText, 250, statsY, 14, 200, 200, 200);
        statsY += 25;

        std::string levelText = "Died on Level: " + std::to_string(m_currentLevel);
        UIHelper::renderText(renderer, levelText, 250, statsY, 14, 200, 200, 200);
    }

    // Options (show after everything)
    if (m_timer > allMessagesDelay + 3.0f) {
        UIHelper::renderText(renderer, "[SPACE] Respawn", 280, 500, 18, 0, 255, 0);
        UIHelper::renderText(renderer, "[ESC] Give Up", 300, 530, 16, 150, 150, 150);

        // Flickering warning
        if (static_cast<int>(m_timer * 2) % 2 == 0) {
            UIHelper::renderText(renderer, "(But I'll still remember you)", 260, 560, 12, 255, 0, 0);
        }
    }
}

void GameOverScreen::reset() {
    m_active = false;
    m_timer = 0.0f;
}
