#include "Game.h"
#include "MetaHorror.h"
#include "PermissionManager.h"
#include "CorruptionSystem.h"
#include "Player.h"
#include "Level.h"
#include "AudioManager.h"
#include "UIHelper.h"
#include "MainMenu.h"
#include "PauseMenu.h"
#include "DialogSystem.h"
#include "FakeBlueScreen.h"
#include "JumpscareSystem.h"
#include "NPC.h"
#include "Boss.h"
#include <iostream>
#include <cmath>

Game::Game()
    : m_window(nullptr)
    , m_renderer(nullptr)
    , m_currentState(GameState::MAIN_MENU)
    , m_isRunning(false)
    , m_isFullscreen(false)
    , m_windowPosX(SDL_WINDOWPOS_CENTERED)
    , m_windowPosY(SDL_WINDOWPOS_CENTERED)
    , m_lastFrameTime(0)
    , m_gameTime(0.0f)
    , m_totalTime(0.0f)
    , m_corruptionLevel(0)
    , m_currentLevel(1)
    , m_activeNPC(nullptr)
{
}

Game::~Game() {
    shutdown();
}

bool Game::initialize() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Initializing ECHOES.exe" << std::endl;
    std::cout << "========================================" << std::endl;

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

    // UI Helper initialisieren
    UIHelper::init();

    // Subsysteme initialisieren
    std::cout << "Initializing subsystems..." << std::endl;

    m_metaHorror = std::make_unique<MetaHorror>();
    m_permissionManager = std::make_unique<PermissionManager>();
    m_corruptionSystem = std::make_unique<CorruptionSystem>(m_renderer);
    m_audioManager = std::make_unique<AudioManager>();
    m_player = std::make_unique<Player>();
    m_player->setAudioManager(m_audioManager.get());  // Connect audio to player
    m_level = std::make_unique<Level>();
    m_mainMenu = std::make_unique<MainMenu>();
    m_pauseMenu = std::make_unique<PauseMenu>();
    m_dialogSystem = std::make_unique<DialogSystem>();
    m_fakeBlueScreen = std::make_unique<FakeBlueScreen>();
    m_jumpscareSystem = std::make_unique<JumpscareSystem>(m_audioManager.get());

    // Meta-Horror initialisieren (erstellt erste Dateien)
    m_metaHorror->initialize();

    // Audio Manager initialisieren
    m_audioManager->initialize();

    // Audio-Dateien laden (14 sounds total)
    std::cout << "Loading audio files..." << std::endl;

    // Music (4 tracks)
    m_audioManager->loadMusic("menu", "assets/audio/menu_music.mp3");
    m_audioManager->loadMusic("level", "assets/audio/level_music.mp3");
    m_audioManager->loadMusic("corrupted", "assets/audio/corrupted_music.mp3");
    m_audioManager->loadMusic("horror_ambient", "assets/audio/horror_ambient.mp3");

    // SFX - Gameplay (6 sounds)
    m_audioManager->loadSound("jump", "assets/audio/jump.wav");
    m_audioManager->loadSound("collect", "assets/audio/collect.wav");
    m_audioManager->loadSound("death", "assets/audio/death.wav");
    m_audioManager->loadSound("hit", "assets/audio/hit.wav");
    m_audioManager->loadSound("chirp", "assets/audio/chirp.wav");
    m_audioManager->loadSound("zap", "assets/audio/zap.wav");

    // Horror SFX (4 sounds)
    m_audioManager->loadSound("jumpscare", "assets/audio/jumpscare.wav");
    m_audioManager->loadSound("whitenoise", "assets/audio/whitenoise.wav");
    m_audioManager->loadSound("scare_appear", "assets/audio/scare_appear.wav");
    m_audioManager->loadSound("scare_audio", "assets/audio/scare_audio.wav");

    std::cout << "✓ Loaded 14 audio files (4 music + 10 SFX)" << std::endl;

    m_isRunning = true;
    m_lastFrameTime = SDL_GetTicks();

    std::cout << "========================================" << std::endl;
    std::cout << "  Initialization complete!" << std::endl;
    std::cout << "========================================" << std::endl;

    // Start menu music
    m_audioManager->playMusic("menu");

    return true;
}

