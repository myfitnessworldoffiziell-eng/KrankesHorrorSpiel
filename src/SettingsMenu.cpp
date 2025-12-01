#include "SettingsMenu.h"
#include "UIHelper.h"
#include <iostream>
#include <cmath>

SettingsMenu::SettingsMenu(AudioManager* audioManager)
    : m_audioManager(audioManager)
    , m_musicVolume(70)
    , m_sfxVolume(80)
    , m_fullscreen(false)
    , m_resolutionIndex(0)  // Default: 800x600
    , m_selectedOption(0)
    , m_selectionTimer(0.0f)
    , m_showingControls(false)
{
    // Initialize available resolutions
    m_resolutions = {
        {800, 600, "800x600"},
        {1024, 768, "1024x768"},
        {1280, 720, "1280x720 (HD)"},
        {1920, 1080, "1920x1080 (Full HD)"}
    };
}

void SettingsMenu::resetToDefaults() {
    m_musicVolume = 70;
    m_sfxVolume = 80;
    m_fullscreen = false;
    m_resolutionIndex = 0;

    // Apply to audio manager
    if (m_audioManager) {
        m_audioManager->setMusicVolume(m_musicVolume);
        m_audioManager->setSFXVolume(m_sfxVolume);
    }

    std::cout << "[SettingsMenu] Reset to defaults" << std::endl;
}

void SettingsMenu::setMusicVolume(int volume) {
    m_musicVolume = std::max(0, std::min(100, volume));
    if (m_audioManager) {
        m_audioManager->setMusicVolume(m_musicVolume);
    }
}

void SettingsMenu::setSFXVolume(int volume) {
    m_sfxVolume = std::max(0, std::min(100, volume));
    if (m_audioManager) {
        m_audioManager->setSFXVolume(m_sfxVolume);
    }
}

void SettingsMenu::setFullscreen(bool fullscreen) {
    m_fullscreen = fullscreen;
}

void SettingsMenu::setResolutionIndex(int index) {
    m_resolutionIndex = std::max(0, std::min(static_cast<int>(m_resolutions.size()) - 1, index));
}

SettingsMenu::SettingsResult SettingsMenu::update(float deltaTime) {
    m_selectionTimer += deltaTime;
    return SettingsResult::NONE;
}

void SettingsMenu::handleInput(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        // If showing controls, any key closes it
        if (m_showingControls) {
            m_showingControls = false;
            return;
        }

        switch (event.key.keysym.sym) {
            case SDLK_UP:
            case SDLK_w:
                moveSelectionUp();
                break;

            case SDLK_DOWN:
            case SDLK_s:
                moveSelectionDown();
                break;

            case SDLK_LEFT:
            case SDLK_a:
                adjustSetting(-1);
                break;

            case SDLK_RIGHT:
            case SDLK_d:
                adjustSetting(1);
                break;

            case SDLK_RETURN:
            case SDLK_SPACE:
                selectCurrentOption();
                break;

            case SDLK_ESCAPE:
                // Go back
                m_selectedOption = static_cast<int>(SettingsOption::BACK);
                selectCurrentOption();
                break;
        }
    }
}

void SettingsMenu::moveSelectionUp() {
    m_selectedOption--;
    if (m_selectedOption < 0) {
        m_selectedOption = static_cast<int>(SettingsOption::COUNT) - 1;
    }
    m_selectionTimer = 0.0f;
}

void SettingsMenu::moveSelectionDown() {
    m_selectedOption++;
    if (m_selectedOption >= static_cast<int>(SettingsOption::COUNT)) {
        m_selectedOption = 0;
    }
    m_selectionTimer = 0.0f;
}

void SettingsMenu::adjustSetting(int delta) {
    SettingsOption option = static_cast<SettingsOption>(m_selectedOption);

    switch (option) {
        case SettingsOption::MUSIC_VOLUME:
            setMusicVolume(m_musicVolume + delta * 5);  // Adjust by 5%
            std::cout << "[Settings] Music volume: " << m_musicVolume << "%" << std::endl;
            break;

        case SettingsOption::SFX_VOLUME:
            setSFXVolume(m_sfxVolume + delta * 5);
            // Play test sound
            if (m_audioManager) {
                m_audioManager->playSound("collect", m_sfxVolume);
            }
            std::cout << "[Settings] SFX volume: " << m_sfxVolume << "%" << std::endl;
            break;

        case SettingsOption::FULLSCREEN:
            setFullscreen(!m_fullscreen);
            std::cout << "[Settings] Fullscreen: " << (m_fullscreen ? "ON" : "OFF") << std::endl;
            break;

        case SettingsOption::RESOLUTION:
            setResolutionIndex(m_resolutionIndex + delta);
            std::cout << "[Settings] Resolution: " << m_resolutions[m_resolutionIndex].name << std::endl;
            break;

        default:
            break;
    }
}

