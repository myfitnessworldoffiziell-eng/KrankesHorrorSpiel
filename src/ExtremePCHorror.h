#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "AudioManager.h"
#include "MetaHorror.h"

/**
 * EXTREME PC Horror System - MAXIMUM HORROR!
 *
 * Diese Klasse implementiert die KRASSESTEN Horror-Effekte,
 * die den PC scheinbar "übernehmen" und maximale Angst erzeugen.
 *
 * WARNUNG: Extrem verstörend! Alle Effekte sind SIMULATIONEN!
 */

enum class ExtremePCHorrorType {
    // CRASH HORROR
    FAKE_CRASH_WITH_JUMPSCARE,     // Game crashed, restart = Jumpscare
    FAKE_NOT_RESPONDING,            // Game friert ein, dann Jumpscare

    // WINDOW HORROR
    MULTIPLE_WINDOWS,               // 10+ Windows spawnen
    WINDOW_TITLE_HORROR,            // Title wird zu "I SEE YOU"
    UNSTOPPABLE_GAME,               // Kann nicht geschlossen werden
    WINDOW_INVERSION,               // Fenster dreht sich um

    // DESKTOP HORROR
    DESKTOP_ICON_CHAOS,             // Icons bewegen sich
    FAKE_FILE_EXPLORER,             // Explorer mit fake Files
    WALLPAPER_HORROR,               // Wallpaper zu Echo's Gesicht

    // SURVEILLANCE HORROR
    FAKE_WEBCAM_FEED,               // Live Webcam "Feed"
    FAKE_AUDIO_RECORDING,           // Mikrofon Recording
    FAKE_SCREEN_RECORDING,          // Screen Recording Active

    // BROWSER HORROR
    FAKE_BROWSER_SEARCH,            // Browser mit verstörenden Suchen
    FAKE_SOCIAL_MEDIA,              // Fake Social Media Posts

    // PERSISTENCE HORROR
    AUDIO_AFTER_CLOSE,              // Audio spielt nach Close weiter
    FAKE_STARTUP_PROGRAM,           // "Added to Startup" Notification
    DESKTOP_POPUP_DELAYED,          // Popup 5 Min nach Close

    // EXTREME JUMPSCARE
    FULLSCREEN_JUMPSCARE,           // Komplettes Fullscreen Jumpscare
    MULTI_MONITOR_HORROR            // Auf allen Monitoren gleichzeitig
};

struct ExtremePCHorrorEffect {
    ExtremePCHorrorType type;
    float duration;
    float intensity;
    bool isActive;
    float timer;
    int phase;
    std::vector<std::string> data;  // For messages, fake files, etc.

    ExtremePCHorrorEffect(ExtremePCHorrorType t, float dur, float intens = 1.0f)
        : type(t), duration(dur), intensity(intens), isActive(false), timer(0.0f), phase(0) {}
};

class ExtremePCHorror {
public:
    ExtremePCHorror(SDL_Window* window, AudioManager* audioManager, MetaHorror* metaHorror);
    ~ExtremePCHorror();

    // Trigger specific extreme horror
    void triggerEffect(ExtremePCHorrorType type, float duration = 10.0f, float intensity = 1.0f);

    // Trigger random EXTREME effect for level
    void triggerRandomExtreme(int level);

    // Update all effects
    void update(float deltaTime);

    // Render all effects
    void render(SDL_Renderer* renderer);

    // Window manipulation
    void updateWindow();

    // Check if any effects active
    bool hasActiveEffects() const;

    // Clear all (emergency)
    void clearAll();

    // Fake Game Crash (returns true if should "restart")
    bool triggerFakeCrash();

    // Check if game is "crashed"
    bool isCrashed() const { return m_isCrashed; }

    // Restart from crash
    void restartFromCrash();

private:
    SDL_Window* m_window;
    AudioManager* m_audioManager;
    MetaHorror* m_metaHorror;

    std::vector<std::unique_ptr<ExtremePCHorrorEffect>> m_activeEffects;

    // Crash state
    bool m_isCrashed;
    float m_crashTimer;
    bool m_jumpscareReady;

    // Window state
    int m_originalWindowX, m_originalWindowY;
    int m_originalWindowW, m_originalWindowH;
    std::string m_originalWindowTitle;
    bool m_windowManipulated;

    // Multiple windows (fake)
    struct FakeWindow {
        SDL_Rect bounds;
        std::string title;
        bool active;
    };
    std::vector<FakeWindow> m_fakeWindows;

    // Effect renderers
    void renderFakeCrashWithJumpscare(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect);
    void renderFakeNotResponding(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect);
    void renderMultipleWindows(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect);
    void renderWindowTitleHorror(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect);
    void renderUnstoppableGame(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect);
    void renderDesktopIconChaos(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect);
    void renderFakeFileExplorer(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect);
    void renderWallpaperHorror(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect);
    void renderFakeWebcamFeed(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect);
    void renderFakeAudioRecording(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect);
    void renderFakeScreenRecording(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect);
    void renderFakeBrowserSearch(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect);
    void renderFakeSocialMedia(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect);
    void renderFullscreenJumpscare(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect);

    // Effect updaters
    void updateMultipleWindows(ExtremePCHorrorEffect* effect, float deltaTime);
    void updateWindowTitleHorror(ExtremePCHorrorEffect* effect, float deltaTime);

    // Helpers
    void createFakeWindows(int count);
    void clearFakeWindows();
    void restoreWindow();
    std::string generateHorrorWindowTitle();
    std::vector<std::string> generateFakeBrowserSearches();
    std::vector<std::string> generateFakeFiles();
};
