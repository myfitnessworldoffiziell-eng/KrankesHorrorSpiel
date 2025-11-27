#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <string>

class MetaHorror;
class PermissionManager;
class CorruptionSystem;
class Player;
class Level;

/**
 * Haupt-Game-Klasse für ECHOES.exe
 * Verwaltet den Game Loop, Rendering und alle Subsysteme
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

    // Meta-Horror Trigger
    void triggerGlitch();
    void triggerFileCreation();
    void triggerWindowManipulation();

private:
    // Core Update/Render
    void handleEvents();
    void update(float deltaTime);
    void render();

    // SDL Components
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

    // Game State
    bool m_isRunning;
    bool m_isFullscreen;
    int m_windowPosX;
    int m_windowPosY;

    // Subsystems
    std::unique_ptr<MetaHorror> m_metaHorror;
    std::unique_ptr<PermissionManager> m_permissionManager;
    std::unique_ptr<CorruptionSystem> m_corruptionSystem;
    std::unique_ptr<Player> m_player;
    std::unique_ptr<Level> m_level;

    // Game Timers
    Uint32 m_lastFrameTime;
    float m_gameTime;

    // Horror State
    int m_corruptionLevel; // 0-100, steigt mit Zeit
    bool m_glitchActive;

    // Constants
    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 600;
    static constexpr const char* WINDOW_TITLE = "ECHOES - The Forgotten Platformer";
};