void Game::run() {
    while (m_isRunning) {
        // Delta Time berechnen
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - m_lastFrameTime) / 1000.0f;
        m_lastFrameTime = currentTime;
        m_totalTime += deltaTime;

        // Cap delta time (prevent huge jumps)
        if (deltaTime > 0.1f) deltaTime = 0.1f;

        handleEvents();
        update(deltaTime);
        render();

        // FPS Cap (optional)
        SDL_Delay(1);
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Global events
        if (event.type == SDL_QUIT) {
            m_isRunning = false;
            return;
        }

        // State-specific input handling
        switch (m_currentState) {
            case GameState::MAIN_MENU:
                m_mainMenu->handleInput(event);
                break;

            case GameState::PLAYING:
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    setState(GameState::PAUSED);
                }
                // E key for NPC interaction
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_e) {
                    // Check if near an NPC
                    for (auto& npc : m_level->getNPCs()) {
                        if (npc && npc->isPlayerNearby(m_player->getX(), m_player->getY())) {
                            // Start dialogue with this NPC
                            npc->interact();
                            m_activeNPC = npc.get();

                            // Show first dialogue line
                            if (npc->hasDialogue()) {
                                m_dialogSystem->addDialog("NPC", npc->getCurrentLine(), 0.0f);
                                m_dialogSystem->start();
                                setState(GameState::DIALOG);
                            }
                            break;
                        }
                    }
                }
                // Debug keys
                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_g: triggerGlitch(); break;
                        case SDLK_f: triggerFileCreation(); break;
                        case SDLK_w: triggerWindowManipulation(); break;
                        case SDLK_j: triggerJumpscare(); break;
                        case SDLK_b: triggerFakeBSOD(); break;
                        case SDLK_F11:
                            m_isFullscreen = !m_isFullscreen;
                            SDL_SetWindowFullscreen(m_window, m_isFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
                            break;
                    }
                }
                break;

            case GameState::PAUSED:
                m_pauseMenu->handleInput(event);
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    setState(GameState::PLAYING);
                }
                break;

            case GameState::DIALOG:
                m_dialogSystem->handleInput(event);
                // Allow advancing NPC dialogue with E or Enter
                if (event.type == SDL_KEYDOWN &&
                    (event.key.keysym.sym == SDLK_e || event.key.keysym.sym == SDLK_RETURN)) {
                    if (m_activeNPC && m_activeNPC->hasMoreLines()) {
                        m_activeNPC->nextLine();
                        m_dialogSystem->addDialog("NPC", m_activeNPC->getCurrentLine(), 0.0f);
                    } else {
                        // No more dialogue, clear active NPC
                        m_activeNPC = nullptr;
                    }
                }
                break;

            case GameState::FAKE_CRASH:
                // Allow skip with any key
                if (event.type == SDL_KEYDOWN) {
                    m_fakeBlueScreen->skip();
                }
                break;

            default:
                break;
        }
    }
}

void Game::update(float deltaTime) {
    // Update audio system (global)
    m_audioManager->update(deltaTime, m_corruptionLevel);

    // State-specific updates
    switch (m_currentState) {
        case GameState::MAIN_MENU:
            updateMainMenu(deltaTime);
            break;
        case GameState::PLAYING:
            updatePlaying(deltaTime);
            break;
        case GameState::PAUSED:
            updatePaused(deltaTime);
            break;
        case GameState::DIALOG:
            updateDialog(deltaTime);
            break;
        case GameState::FAKE_CRASH:
            updateFakeCrash(deltaTime);
            break;
        default:
            break;
    }
}

void Game::render() {
    // Clear
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    // State-specific rendering
    switch (m_currentState) {
        case GameState::MAIN_MENU:
            renderMainMenu();
            break;
        case GameState::PLAYING:
            renderPlaying();
            break;
        case GameState::PAUSED:
            renderPlaying(); // Render game in background
            renderPaused();  // Then overlay pause menu
            break;
        case GameState::DIALOG:
            renderPlaying(); // Render game in background
            renderDialog();  // Then overlay dialog
            break;
        case GameState::FAKE_CRASH:
            renderFakeCrash();
            break;
        default:
            break;
    }

    // Present
    SDL_RenderPresent(m_renderer);
}

