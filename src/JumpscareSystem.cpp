#include "JumpscareSystem.h"
#include "AudioManager.h"
#include "UIHelper.h"
#include <iostream>

JumpscareSystem::JumpscareSystem(AudioManager* audioManager)
    : m_audioManager(audioManager)
    , m_active(false)
    , m_currentType(ScareType::FLASH)
    , m_scareTimer(0.0f)
    , m_scareDuration(0.5f)
    , m_timeSinceLastScare(0.0f)
    , m_flashAlpha(0)
{
    std::random_device rd;
    m_rng.seed(rd());
}

JumpscareSystem::~JumpscareSystem() {
}

void JumpscareSystem::triggerScare(ScareType type) {
    m_active = true;
    m_currentType = type;
    m_scareTimer = 0.0f;
    m_timeSinceLastScare = 0.0f;

    switch (type) {
        case ScareType::FLASH:
            m_scareDuration = 0.3f;
            m_flashAlpha = 255;
            if (m_audioManager) {
                m_audioManager->playJumpscare();
            }
            std::cout << "[Jumpscare] FLASH!" << std::endl;
            break;

        case ScareType::SCREAMER_IMAGE:
            m_scareDuration = 1.0f;
            if (m_audioManager) {
                m_audioManager->playJumpscare();
            }
            std::cout << "[Jumpscare] SCREAMER IMAGE!" << std::endl;
            break;

        case ScareType::SUDDEN_APPEAR:
            m_scareDuration = 0.5f;
            if (m_audioManager) {
                m_audioManager->playSound("scare_appear", 100);
            }
            std::cout << "[Jumpscare] SUDDEN APPEAR!" << std::endl;
            break;

        case ScareType::AUDIO_SPIKE:
            m_scareDuration = 0.2f;
            if (m_audioManager) {
                m_audioManager->playSound("scare_audio", 128);  // ✓ Scary audio spike!
            }
            std::cout << "[Jumpscare] AUDIO SPIKE!" << std::endl;
            break;
    }
}

void JumpscareSystem::triggerRandomScare() {
    std::uniform_int_distribution<> dis(0, 3);
    ScareType type = static_cast<ScareType>(dis(m_rng));
    triggerScare(type);
}

void JumpscareSystem::update(float deltaTime, int corruptionLevel) {
    m_timeSinceLastScare += deltaTime;

    if (m_active) {
        m_scareTimer += deltaTime;

        // Flash fade-out
        if (m_currentType == ScareType::FLASH && m_flashAlpha > 0) {
            m_flashAlpha = static_cast<Uint8>(255 * (1.0f - m_scareTimer / m_scareDuration));
        }

        if (m_scareTimer >= m_scareDuration) {
            m_active = false;
            m_scareTimer = 0.0f;
        }
    } else {
        // Random scares based on corruption level
        // Mindestens 30 Sekunden zwischen Scares
        if (m_timeSinceLastScare > 30.0f && corruptionLevel > 20) {
            std::uniform_int_distribution<> chanceDis(0, 1000);
            int chance = chanceDis(m_rng);

            // Je höher corruption, desto öfter Scares
            if (chance < corruptionLevel / 2) {
                triggerRandomScare();
            }
        }
    }
}

void JumpscareSystem::render(SDL_Renderer* renderer) {
    if (!m_active) return;

    switch (m_currentType) {
        case ScareType::FLASH:
            {
                // White flash
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, m_flashAlpha);
                SDL_Rect fullScreen = {0, 0, 800, 600};
                SDL_RenderFillRect(renderer, &fullScreen);
            }
            break;

        case ScareType::SCREAMER_IMAGE:
            // Red screen with creepy text
            SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
            SDL_RenderClear(renderer);

            {
                SDL_Color textColor = {255, 0, 0, 255};
                UIHelper::renderText(renderer, "I SEE YOU", 400, 250, textColor, 72, true);
                UIHelper::renderText(renderer, "E̴C̷H̶O̸", 400, 350, textColor, 48, true);
            }
            break;

        case ScareType::SUDDEN_APPEAR:
            // Quick red flash
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 180);
            {
                SDL_Rect flash = {0, 0, 800, 600};
                SDL_RenderFillRect(renderer, &flash);
            }
            break;

        case ScareType::AUDIO_SPIKE:
            // Just audio, no visual
            break;
    }
}
