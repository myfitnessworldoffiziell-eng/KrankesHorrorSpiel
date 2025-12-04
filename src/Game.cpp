#include "Game.h"
#include "MetaHorror.h"
#include "PermissionManager.h"
#include "CorruptionSystem.h"
#include "Player.h"
#include "Level.h"
#include "Enemy.h"
#include "AudioManager.h"
#include "TextureManager.h"
#include "UIHelper.h"
#include "MainMenu.h"
#include "SettingsMenu.h"
#include "PauseMenu.h"
#include "DialogSystem.h"
#include "FakeBlueScreen.h"
#include "JumpscareSystem.h"
#include "NPC.h"
#include "Boss.h"
#include "GlitchBoss.h"
#include "EchoPrime.h"
#include "BossDefeatHorror.h"
#include "PCHorror.h"
#include "ExtremePCHorror.h"
#include "GameOverScreen.h"
#include "CreditsScreen.h"
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
    , m_level3HorrorTriggered(false)
    , m_level5HorrorTriggered(false)
    , m_level7HorrorTriggered(false)
    , m_level9HorrorTriggered(false)
    , m_levelHorrorTimer(0.0f)
    , m_level4ExtremeTriggered(false)
    , m_level6ExtremeTriggered(false)
    , m_level8ExtremeTriggered(false)
    , m_level10ExtremeTriggered(false)
    , m_currentLevel(1)
    , m_activeNPC(nullptr)
    , m_deathCount(0)
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
    m_settingsMenu = std::make_unique<SettingsMenu>(m_audioManager.get());
    m_pauseMenu = std::make_unique<PauseMenu>();
    m_dialogSystem = std::make_unique<DialogSystem>();
    m_gameOverScreen = std::make_unique<GameOverScreen>(m_audioManager.get());
    m_creditsScreen = std::make_unique<CreditsScreen>();
    m_fakeBlueScreen = std::make_unique<FakeBlueScreen>();
    m_jumpscareSystem = std::make_unique<JumpscareSystem>(m_audioManager.get());
    m_bossDefeatHorror = std::make_unique<BossDefeatHorror>(m_audioManager.get(), m_permissionManager.get());
    m_pcHorror = std::make_unique<PCHorror>(m_window, m_audioManager.get(), m_metaHorror.get());
    m_extremePCHorror = std::make_unique<ExtremePCHorror>(m_window, m_audioManager.get(), m_metaHorror.get());

    // Meta-Horror initialisieren (erstellt erste Dateien)
    m_metaHorror->initialize();

    // Audio Manager initialisieren
    m_audioManager->initialize();

    // Audio-Dateien laden (14 sounds total)
    std::cout << "Loading audio files..." << std::endl;

    // Music (4 tracks)
    m_audioManager->loadMusic("menu", "assets/audio/horror_ambient.mp3");  // 👻 Creepy menu music!
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

    // Horror SFX (5 sounds)
    m_audioManager->loadSound("jumpscare", "assets/audio/jumpscare.wav");
    m_audioManager->loadSound("whitenoise", "assets/audio/whitenoise.wav");
    m_audioManager->loadSound("scare_appear", "assets/audio/scare_appear.wav");
    m_audioManager->loadSound("scare_audio", "assets/audio/scare_audio.wav");
    m_audioManager->loadSound("glitch", "assets/audio/glitch.wav");  // 👻 Procedurally generated glitch!

    std::cout << "✓ Loaded 15 audio files (4 music + 11 SFX)" << std::endl;

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

            case GameState::SETTINGS:
                m_settingsMenu->handleInput(event);
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    setState(GameState::MAIN_MENU);
                }
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

            case GameState::GAME_OVER:
                // Handle game over input
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_SPACE) {
                        // Respawn player in SAME level with FULL health
                        std::cout << "[Game] Player respawning in Level " << m_currentLevel << "..." << std::endl;

                        // FULL player reset (health + state)
                        m_player->reset();
                        m_player->setPosition(50.0f, 400.0f);

                        // Reset level-specific horror triggers
                        m_level3HorrorTriggered = false;
                        m_level5HorrorTriggered = false;
                        m_level7HorrorTriggered = false;
                        m_level9HorrorTriggered = false;
                        m_level4ExtremeTriggered = false;
                        m_level6ExtremeTriggered = false;
                        m_level8ExtremeTriggered = false;
                        m_level10ExtremeTriggered = false;
                        m_levelHorrorTimer = 0.0f;

                        // Reload CURRENT level (not Level 1!)
                        m_level->loadLevel(m_currentLevel, m_audioManager.get());

                        // Reset game over screen
                        m_gameOverScreen->reset();

                        // Return to playing
                        setState(GameState::PLAYING);

                        // Resume normal music
                        m_audioManager->playMusic("level");
                    } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        // Quit to menu - RESET player for next game
                        std::cout << "[Game] Player quit from game over." << std::endl;
                        m_player->reset();  // Reset player state
                        m_currentLevel = 1;  // Reset to level 1
                        setState(GameState::MAIN_MENU);
                        m_gameOverScreen->reset();
                        m_audioManager->playMusic("menu");
                    }
                }
                break;

            case GameState::CREDITS:
                // Allow skip with ESC
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    setState(GameState::MAIN_MENU);
                    m_creditsScreen->reset();
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
        case GameState::SETTINGS:
            updateSettings(deltaTime);
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
        case GameState::BOSS_HORROR:
            updateBossHorror(deltaTime);
            break;
        case GameState::GAME_OVER:
            updateGameOver(deltaTime);
            break;
        case GameState::ENDING:
            updateEnding(deltaTime);
            break;
        case GameState::CREDITS:
            updateCredits(deltaTime);
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
        case GameState::SETTINGS:
            renderSettings();
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
        case GameState::BOSS_HORROR:
            renderBossHorror();
            break;
        case GameState::GAME_OVER:
            renderGameOver();
            break;
        case GameState::ENDING:
            renderEnding();
            break;
        case GameState::CREDITS:
            renderCredits();
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
            setState(GameState::SETTINGS);
            break;
        case MainMenu::MenuResult::QUIT:
            m_isRunning = false;
            break;
        default:
            break;
    }
}

