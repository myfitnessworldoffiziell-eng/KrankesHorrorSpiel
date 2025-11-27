#include "Game.h"
#include "MetaHorror.h"
#include "PermissionManager.h"
#include "CorruptionSystem.h"
#include "Player.h"
#include "Level.h"
#include <iostream>
#include <cmath>

Game::Game()
    : m_window(nullptr)
    , m_renderer(nullptr)
    , m_isRunning(false)
    , m_isFullscreen(false)
    , m_windowPosX(SDL_WINDOWPOS_CENTERED)
    , m_windowPosY(SDL_WINDOWPOS_CENTERED)
    , m_lastFrameTime(0)
    , m_gameTime(0.0f)
    , m_corruptionLevel(0)
    , m_glitchActive(false)
{
}

Game::~Game() {
    shutdown();
}

bool Game::initialize() {
    std::cout << "Initializing ECHOES.exe..." << std::endl;

    // SDL initialisieren
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
        std::cerr << "SDL konnte nicht initialisiert werden: " << SDL_GetError() << std::endl;
        return false;
    }

    // SDL_image initialisieren
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image konnte nicht initialisiert werden: " << IMG_GetError() << std::endl;
        return false;
    }

    // SDL_mixer initialisieren
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer konnte nicht initialisiert werden: " << Mix_GetError() << std::endl;
        return false;
    }

    // SDL_ttf initialisieren
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf konnte nicht initialisiert werden: " << TTF_GetError() << std::endl;
        return false;
    }

    // Fenster erstellen
    m_window = SDL_CreateWindow(
        WINDOW_TITLE,
        m_windowPosX, m_windowPosY,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!m_window) {
        std::cerr << "Fenster konnte nicht erstellt werden: " << SDL_GetError() << std::endl;
        return false;
    }

    // Renderer erstellen
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_renderer) {
        std::cerr << "Renderer konnte nicht erstellt werden: " << SDL_GetError() << std::endl;
        return false;
    }

    // Subsysteme initialisieren
    m_metaHorror = std::make_unique<MetaHorror>();
    m_permissionManager = std::make_unique<PermissionManager>();
    m_corruptionSystem = std::make_unique<CorruptionSystem>(m_renderer);
    m_player = std::make_unique<Player>();
    m_level = std::make_unique<Level>();

    // Meta-Horror initialisieren (erstellt erste Dateien)
    m_metaHorror->initialize();

    m_isRunning = true;
    m_lastFrameTime = SDL_GetTicks();

    std::cout << "Initialisierung abgeschlossen!" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "WILLKOMMEN zu ECHOES!" << std::endl;
    std::cout << "Ein nostalgisches Platformer-Spiel..." << std::endl;
    std::cout << "==================================" << std::endl;

    return true;
}

void Game::run() {
    while (m_isRunning) {
        // Delta Time berechnen
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - m_lastFrameTime) / 1000.0f;
        m_lastFrameTime = currentTime;
        m_gameTime += deltaTime;

        handleEvents();
        update(deltaTime);
        render();

        // FPS Cap (optional, wenn kein VSync)
        SDL_Delay(1);
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                m_isRunning = false;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        m_isRunning = false;
                        break;
                    case SDLK_F11:
                        // Fullscreen Toggle
                        m_isFullscreen = !m_isFullscreen;
                        SDL_SetWindowFullscreen(m_window, m_isFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
                        break;
                    case SDLK_g:
                        // Debug: Trigger Glitch
                        triggerGlitch();
                        break;
                    case SDLK_f:
                        // Debug: Create Files
                        triggerFileCreation();
                        break;
                    case SDLK_w:
                        // Debug: Window Manipulation
                        triggerWindowManipulation();
                        break;
                }
                break;
        }
    }
}

void Game::update(float deltaTime) {
    // Corruption Level steigt langsam mit Zeit (horror steigert sich)
    m_corruptionLevel = static_cast<int>(std::min(100.0f, m_gameTime / 2.0f)); // 200 Sekunden bis max

    // Corruption System Update
    m_corruptionSystem->update(deltaTime, m_corruptionLevel);

    // Player Update
    m_player->update(deltaTime);

    // Level Update
    m_level->update(deltaTime);

    // Meta-Horror Events (random, basierend auf corruption level)
    m_metaHorror->update(deltaTime, m_corruptionLevel);

    // Nach 30 Sekunden: Erste Kamera-Permission-Anfrage
    if (m_gameTime > 30.0f && !m_permissionManager->hasCameraPermission()) {
        static bool askedOnce = false;
        if (!askedOnce) {
            m_permissionManager->requestCameraPermission();
            askedOnce = true;
        }
    }
}

void Game::render() {
    // Clear screen
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    // Render Level
    m_level->render(m_renderer);

    // Render Player
    m_player->render(m_renderer);

    // Corruption Effects (Overlay)
    m_corruptionSystem->render(m_renderer, m_corruptionLevel);

    // Present
    SDL_RenderPresent(m_renderer);
}

void Game::shutdown() {
    std::cout << "Shutting down ECHOES.exe..." << std::endl;

    // Meta-Horror Cleanup (erstellt letzte Dateien)
    if (m_metaHorror) {
        m_metaHorror->onGameExit();
    }

    // Subsysteme freigeben
    m_level.reset();
    m_player.reset();
    m_corruptionSystem.reset();
    m_permissionManager.reset();
    m_metaHorror.reset();

    // SDL Cleanup
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }

    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();

    std::cout << "Goodbye..." << std::endl;
}

void Game::triggerGlitch() {
    std::cout << "[DEBUG] Glitch triggered!" << std::endl;
    m_corruptionSystem->forceGlitch();
}

void Game::triggerFileCreation() {
    std::cout << "[DEBUG] Creating creepy files..." << std::endl;
    m_metaHorror->createCreepyFile();
}

void Game::triggerWindowManipulation() {
    std::cout << "[DEBUG] Window manipulation..." << std::endl;
    m_metaHorror->manipulateWindow(m_window);
}
