#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>

class AudioManager;
class PermissionManager;

/**
 * BossDefeatHorror
 * EXTREME horror sequence that plays after defeating a boss
 *
 * Sequence:
 * 1. Black screen + white noise
 * 2. "I'm free..." message
 * 3. Fake file deletion (system files, documents, photos)
 * 4. Camera activation (if permission granted)
 * 5. "I can see you" message
 * 6. Extreme screen glitches
 * 7. Window manipulation (moves wildly)
 * 8. Final warning message
 */
class BossDefeatHorror {
public:
    BossDefeatHorror(AudioManager* audioManager = nullptr, PermissionManager* permManager = nullptr);
    ~BossDefeatHorror();

    void trigger(const std::string& bossName);  // Start the horror sequence
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    bool isActive() const { return m_active; }
    bool isComplete() const { return m_sequenceComplete; }

    // For window manipulation
    void updateWindow(SDL_Window* window);

private:
    AudioManager* m_audioManager;
    PermissionManager* m_permissionManager;

    bool m_active;
    bool m_sequenceComplete;
    float m_timer;
    int m_currentPhase;  // 0-8 for different horror phases

    std::string m_bossName;
    std::vector<std::string> m_fakeFiles;  // Files being "deleted"
    int m_filesDeleted;

    // Visual effects
    float m_glitchIntensity;
    int m_glitchOffsetX, m_glitchOffsetY;
    bool m_screenShake;
    float m_shakeTimer;

    // Window manipulation
    bool m_windowManipulation;
    float m_windowMoveTimer;
    int m_originalWindowX, m_originalWindowY;

    // Rendering helpers
    void renderPhase0(SDL_Renderer* renderer);  // Black screen
    void renderPhase1(SDL_Renderer* renderer);  // "I'm free..."
    void renderPhase2(SDL_Renderer* renderer);  // File deletion
    void renderPhase3(SDL_Renderer* renderer);  // Camera warning
    void renderPhase4(SDL_Renderer* renderer);  // "I can see you"
    void renderPhase5(SDL_Renderer* renderer);  // Extreme glitches
    void renderPhase6(SDL_Renderer* renderer);  // Final warning
    void renderPhase7(SDL_Renderer* renderer);  // Fade out

    void generateFakeFiles();
};