void Game::updateMainMenu(float deltaTime) {
    auto result = m_mainMenu->update(deltaTime);

    switch (result) {
        case MainMenu::MenuResult::START_GAME:
            setState(GameState::PLAYING);
            break;
        case MainMenu::MenuResult::SETTINGS:
            // TODO: Settings menu
            std::cout << "[Game] Settings not implemented yet" << std::endl;
            break;
        case MainMenu::MenuResult::QUIT:
            m_isRunning = false;
            break;
        default:
            break;
    }
}

void Game::updatePlaying(float deltaTime) {
    m_gameTime += deltaTime;

    // Corruption Level steigt langsam mit Zeit
    m_corruptionLevel = static_cast<int>(std::min(100.0f, m_gameTime / 2.0f)); // 200 Sekunden bis max

    // Corruption System Update
    m_corruptionSystem->update(deltaTime, m_corruptionLevel);

    // Player Update
    m_player->update(deltaTime);

    // Level Update (now includes player for boss AI)
    m_level->update(deltaTime, m_player.get());

    // === COLLISION DETECTION ===

    // Player vs Enemies
    if (m_player->isAlive()) {
        SDL_Rect playerBounds = m_player->getBounds();

        for (auto& enemy : m_level->getEnemies()) {
            if (enemy && enemy->isAlive()) {
                SDL_Rect enemyBounds = enemy->getBounds();

                // Check collision
                if (SDL_HasIntersection(&playerBounds, &enemyBounds)) {
                    // Check if player is jumping on enemy
                    bool jumpedOn = (m_player->getY() + 32 < enemy->getY() + 10);

                    if (jumpedOn && enemy->canBeJumpedOn()) {
                        // Player defeats enemy by jumping on it
                        enemy->kill();
                        m_audioManager->playSound("collect", 100);  // Victory sound
                        std::cout << "[Game] Enemy defeated!" << std::endl;
                    } else {
                        // Enemy damages player
                        m_player->takeDamage(enemy->getDamage());
                        std::cout << "[Game] Player hit! Health: " << m_player->getHealth() << std::endl;
                    }
                }
            }
        }

        // Player vs Stars (collectibles)
        for (auto& star : m_level->getStars()) {
            if (!star.collected) {
                // Simple distance check
                float dx = m_player->getX() - star.x;
                float dy = m_player->getY() - star.y;
                float distance = std::sqrt(dx * dx + dy * dy);

                if (distance < 30.0f) {  // Collection radius
                    star.collected = true;
                    m_audioManager->playSound("collect", 120);
                    std::cout << "[Game] Star collected!" << std::endl;
                }
            }
        }

        // Check for code fragment collection
        checkCodeFragmentCollection();

        // Check for goal portal entry
        checkGoalPortal();
    }

    // Check if player died
    if (!m_player->isAlive()) {
        std::cout << "[Game] Player died! Respawning..." << std::endl;
        // TODO: Game over screen or respawn
        // For now, respawn at start
        m_player->setPosition(50.0f, 400.0f);
        m_player->takeDamage(-100);  // Restore health (hack for now)
        m_level->loadLevel(m_level->getLevelNumber(), m_audioManager.get());  // Reload level
    }

    // Meta-Horror Events
    m_metaHorror->update(deltaTime, m_corruptionLevel);

    // Jumpscare System
    m_jumpscareSystem->update(deltaTime, m_corruptionLevel);

    // Nach 30 Sekunden: Erste creepy Dialog-Sequenz
    static bool firstDialogShown = false;
    if (m_gameTime > 30.0f && !firstDialogShown && !m_dialogSystem->isActive()) {
        m_dialogSystem->addDialog("", "Something feels... off.", 3.0f);
        m_dialogSystem->addDialog("Echo", "Welcome, player.", 2.0f);
        m_dialogSystem->addDialog("Echo", "I've been waiting for you...", 0.0f);
        m_dialogSystem->start();
        setState(GameState::DIALOG);
        firstDialogShown = true;
    }

    // Nach 60 Sekunden: Kamera-Permission
    static bool cameraRequested = false;
    if (m_gameTime > 60.0f && !cameraRequested) {
        if (!m_permissionManager->hasCameraPermission()) {
            m_permissionManager->requestCameraPermission();
            cameraRequested = true;
        }
    }

    // Nach 120 Sekunden: Fake BSOD
    static bool bsodShown = false;
    if (m_gameTime > 120.0f && !bsodShown) {
        triggerFakeBSOD();
        bsodShown = true;
    }
}