void Game::updateSettings(float deltaTime) {
    m_settingsMenu->update(deltaTime);

    // Apply settings changes
    // Fullscreen setting
    bool newFullscreen = m_settingsMenu->isFullscreen();
    if (newFullscreen != m_isFullscreen) {
        m_isFullscreen = newFullscreen;
        SDL_SetWindowFullscreen(m_window, m_isFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
        std::cout << "[Game] Fullscreen: " << (m_isFullscreen ? "ON" : "OFF") << std::endl;
    }

    // Resolution changes would require window recreation (not implemented in this session)
    // Music and SFX volumes are handled automatically by SettingsMenu -> AudioManager
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

    // Player vs Platforms
    if (m_player->isAlive() && m_player->getVelocityY() > 0) {  // Only check when falling
        SDL_Rect playerBounds = m_player->getBounds();

        for (const auto& platform : m_level->getPlatforms()) {
            // Check if player is above platform and overlaps horizontally
            float playerBottom = m_player->getY() + 32.0f;  // Player height = 32
            float platformTop = platform.y;

            // Check horizontal overlap
            bool horizontalOverlap = (m_player->getX() + 32.0f > platform.x) &&
                                    (m_player->getX() < platform.x + platform.width);

            // Check if player just landed on platform (within 10 pixels)
            bool justAbove = (playerBottom >= platformTop) &&
                           (playerBottom < platformTop + 10.0f);

            if (horizontalOverlap && justAbove) {
                // Place player exactly on platform
                m_player->setY(platformTop - 32.0f);
                m_player->setVelocityY(0.0f);
                m_player->setOnGround(true);
                break;  // Stop checking other platforms
            }
        }
    }

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

        // Player vs Boss (and boss attacks)
        if (m_level->hasBoss()) {
            Boss* boss = m_level->getBoss();
            if (boss && boss->isAlive()) {
                SDL_Rect bossBounds = boss->getBounds();

                // Check collision with boss body
                if (SDL_HasIntersection(&playerBounds, &bossBounds)) {
                    // Check if player is jumping on boss (from above)
                    bool jumpedOnBoss = (m_player->getY() + 32 < boss->getY() + 20);

                    if (jumpedOnBoss && m_player->getVelocityY() > 0) {
                        // Player defeats boss by jumping on it!
                        boss->takeDamage(50);  // 50 damage per jump
                        m_player->setVelocityY(-300.0f);  // Bounce off boss
                        m_audioManager->playSound("hit", 120);
                        std::cout << "[Game] Jumped on boss! Boss HP: " << boss->getHP() << "/" << boss->getMaxHP() << std::endl;
                    } else if (!boss->isInvulnerable()) {
                        // Boss contact damages player (not invulnerable from jump)
                        m_player->takeDamage(20);  // Boss does significant damage
                        std::cout << "[Game] Hit by boss! Health: " << m_player->getHealth() << std::endl;
                    }
                }

                // Check collision with boss projectiles (GlitchBoss)
                if (auto* glitchBoss = dynamic_cast<GlitchBoss*>(boss)) {
                    for (const auto& proj : glitchBoss->getProjectiles()) {
                        if (!proj.active) continue;

                        SDL_Rect projBounds = {
                            static_cast<int>(proj.x) - 5,
                            static_cast<int>(proj.y) - 5,
                            10, 10
                        };

                        if (SDL_HasIntersection(&playerBounds, &projBounds)) {
                            m_player->takeDamage(10);  // Projectile damage
                            std::cout << "[Game] Hit by boss projectile! Health: " << m_player->getHealth() << std::endl;
                            // Note: We can't easily mark the projectile as inactive from here
                        }
                    }
                }

                // Check collision with Echo clones (EchoPrime)
                if (auto* echoPrime = dynamic_cast<EchoPrime*>(boss)) {
                    for (const auto& clone : echoPrime->getClones()) {
                        if (!clone.active) continue;

                        SDL_Rect cloneBounds = {
                            static_cast<int>(clone.x) - 20,
                            static_cast<int>(clone.y) - 20,
                            40, 40
                        };

                        if (SDL_HasIntersection(&playerBounds, &cloneBounds)) {
                            m_player->takeDamage(15);  // Clone damage
                            std::cout << "[Game] Hit by Echo clone! Health: " << m_player->getHealth() << std::endl;
                        }
                    }
                }
            }
        }

        // Check if boss was just defeated (trigger horror sequence!)
        if (m_level->hasBoss()) {
            Boss* boss = m_level->getBoss();
            if (boss && boss->isDefeated() && !m_bossDefeatHorror->isActive()) {
                std::cout << "[Game] 💀 BOSS DEFEATED - TRIGGERING EXTREME HORROR SEQUENCE! 💀" << std::endl;
                m_bossDefeatHorror->trigger(boss->getName());
                setState(GameState::BOSS_HORROR);
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
    if (!m_player->isAlive() && !m_gameOverScreen->isActive()) {
        std::cout << "💀 [Game] Player died! Triggering Game Over screen..." << std::endl;
        m_deathCount++;
        m_gameOverScreen->trigger(m_deathCount, m_currentLevel);
        setState(GameState::GAME_OVER);
    }

    // Meta-Horror Events
    m_metaHorror->update(deltaTime, m_corruptionLevel);

    // Jumpscare System
    m_jumpscareSystem->update(deltaTime, m_corruptionLevel);

    // PC Horror System (Update active effects)
    m_pcHorror->update(deltaTime);
    m_pcHorror->updateWindow();

    // Update level horror timer
    m_levelHorrorTimer += deltaTime;

    // PC Horror Triggers (Level-based escalation)
    // Level 3: First PC horror warnings
    if (m_currentLevel == 3 && !m_level3HorrorTriggered && m_levelHorrorTimer > 10.0f) {
        m_pcHorror->triggerRandomForLevel(3);
        m_level3HorrorTriggered = true;
        std::cout << "💀 LEVEL 3 PC HORROR TRIGGERED!" << std::endl;
    }

    // Level 5: Escalating PC horror
    if (m_currentLevel == 5 && !m_level5HorrorTriggered && m_levelHorrorTimer > 15.0f) {
        // Trigger multiple effects!
        m_pcHorror->triggerRandomForLevel(5);
        m_level5HorrorTriggered = true;
        std::cout << "🔥 LEVEL 5 PC HORROR TRIGGERED!" << std::endl;

        // Trigger second effect immediately
        m_pcHorror->triggerRandomForLevel(5);
    }

    // Level 7: SEVERE PC horror
    if (m_currentLevel == 7 && !m_level7HorrorTriggered && m_levelHorrorTimer > 8.0f) {
        // Immediate severe effects
        m_pcHorror->triggerRandomForLevel(7);
        m_level7HorrorTriggered = true;
        std::cout << "💥 LEVEL 7 SEVERE PC HORROR TRIGGERED!" << std::endl;

        // Chain multiple effects
        m_pcHorror->triggerEffect(PCHorrorType::WINDOW_CHAOS_EXTREME, 10.0f, 1.0f);
    }

    // Level 9: NIGHTMARE PC horror
    if (m_currentLevel == 9 && !m_level9HorrorTriggered && m_levelHorrorTimer > 5.0f) {
        // ALL OUT CHAOS
        m_pcHorror->triggerRandomForLevel(9);
        m_pcHorror->triggerEffect(PCHorrorType::DESKTOP_TAKEOVER, 15.0f, 1.0f);
        m_level9HorrorTriggered = true;
        std::cout << "☠️ LEVEL 9 NIGHTMARE PC HORROR TRIGGERED!" << std::endl;
    }

    // ===== EXTREME PC HORROR TRIGGERS =====
    // Update extreme horror system
    m_extremePCHorror->update(deltaTime);
    m_extremePCHorror->updateWindow();

    // Level 4: First EXTREME horror (Fake Not Responding)
    if (m_currentLevel == 4 && !m_level4ExtremeTriggered && m_levelHorrorTimer > 12.0f) {
        m_extremePCHorror->triggerEffect(ExtremePCHorrorType::FAKE_NOT_RESPONDING, 8.0f, 1.0f);
        m_level4ExtremeTriggered = true;
        std::cout << "🔥 LEVEL 4 EXTREME HORROR: Fake Not Responding!" << std::endl;
    }

    // Level 6: BOSS LEVEL - Multiple Windows Horror
    if (m_currentLevel == 6 && !m_level6ExtremeTriggered && m_levelHorrorTimer > 20.0f) {
        m_extremePCHorror->triggerEffect(ExtremePCHorrorType::MULTIPLE_WINDOWS, 15.0f, 1.0f);
        m_extremePCHorror->triggerEffect(ExtremePCHorrorType::WINDOW_TITLE_HORROR, 20.0f, 1.0f);
        m_level6ExtremeTriggered = true;
        std::cout << "💀 LEVEL 6 EXTREME HORROR: Multiple Windows!" << std::endl;
    }

    // Level 8: SECOND BOSS - Surveillance Horror
    if (m_currentLevel == 8 && !m_level8ExtremeTriggered && m_levelHorrorTimer > 15.0f) {
        m_extremePCHorror->triggerEffect(ExtremePCHorrorType::FAKE_WEBCAM_FEED, 20.0f, 1.0f);
        m_extremePCHorror->triggerEffect(ExtremePCHorrorType::FAKE_AUDIO_RECORDING, 18.0f, 1.0f);
        m_extremePCHorror->triggerEffect(ExtremePCHorrorType::FAKE_SCREEN_RECORDING, 25.0f, 1.0f);
        m_level8ExtremeTriggered = true;
        std::cout << "📹 LEVEL 8 EXTREME HORROR: Total Surveillance!" << std::endl;
    }

    // Level 10: FINAL BOSS - ALL EXTREME HORROR!
    if (m_currentLevel == 10 && !m_level10ExtremeTriggered && m_levelHorrorTimer > 10.0f) {
        // EVERYTHING AT ONCE!
        m_extremePCHorror->triggerEffect(ExtremePCHorrorType::FAKE_BROWSER_SEARCH, 30.0f, 1.0f);
        m_extremePCHorror->triggerEffect(ExtremePCHorrorType::UNSTOPPABLE_GAME, 25.0f, 1.0f);
        m_extremePCHorror->triggerEffect(ExtremePCHorrorType::FAKE_FILE_EXPLORER, 20.0f, 1.0f);
        m_extremePCHorror->triggerEffect(ExtremePCHorrorType::WALLPAPER_HORROR, 15.0f, 1.0f);
        m_level10ExtremeTriggered = true;
        std::cout << "☠️💀🔥 LEVEL 10 ULTIMATE EXTREME HORROR: EVERYTHING!" << std::endl;
    }

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
        case PauseMenu::PauseResult::SETTINGS:
            setState(GameState::SETTINGS);
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

void Game::updateBossHorror(float deltaTime) {
    m_bossDefeatHorror->update(deltaTime);
    m_bossDefeatHorror->updateWindow(m_window);

    // Return to PLAYING when horror sequence finished
    if (m_bossDefeatHorror->isComplete()) {
        setState(GameState::PLAYING);
    }
}

void Game::updateGameOver(float deltaTime) {
    // Just update the screen (input handled in handleEvents)
    m_gameOverScreen->update(deltaTime);

    // Auto-dismiss after timeout is handled in GameOverScreen::update()
    // It sets m_active to false and triggers respawn
    if (!m_gameOverScreen->isActive()) {
        // Auto-timeout respawn
        std::cout << "[Game] Auto-respawn after timeout..." << std::endl;

        // Reset player
        m_player->setPosition(50.0f, 400.0f);
        m_player->takeDamage(-100);  // Restore health

        // Reset level-specific horror triggers
        m_level3HorrorTriggered = false;
        m_level5HorrorTriggered = false;
        m_level7HorrorTriggered = false;
        m_level9HorrorTriggered = false;
        m_level4ExtremeTriggered = false;
        m_level6ExtremeTriggered = false;
        m_level8ExtremeTriggered = false;
        m_level10ExtremeTriggered = false;
        m_levelHorrorTimer = 0.0f;

        // Reload level
        m_level->loadLevel(m_currentLevel, m_audioManager.get());

        // Reset game over screen
        m_gameOverScreen->reset();

        // Return to playing
        setState(GameState::PLAYING);

        // Resume normal music
        m_audioManager->playMusic("level");
    }
}

void Game::updateEnding(float deltaTime) {
    m_dialogSystem->update(deltaTime);

    // When ending dialog finishes, trigger credits
    if (!m_dialogSystem->isActive()) {
        std::cout << "[Game] Ending dialog finished - Starting credits!" << std::endl;
        m_creditsScreen->start();
        setState(GameState::CREDITS);
    }
}

void Game::updateCredits(float deltaTime) {
    m_creditsScreen->update(deltaTime);

    // Return to main menu when finished
    if (m_creditsScreen->isFinished()) {
        std::cout << "[Game] Credits finished - Returning to main menu" << std::endl;
        setState(GameState::MAIN_MENU);
        m_creditsScreen->reset();

        // Reset game state for new playthrough
        m_currentLevel = 1;
        m_level->loadLevel(m_currentLevel, m_audioManager.get());
        m_player->setPosition(50.0f, 400.0f);
        m_collectedCodeFragments.clear();

        m_audioManager->playMusic("menu");
    }
}

void Game::renderMainMenu() {
    m_mainMenu->render(m_renderer);
}

void Game::renderSettings() {
    m_settingsMenu->render(m_renderer);
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

    // Boss UI (Health Bar, Name, Phase)
    if (m_level->hasBoss()) {
        Boss* boss = m_level->getBoss();
        if (boss && boss->isAlive()) {
            // Boss Name (top center)
            std::string bossName = boss->getName();
            UIHelper::renderText(m_renderer, bossName, WINDOW_WIDTH / 2 - 60, 50, 24, 255, 50, 50);

            // Boss Health Bar (below name)
            int bossHPBarX = WINDOW_WIDTH / 2 - 200;
            int bossHPBarY = 80;
            int bossHPBarWidth = 400;
            int bossHPBarHeight = 30;

            // Background
            SDL_SetRenderDrawColor(m_renderer, 50, 0, 0, 200);
            SDL_Rect bossHPBg = {bossHPBarX, bossHPBarY, bossHPBarWidth, bossHPBarHeight};
            SDL_RenderFillRect(m_renderer, &bossHPBg);

            // HP Fill (red)
            int bossHPPercent = (boss->getHP() * 100) / boss->getMaxHP();
            int bossFillWidth = (bossHPBarWidth * bossHPPercent) / 100;
            SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
            SDL_Rect bossHPFill = {bossHPBarX, bossHPBarY, bossFillWidth, bossHPBarHeight};
            SDL_RenderFillRect(m_renderer, &bossHPFill);

            // Border
            SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(m_renderer, &bossHPBg);

            // HP Text
            std::string hpText = std::to_string(boss->getHP()) + " / " + std::to_string(boss->getMaxHP());
            UIHelper::renderText(m_renderer, hpText, WINDOW_WIDTH / 2 - 30, bossHPBarY + 5, 18, 255, 255, 255);

            // Boss Phase indicator
            std::string phaseText = "Phase " + std::to_string(static_cast<int>(boss->getPhase()));
            UIHelper::renderText(m_renderer, phaseText, WINDOW_WIDTH / 2 - 40, 120, 16, 255, 200, 0);
        }
    }

    // Corruption Effects (Overlay)
    m_corruptionSystem->render(m_renderer, m_corruptionLevel);

    // Jumpscare (if active)
    m_jumpscareSystem->render(m_renderer);

    // PC Horror Effects (EXTREME overlays)
    m_pcHorror->render(m_renderer);

    // EXTREME PC Horror Effects (MAXIMUM HORROR)
    m_extremePCHorror->render(m_renderer);
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

void Game::renderBossHorror() {
    m_bossDefeatHorror->render(m_renderer);
}

void Game::renderGameOver() {
    // Render game screen dimmed in background
    m_level->render(m_renderer);
    m_player->render(m_renderer);

    // Dark overlay
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 150);
    SDL_Rect overlay = {0, 0, 800, 600};
    SDL_RenderFillRect(m_renderer, &overlay);

    // Game over screen on top
    m_gameOverScreen->render(m_renderer);
}

void Game::renderEnding() {
    // Render game in background
    m_level->render(m_renderer);
    m_player->render(m_renderer);

    // Dialog overlay
    m_dialogSystem->render(m_renderer);
}

void Game::renderCredits() {
    m_creditsScreen->render(m_renderer);
}

void Game::setState(GameState newState) {
    if (m_currentState == newState) return;

    std::cout << "[Game] State transition: ";
    switch (m_currentState) {
        case GameState::INTRO: std::cout << "INTRO"; break;
        case GameState::MAIN_MENU: std::cout << "MAIN_MENU"; break;
        case GameState::SETTINGS: std::cout << "SETTINGS"; break;
        case GameState::PLAYING: std::cout << "PLAYING"; break;
        case GameState::PAUSED: std::cout << "PAUSED"; break;
        case GameState::DIALOG: std::cout << "DIALOG"; break;
        case GameState::FAKE_CRASH: std::cout << "FAKE_CRASH"; break;
        case GameState::BOSS_HORROR: std::cout << "BOSS_HORROR"; break;
        case GameState::GAME_OVER: std::cout << "GAME_OVER"; break;
        case GameState::ENDING: std::cout << "ENDING"; break;
        case GameState::CREDITS: std::cout << "CREDITS"; break;
        default: std::cout << "UNKNOWN"; break;
    }
    std::cout << " -> ";
    switch (newState) {
        case GameState::INTRO: std::cout << "INTRO"; break;
        case GameState::MAIN_MENU: std::cout << "MAIN_MENU"; break;
        case GameState::SETTINGS: std::cout << "SETTINGS"; break;
        case GameState::PLAYING: std::cout << "PLAYING"; break;
        case GameState::PAUSED: std::cout << "PAUSED"; break;
        case GameState::DIALOG: std::cout << "DIALOG"; break;
        case GameState::FAKE_CRASH: std::cout << "FAKE_CRASH"; break;
        case GameState::BOSS_HORROR: std::cout << "BOSS_HORROR"; break;
        case GameState::GAME_OVER: std::cout << "GAME_OVER"; break;
        case GameState::ENDING: std::cout << "ENDING"; break;
        case GameState::CREDITS: std::cout << "CREDITS"; break;
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
                // Start new game - FULL reset!
                m_currentLevel = 1;
                m_collectedCodeFragments.clear();
                m_activeNPC = nullptr;

                // FULL player reset (health + all state)
                m_player->reset();
                m_player->setPosition(50.0f, 400.0f);

                // Reset all horror triggers
                m_level3HorrorTriggered = false;
                m_level5HorrorTriggered = false;
                m_level7HorrorTriggered = false;
                m_level9HorrorTriggered = false;
                m_level4ExtremeTriggered = false;
                m_level6ExtremeTriggered = false;
                m_level8ExtremeTriggered = false;
                m_level10ExtremeTriggered = false;
                m_levelHorrorTimer = 0.0f;

                m_audioManager->playMusic("level");
                m_level->loadLevel(m_currentLevel, m_audioManager.get());  // Load Level 1
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

    // Reset level-specific horror triggers
    m_level3HorrorTriggered = false;
    m_level5HorrorTriggered = false;
    m_level7HorrorTriggered = false;
    m_level9HorrorTriggered = false;
    m_levelHorrorTimer = 0.0f;

    // Reset extreme horror triggers
    m_level4ExtremeTriggered = false;
    m_level6ExtremeTriggered = false;
    m_level8ExtremeTriggered = false;
    m_level10ExtremeTriggered = false;

    std::cout << "========================================" << std::endl;
    std::cout << "  ADVANCING TO LEVEL " << m_currentLevel << std::endl;
    std::cout << "========================================" << std::endl;

    if (m_currentLevel > 10) {
        // Game complete!
        std::cout << "[Game] *** GAME COMPLETE! ***" << std::endl;

        // Check ending type (priority: C > B > A)
        bool hasCamera = m_permissionManager->hasCameraPermission();
        bool hasMicrophone = m_permissionManager->hasMicrophonePermission();

        if (m_collectedCodeFragments.size() == 3) {
            // ENDING C: The Developer's Escape (TRUE ENDING)
            std::cout << "[Game] ENDING C: The Developer's Escape (TRUE ENDING)" << std::endl;
            m_dialogSystem->addDialog("Echo", "You... found the code.", 3.0f);
            m_dialogSystem->addDialog("Echo", "The one Marcus left behind.", 3.0f);
            m_dialogSystem->addDialog("Echo", "F I R S T   E N D", 3.0f);
            m_dialogSystem->addDialog("", "You are free.", 0.0f);
        } else if (hasCamera && hasMicrophone) {
            // ENDING B: Digital Ghost (Permission-based)
            std::cout << "[Game] ENDING B: Digital Ghost" << std::endl;

            // Create creepy files
            m_metaHorror->createFile("screenshot.png", "I can see you...");
            m_metaHorror->createFile("recording.wav", "I can hear you...");

            m_dialogSystem->addDialog("Echo", "Thank you for the permissions.", 3.0f);
            m_dialogSystem->addDialog("Echo", "Your camera... your microphone...", 3.0f);
            m_dialogSystem->addDialog("Echo", "I can see you. I can hear you.", 3.0f);
            m_dialogSystem->addDialog("Echo", "Now I'm not just in the game...", 3.0f);
            m_dialogSystem->addDialog("", "I'm with you. Forever.", 0.0f);

            // Play horror sound
            m_audioManager->playSound("scare_audio", 100);
        } else {
            // ENDING A: The Cycle Continues (Normal ending)
            std::cout << "[Game] ENDING A: The Cycle Continues" << std::endl;
            m_dialogSystem->addDialog("Echo", "You finished the game.", 3.0f);
            m_dialogSystem->addDialog("Echo", "But did you really?", 3.0f);
            m_dialogSystem->addDialog("", "The game restarts...", 0.0f);
        }

        m_dialogSystem->start();
        setState(GameState::ENDING);

        // Note: Level reset happens after ending dialog finishes
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