void SettingsMenu::selectCurrentOption() {
    SettingsOption option = static_cast<SettingsOption>(m_selectedOption);

    switch (option) {
        case SettingsOption::SHOW_CONTROLS:
            m_showingControls = true;
            std::cout << "[Settings] Showing controls" << std::endl;
            break;

        case SettingsOption::RESET_DEFAULTS:
            resetToDefaults();
            break;

        case SettingsOption::BACK:
            std::cout << "[Settings] Back to menu" << std::endl;
            break;

        default:
            // For other options, left/right adjusts them
            break;
    }
}

std::string SettingsMenu::getOptionText(SettingsOption option) const {
    switch (option) {
        case SettingsOption::MUSIC_VOLUME: return "Music Volume";
        case SettingsOption::SFX_VOLUME: return "SFX Volume";
        case SettingsOption::FULLSCREEN: return "Fullscreen";
        case SettingsOption::RESOLUTION: return "Resolution";
        case SettingsOption::SHOW_CONTROLS: return "Show Controls";
        case SettingsOption::RESET_DEFAULTS: return "Reset to Defaults";
        case SettingsOption::BACK: return "Back";
        default: return "";
    }
}

std::string SettingsMenu::getOptionValue(SettingsOption option) const {
    switch (option) {
        case SettingsOption::MUSIC_VOLUME:
            return std::to_string(m_musicVolume) + "%";

        case SettingsOption::SFX_VOLUME:
            return std::to_string(m_sfxVolume) + "%";

        case SettingsOption::FULLSCREEN:
            return m_fullscreen ? "ON" : "OFF";

        case SettingsOption::RESOLUTION:
            return m_resolutions[m_resolutionIndex].name;

        case SettingsOption::SHOW_CONTROLS:
        case SettingsOption::RESET_DEFAULTS:
        case SettingsOption::BACK:
            return "";  // No value for these options

        default:
            return "";
    }
}