void Game::updatePaused(float deltaTime) {
    auto result = m_pauseMenu->update(deltaTime);

    switch (result) {
        case PauseMenu::PauseResult::RESUME:
            setState(GameState::PLAYING);
            break;
        case PauseMenu::PauseResult::MAIN_MENU:
            setState(GameState::MAIN_MENU);
            break;
        case PauseMenu::PauseResult::QUIT:
            m_isRunning = false;
            break;
        default:
            break;
    }
}

void Game::updateDialog(float deltaTime) {
    m_dialogSystem->update(deltaTime);

    // Return to PLAYING when dialog finished
    if (!m_dialogSystem->isActive()) {
        setState(GameState::PLAYING);
    }
}

void Game::updateFakeCrash(float deltaTime) {
    m_fakeBlueScreen->update(deltaTime);

    // Return to PLAYING when BSOD finished
    if (!m_fakeBlueScreen->isActive()) {
        setState(GameState::PLAYING);
    }
}

void Game::renderMainMenu() {
    m_mainMenu->render(m_renderer);
}

void Game::renderPlaying() {
    // Level
    m_level->render(m_renderer);

    // Player
    m_player->render(m_renderer);

    // === UI OVERLAY ===

    // Health Bar (top left)
    int healthBarX = 10;
    int healthBarY = 10;
    int healthBarWidth = 200;
    int healthBarHeight = 20;

    // Health bar background (dark)
    SDL_SetRenderDrawColor(m_renderer, 50, 50, 50, 200);
    SDL_Rect healthBg = {healthBarX, healthBarY, healthBarWidth, healthBarHeight};
    SDL_RenderFillRect(m_renderer, &healthBg);

    // Health bar fill (red to green)
    int healthPercent = (m_player->getHealth() * 100) / m_player->getMaxHealth();
    int fillWidth = (healthBarWidth * healthPercent) / 100;

    // Color based on health
    Uint8 r = 255 - (healthPercent * 255 / 100);
    Uint8 g = (healthPercent * 255 / 100);
    SDL_SetRenderDrawColor(m_renderer, r, g, 0, 255);
    SDL_Rect healthFill = {healthBarX, healthBarY, fillWidth, healthBarHeight};
    SDL_RenderFillRect(m_renderer, &healthFill);

    // Health bar border
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(m_renderer, &healthBg);

    // Health text
    UIHelper::renderText(m_renderer, "HP: " + std::to_string(m_player->getHealth()),
                        healthBarX + healthBarWidth + 10, healthBarY, 18, 255, 255, 255);

    // Star Counter (top right)
    int starsCollected = 0;
    int totalStars = 0;
    for (const auto& star : m_level->getStars()) {
        totalStars++;
        if (star.collected) starsCollected++;
    }

    std::string starText = "⭐ " + std::to_string(starsCollected) + "/" + std::to_string(totalStars);
    UIHelper::renderText(m_renderer, starText, WINDOW_WIDTH - 100, 10, 20, 255, 220, 0);

    // Level indicator
    std::string levelText = "Level " + std::to_string(m_currentLevel);
    UIHelper::renderText(m_renderer, levelText, WINDOW_WIDTH / 2 - 40, 10, 18, 255, 255, 255);

    // Code Fragment indicator (top left, below health)
    if (!m_collectedCodeFragments.empty()) {
        std::string fragmentText = "Code: " + std::to_string(m_collectedCodeFragments.size()) + "/3";
        UIHelper::renderText(m_renderer, fragmentText, 10, 40, 16, 0, 255, 255);
    }

    // NPC Interaction hint (center bottom)
    for (const auto& npc : m_level->getNPCs()) {
        if (npc && npc->isPlayerNearby(m_player->getX(), m_player->getY())) {
            UIHelper::renderText(m_renderer, "[E] Talk", WINDOW_WIDTH / 2 - 30, WINDOW_HEIGHT - 40, 18, 255, 255, 0);
            break;
        }
    }

    // Corruption Effects (Overlay)
    m_corruptionSystem->render(m_renderer, m_corruptionLevel);

    // Jumpscare (if active)
    m_jumpscareSystem->render(m_renderer);
}

