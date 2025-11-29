#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <string>
#include "GameState.h"

// Forward declarations
class MetaHorror;
class PermissionManager;
class CorruptionSystem;
class Player;
class Level;
class AudioManager;
class MainMenu;
class PauseMenu;
class DialogSystem;
class FakeBlueScreen;
class JumpscareSystem;

/**
 * Haupt-Game-Klasse für ECHOES.exe
 * Verwaltet den Game Loop, Rendering und alle Subsysteme
 * NEU: Mit vollständigem State-Management, Audio, UI, Jumpscares!
 */
class Game {
public:
    Game();
    ~Game();

    // Initialisierung
    bool initialize();

    // Main Game Loop
    void run();

    // Cleanup
    void shutdown();

    // Getter
    SDL_Renderer* getRenderer() const { return m_renderer; }
    bool isRunning() const { return m_isRunning; }
    GameState getCurrentState() const { return m_currentState; }

    // State Transitions
    void setState(GameState newState);

    // Meta-Horror Trigger (Debug)
    void triggerGlitch();
    void triggerFileCreation();
    void triggerWindowManipulation();
    void triggerJumpscare();
    void triggerFakeBSOD();

private:
    // Core Update/Render
    void handleEvents();
    void update(float deltaTime);
    void render();

    // State-specific methods
    void updateMainMenu(float deltaTime);
    void updatePlaying(float deltaTime);
    void updatePaused(float deltaTime);
    void updateDialog(float deltaTime);
    void updateFakeCrash(float deltaTime);

    void renderMainMenu();
    void renderPlaying();
    void renderPaused();
    void renderDialog();
    void renderFakeCrash();

    // SDL Components
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

    // Game State
    GameState m_currentState;
    bool m_isRunning;
    bool m_isFullscreen;
    int m_windowPosX;
    int m_windowPosY;

    // Core Subsystems
    std::unique_ptr<MetaHorror> m_metaHorror;
    std::unique_ptr<PermissionManager> m_permissionManager;
    std::unique_ptr<CorruptionSystem> m_corruptionSystem;
    std::unique_ptr<AudioManager> m_audioManager;

    // Gameplay
    std::unique_ptr<Player> m_player;
    std::unique_ptr<Level> m_level;

    // UI & Menus
    std::unique_ptr<MainMenu> m_mainMenu;
    std::unique_ptr<PauseMenu> m_pauseMenu;
    std::unique_ptr<DialogSystem> m_dialogSystem;

    // Horror Systems
    std::unique_ptr<FakeBlueScreen> m_fakeBlueScreen;
    std::unique_ptr<JumpscareSystem> m_jumpscareSystem;

    // Game Timers
    Uint32 m_lastFrameTime;
    float m_gameTime; // Time since game started (PLAYING state)
    float m_totalTime; // Total time since app start

    // Horror State
    int m_corruptionLevel; // 0-100, steigt mit Zeit

    // Level Progression
    int m_currentLevel; // Current level number (1-10)
    std::vector<std::string> m_collectedCodeFragments; // Collected code fragments for true ending
    NPC* m_activeNPC; // Currently interacting NPC

    // Helper methods
    void checkNPCInteractions();
    void checkCodeFragmentCollection();
    void checkGoalPortal();
    void advanceToNextLevel();

    // Constants
    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 600;
    static constexpr const char* WINDOW_TITLE = "ECHOES - The Forgotten Platformer";
};