void SettingsMenu::render(SDL_Renderer* renderer) {
    // If showing controls, render controls screen instead
    if (m_showingControls) {
        // Dark background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 240);
        SDL_Rect bg = {0, 0, 800, 600};
        SDL_RenderFillRect(renderer, &bg);

        // Title
        UIHelper::renderText(renderer, "CONTROLS", 320, 50, 24, 255, 200, 0);

        // Controls list
        int y = 120;
        int spacing = 30;

        UIHelper::renderText(renderer, "MOVEMENT:", 100, y, 18, 255, 255, 255);
        y += spacing;
        UIHelper::renderText(renderer, "  Arrow Keys / WASD - Move", 120, y, 14, 200, 200, 200);
        y += spacing;
        UIHelper::renderText(renderer, "  SPACE - Jump", 120, y, 14, 200, 200, 200);
        y += spacing + 10;

        UIHelper::renderText(renderer, "INTERACTION:", 100, y, 18, 255, 255, 255);
        y += spacing;
        UIHelper::renderText(renderer, "  E - Talk to NPCs", 120, y, 14, 200, 200, 200);
        y += spacing;
        UIHelper::renderText(renderer, "  ENTER - Advance dialog", 120, y, 14, 200, 200, 200);
        y += spacing + 10;

        UIHelper::renderText(renderer, "MENU:", 100, y, 18, 255, 255, 255);
        y += spacing;
        UIHelper::renderText(renderer, "  ESC - Pause / Back", 120, y, 14, 200, 200, 200);
        y += spacing + 10;

        UIHelper::renderText(renderer, "DEBUG (Optional):", 100, y, 18, 255, 255, 255);
        y += spacing;
        UIHelper::renderText(renderer, "  G - Trigger Glitch", 120, y, 14, 150, 150, 150);
        y += spacing;
        UIHelper::renderText(renderer, "  F - Create File", 120, y, 14, 150, 150, 150);
        y += spacing;
        UIHelper::renderText(renderer, "  W - Window Manipulation", 120, y, 14, 150, 150, 150);
        y += spacing;
        UIHelper::renderText(renderer, "  J - Jumpscare", 120, y, 14, 150, 150, 150);
        y += spacing;
        UIHelper::renderText(renderer, "  B - Fake BSOD", 120, y, 14, 150, 150, 150);
        y += spacing;
        UIHelper::renderText(renderer, "  F11 - Toggle Fullscreen", 120, y, 14, 150, 150, 150);
        y += spacing + 20;

        // Press any key to close
        if (static_cast<int>(m_selectionTimer * 2) % 2 == 0) {
            UIHelper::renderText(renderer, "Press any key to close", 270, 550, 14, 255, 255, 0);
        }

        return;
    }

    // Normal settings menu rendering
    // Dark background with transparency
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect bg = {0, 0, 800, 600};
    SDL_RenderFillRect(renderer, &bg);

    // Title
    UIHelper::renderText(renderer, "SETTINGS", 330, 50, 32, 255, 200, 0);

    // Options
    int startY = 150;
    int spacing = 50;

    for (int i = 0; i < static_cast<int>(SettingsOption::COUNT); i++) {
        SettingsOption option = static_cast<SettingsOption>(i);
        int y = startY + i * spacing;

        // Highlight selected option
        bool isSelected = (i == m_selectedOption);

        // Selection indicator (animated)
        if (isSelected) {
            float pulse = std::sin(m_selectionTimer * 4.0f) * 0.3f + 0.7f;
            Uint8 alpha = static_cast<Uint8>(pulse * 100);

            SDL_SetRenderDrawColor(renderer, 255, 200, 0, alpha);
            SDL_Rect highlight = {180, y - 5, 440, 40};
            SDL_RenderFillRect(renderer, &highlight);

            // Selection arrow
            UIHelper::renderText(renderer, ">", 150, y, 20, 255, 200, 0);
        }

        // Option text
        std::string optionText = getOptionText(option);
        Uint8 r = isSelected ? 255 : 200;
        Uint8 g = isSelected ? 255 : 200;
        Uint8 b = isSelected ? 255 : 200;

        UIHelper::renderText(renderer, optionText, 200, y, 18, r, g, b);

        // Option value (if applicable)
        std::string valueText = getOptionValue(option);
        if (!valueText.empty()) {
            // Value on the right
            UIHelper::renderText(renderer, valueText, 480, y, 18, 0, 255, 100);

            // Show < > arrows for adjustable settings if selected
            if (isSelected) {
                if (option == SettingsOption::MUSIC_VOLUME ||
                    option == SettingsOption::SFX_VOLUME ||
                    option == SettingsOption::FULLSCREEN ||
                    option == SettingsOption::RESOLUTION) {
                    UIHelper::renderText(renderer, "<", 460, y, 16, 255, 255, 0);
                    UIHelper::renderText(renderer, ">", 600, y, 16, 255, 255, 0);
                }
            }
        }

        // Volume bar visualization
        if (option == SettingsOption::MUSIC_VOLUME || option == SettingsOption::SFX_VOLUME) {
            int volume = (option == SettingsOption::MUSIC_VOLUME) ? m_musicVolume : m_sfxVolume;
            int barWidth = static_cast<int>((volume / 100.0f) * 100);

            SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
            SDL_Rect barBg = {500, y + 5, 100, 10};
            SDL_RenderFillRect(renderer, &barBg);

            SDL_SetRenderDrawColor(renderer, 0, 255, 100, 255);
            SDL_Rect barFill = {500, y + 5, barWidth, 10};
            SDL_RenderFillRect(renderer, &barFill);
        }
    }

    // Instructions at bottom
    int instructionY = 560;
    UIHelper::renderText(renderer, "Arrow Keys: Navigate", 100, instructionY, 12, 150, 150, 150);
    UIHelper::renderText(renderer, "Left/Right: Adjust", 300, instructionY, 12, 150, 150, 150);
    UIHelper::renderText(renderer, "Enter: Select", 500, instructionY, 12, 150, 150, 150);
    UIHelper::renderText(renderer, "ESC: Back", 650, instructionY, 12, 150, 150, 150);

    // Warning message if fullscreen changed
    if (m_fullscreen) {
        UIHelper::renderText(renderer, "Note: Fullscreen requires game restart", 220, 520, 12, 255, 150, 0);
    }
}