void Game::renderPaused() {
    m_pauseMenu->render(m_renderer);
}

void Game::renderDialog() {
    m_dialogSystem->render(m_renderer);
}

void Game::renderFakeCrash() {
    m_fakeBlueScreen->render(m_renderer);
}

void Game::setState(GameState newState) {
    if (m_currentState == newState) return;

    std::cout << "[Game] State transition: ";
    switch (m_currentState) {
        case GameState::MAIN_MENU: std::cout << "MAIN_MENU"; break;
        case GameState::PLAYING: std::cout << "PLAYING"; break;
        case GameState::PAUSED: std::cout << "PAUSED"; break;
        case GameState::DIALOG: std::cout << "DIALOG"; break;
        case GameState::FAKE_CRASH: std::cout << "FAKE_CRASH"; break;
        default: std::cout << "UNKNOWN"; break;
    }
    std::cout << " -> ";
    switch (newState) {
        case GameState::MAIN_MENU: std::cout << "MAIN_MENU"; break;
        case GameState::PLAYING: std::cout << "PLAYING"; break;
        case GameState::PAUSED: std::cout << "PAUSED"; break;
        case GameState::DIALOG: std::cout << "DIALOG"; break;
        case GameState::FAKE_CRASH: std::cout << "FAKE_CRASH"; break;
        default: std::cout << "UNKNOWN"; break;
    }
    std::cout << std::endl;

    GameState oldState = m_currentState;
    m_currentState = newState;

    // State entry actions
    switch (newState) {
        case GameState::MAIN_MENU:
            m_audioManager->playMusic("menu");
            m_gameTime = 0.0f; // Reset game time
            break;

        case GameState::PLAYING:
            if (oldState == GameState::MAIN_MENU) {
                // Start new game
                m_currentLevel = 1;
                m_collectedCodeFragments.clear();
                m_activeNPC = nullptr;
                m_audioManager->playMusic("level");
                m_level->loadLevel(m_currentLevel, m_audioManager.get());  // Load Level 1
                m_player->setPosition(50.0f, 400.0f);         // Reset player position
                std::cout << "[Game] Started new game - Level 1: Welcome to Paradise" << std::endl;
            } else if (oldState == GameState::PAUSED) {
                m_audioManager->resumeMusic();
            }
            break;

        case GameState::PAUSED:
            m_audioManager->pauseMusic();
            break;

        case GameState::DIALOG:
            // Keep game music playing
            break;

        case GameState::FAKE_CRASH:
            m_audioManager->stopMusic();
            break;

        default:
            break;
    }
}

void Game::shutdown() {
    std::cout << "========================================" << std::endl;
    std::cout << "  Shutting down ECHOES.exe" << std::endl;
    std::cout << "========================================" << std::endl;

    // Meta-Horror Cleanup (erstellt letzte Dateien)
    if (m_metaHorror) {
        m_metaHorror->onGameExit();
    }

    // Subsysteme freigeben
    m_jumpscareSystem.reset();
    m_fakeBlueScreen.reset();
    m_dialogSystem.reset();
    m_pauseMenu.reset();
    m_mainMenu.reset();
    m_level.reset();
    m_player.reset();
    m_audioManager.reset();
    m_corruptionSystem.reset();
    m_permissionManager.reset();
    m_metaHorror.reset();

    // UI Helper shutdown
    UIHelper::shutdown();

    // SDL Cleanup
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }

    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    Mix_CloseAudio();
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();

    std::cout << "Goodbye..." << std::endl;
    std::cout << "Or is it?" << std::endl;
}

// Debug triggers
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

void Game::triggerJumpscare() {
    std::cout << "[DEBUG] Jumpscare!" << std::endl;
    m_jumpscareSystem->triggerRandomScare();
}

void Game::triggerFakeBSOD() {
    std::cout << "[DEBUG] Fake Blue Screen!" << std::endl;
    m_fakeBlueScreen->trigger(10.0f); // 10 seconds
    setState(GameState::FAKE_CRASH);
}

