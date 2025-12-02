#pragma once
#include "SDL_Platform.h"

#include "AudioManager.h"
#include <string>
#include <vector>

/**
 * Settings Menu
 *
 * Allows player to adjust game settings:
 * - Audio volume (Music, SFX)
 * - Fullscreen toggle
 * - Window resolution
 * - Controls display
 * - Reset to defaults
 */
class SettingsMenu {
public:
    enum class SettingsResult {
        NONE,
        BACK_TO_MENU,
        APPLY_SETTINGS
    };

    enum class SettingsOption {
        MUSIC_VOLUME,
        SFX_VOLUME,
        FULLSCREEN,
        RESOLUTION,
        SHOW_CONTROLS,
        RESET_DEFAULTS,
        BACK,
        COUNT  // Total number of options
    };

    SettingsMenu(AudioManager* audioManager);

    // Update
    SettingsResult update(float deltaTime);

    // Handle input
    void handleInput(SDL_Event& event);

    // Render
    void render(SDL_Renderer* renderer);

    // Reset to defaults
    void resetToDefaults();

    // Settings getters
    int getMusicVolume() const { return m_musicVolume; }
    int getSFXVolume() const { return m_sfxVolume; }
    bool isFullscreen() const { return m_fullscreen; }
    int getResolutionIndex() const { return m_resolutionIndex; }

    // Settings setters (for external use)
    void setMusicVolume(int volume);
    void setSFXVolume(int volume);
    void setFullscreen(bool fullscreen);
    void setResolutionIndex(int index);

private:
    AudioManager* m_audioManager;

    // Current settings
    int m_musicVolume;      // 0-100
    int m_sfxVolume;        // 0-100
    bool m_fullscreen;
    int m_resolutionIndex;  // 0 = 800x600, 1 = 1024x768, 2 = 1280x720, 3 = 1920x1080

    // Menu state
    int m_selectedOption;
    float m_selectionTimer;
    bool m_showingControls;

    // Available resolutions
    struct Resolution {
        int width;
        int height;
        std::string name;
    };
    std::vector<Resolution> m_resolutions;

    // Helper methods
    void moveSelectionUp();
    void moveSelectionDown();
    void adjustSetting(int delta);  // delta: -1 for decrease, +1 for increase
    void selectCurrentOption();
    std::string getOptionText(SettingsOption option) const;
    std::string getOptionValue(SettingsOption option) const;
};
