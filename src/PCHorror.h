#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <memory>
#include "AudioManager.h"
#include "MetaHorror.h"

/**
 * EXTREME PC Horror System
 *
 * Simulates disturbing computer events that make the player feel like
 * the game has broken out of its sandbox and is affecting their real computer.
 *
 * WARNING: These are all SIMULATIONS - no actual system changes are made!
 * All effects are visual/audio only and contained within the game.
 */

enum class PCHorrorType {
    // Level 3 - First warnings
    FAKE_FILE_SCAN,           // "Scanning files..." with fake results
    FAKE_NOTIFICATION,        // System-style notifications
    CURSOR_GLITCH,            // Cursor movement/duplicates

    // Level 5 - Escalation
    FAKE_RANSOMWARE,          // "Your files are being encrypted..."
    FAKE_VIRUS_DETECTED,      // Virus scanner detecting "threats"
    WINDOW_CHAOS_MILD,        // Gentle window shaking
    FAKE_WEBCAM_ACCESS,       // "Camera accessed by ECHOES.exe"

    // Level 7 - Severe
    FAKE_FILE_DELETION,       // "Deleting personal files..."
    FAKE_SYSTEM_CORRUPTION,   // "System32 corrupted"
    WINDOW_CHAOS_EXTREME,     // Wild window movement
    FAKE_KEYLOGGER,           // "Recording keystrokes..."
    FAKE_CONTACT_ACCESS,      // "Accessing contact list..."

    // Level 9 - NIGHTMARE
    FAKE_SHUTDOWN,            // Full shutdown sequence
    FAKE_RANSOMWARE_PAYMENT,  // Ransom demand with Bitcoin address
    DESKTOP_TAKEOVER,         // Full screen fake desktop
    AUDIO_PERSISTENCE,        // Sounds continue after "closing" game
    MULTIPLE_WINDOWS          // Spawn multiple fake game windows
};

struct PCHorrorEffect {
    PCHorrorType type;
    float duration;           // How long the effect lasts
    float intensity;          // 0.0 - 1.0 (affects visuals/behavior)
    bool isActive;
    float timer;
    std::vector<std::string> messages;
    int phase;                // For multi-phase effects

    PCHorrorEffect(PCHorrorType t, float dur, float intens = 1.0f)
        : type(t), duration(dur), intensity(intens), isActive(false), timer(0.0f), phase(0) {}
};

class PCHorror {
public:
    PCHorror(SDL_Window* window, AudioManager* audioManager, MetaHorror* metaHorror);
    ~PCHorror();

    // Trigger specific horror effects
    void triggerEffect(PCHorrorType type, float duration = 5.0f, float intensity = 1.0f);

    // Trigger random effect appropriate for level
    void triggerRandomForLevel(int level);

    // Update all active effects
    void update(float deltaTime);

    // Render all active effects
    void render(SDL_Renderer* renderer);

    // Update window (for window manipulation effects)
    void updateWindow();

    // Check if any effects are active
    bool hasActiveEffects() const;

    // Clear all effects (emergency stop)
    void clearAll();

private:
    SDL_Window* m_window;
    AudioManager* m_audioManager;
    MetaHorror* m_metaHorror;

    std::vector<std::unique_ptr<PCHorrorEffect>> m_activeEffects;

    // Window manipulation state
    int m_originalWindowX;
    int m_originalWindowY;
    int m_originalWindowW;
    int m_originalWindowH;
    bool m_windowManipulated;

    // Cursor state
    int m_cursorOffsetX;
    int m_cursorOffsetY;

    // Fake file lists for deletion/encryption effects
    std::vector<std::string> m_fakePersonalFiles;
    std::vector<std::string> m_fakeSystemFiles;
    std::vector<std::string> m_fakeVirusThreats;

    // Individual effect renderers
    void renderFileScan(SDL_Renderer* renderer, PCHorrorEffect* effect);
    void renderNotification(SDL_Renderer* renderer, PCHorrorEffect* effect);
    void renderCursorGlitch(SDL_Renderer* renderer, PCHorrorEffect* effect);
    void renderRansomware(SDL_Renderer* renderer, PCHorrorEffect* effect);
    void renderVirusDetected(SDL_Renderer* renderer, PCHorrorEffect* effect);
    void renderFileDeletion(SDL_Renderer* renderer, PCHorrorEffect* effect);
    void renderSystemCorruption(SDL_Renderer* renderer, PCHorrorEffect* effect);
    void renderKeylogger(SDL_Renderer* renderer, PCHorrorEffect* effect);
    void renderContactAccess(SDL_Renderer* renderer, PCHorrorEffect* effect);
    void renderShutdown(SDL_Renderer* renderer, PCHorrorEffect* effect);
    void renderRansomwarePayment(SDL_Renderer* renderer, PCHorrorEffect* effect);
    void renderDesktopTakeover(SDL_Renderer* renderer, PCHorrorEffect* effect);
    void renderWebcamAccess(SDL_Renderer* renderer, PCHorrorEffect* effect);

    // Individual effect updaters
    void updateWindowChaos(PCHorrorEffect* effect, float deltaTime);
    void updateCursorGlitch(PCHorrorEffect* effect, float deltaTime);
    void updateAudioPersistence(PCHorrorEffect* effect, float deltaTime);

    // Helper functions
    void initializeFakeLists();
    void restoreWindow();
    std::string generateFakeBitcoinAddress();
    std::string generateFakeIPAddress();
};