// ========================================
// Level Progression & Interaction Methods
// ========================================

void Game::checkNPCInteractions() {
    // This is now handled in handleEvents() for E key press
    // Could be used for auto-triggers in future
}

void Game::checkCodeFragmentCollection() {
    for (auto& fragment : m_level->getCodeFragments()) {
        if (!fragment.collected) {
            // Check distance to player
            float dx = m_player->getX() - fragment.x;
            float dy = m_player->getY() - fragment.y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance < 30.0f) {  // Collection radius
                fragment.collected = true;
                m_collectedCodeFragments.push_back(fragment.code);

                m_audioManager->playSound("collect", 150);
                std::cout << "[Game] CODE FRAGMENT COLLECTED: \"" << fragment.code << "\"" << std::endl;
                std::cout << "[Game] Total fragments: " << m_collectedCodeFragments.size() << "/3" << std::endl;

                // Special message if all 3 collected
                if (m_collectedCodeFragments.size() == 3) {
                    std::cout << "[Game] *** ALL CODE FRAGMENTS COLLECTED! TRUE ENDING UNLOCKED! ***" << std::endl;
                    m_dialogSystem->addDialog("", "The code is complete...", 3.0f);
                    m_dialogSystem->addDialog("", "F I R S T  E N D", 0.0f);
                    m_dialogSystem->start();
                    setState(GameState::DIALOG);
                }
            }
        }
    }
}

void Game::checkGoalPortal() {
    const auto& goal = m_level->getGoal();

    // Only check if goal is active
    if (!goal.active) return;

    // Check player collision with goal portal
    SDL_Rect playerBounds = m_player->getBounds();
    SDL_Rect goalBounds = {
        static_cast<int>(goal.x),
        static_cast<int>(goal.y),
        static_cast<int>(goal.width),
        static_cast<int>(goal.height)
    };

    if (SDL_HasIntersection(&playerBounds, &goalBounds)) {
        std::cout << "[Game] Player entered goal portal!" << std::endl;
        advanceToNextLevel();
    }
}

void Game::advanceToNextLevel() {
    m_currentLevel++;

    std::cout << "========================================" << std::endl;
    std::cout << "  ADVANCING TO LEVEL " << m_currentLevel << std::endl;
    std::cout << "========================================" << std::endl;

    if (m_currentLevel > 10) {
        // Game complete!
        std::cout << "[Game] *** GAME COMPLETE! ***" << std::endl;

        // Check ending type
        if (m_collectedCodeFragments.size() == 3) {
            std::cout << "[Game] ENDING C: The Developer's Escape (TRUE ENDING)" << std::endl;
            m_dialogSystem->addDialog("Echo", "You... found the code.", 3.0f);
            m_dialogSystem->addDialog("Echo", "The one Marcus left behind.", 3.0f);
            m_dialogSystem->addDialog("Echo", "F I R S T   E N D", 3.0f);
            m_dialogSystem->addDialog("", "You are free.", 0.0f);
        } else {
            std::cout << "[Game] ENDING A: The Cycle Continues" << std::endl;
            m_dialogSystem->addDialog("Echo", "You finished the game.", 3.0f);
            m_dialogSystem->addDialog("Echo", "But did you really?", 3.0f);
            m_dialogSystem->addDialog("", "The game restarts...", 0.0f);
        }

        m_dialogSystem->start();
        setState(GameState::DIALOG);

        // Reset to level 1 after completing
        m_currentLevel = 1;
        m_level->loadLevel(m_currentLevel, m_audioManager.get());
        m_player->setPosition(50.0f, 400.0f);
    } else {
        // Load next level
        m_level->loadLevel(m_currentLevel, m_audioManager.get());
        m_player->setPosition(50.0f, 400.0f);  // Reset player position

        // Level-specific music changes
        if (m_currentLevel >= 7) {
            m_audioManager->playMusic("horror_ambient");
        } else if (m_currentLevel >= 4) {
            m_audioManager->playMusic("corrupted");
        }

        std::cout << "[Game] Loaded Level " << m_currentLevel << std::endl;
    }
}
