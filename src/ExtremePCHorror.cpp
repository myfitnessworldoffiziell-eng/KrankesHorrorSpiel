#include "ExtremePCHorror.h"
#include "UIHelper.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

ExtremePCHorror::ExtremePCHorror(SDL_Window* window, AudioManager* audioManager, MetaHorror* metaHorror)
    : m_window(window)
    , m_audioManager(audioManager)
    , m_metaHorror(metaHorror)
    , m_isCrashed(false)
    , m_crashTimer(0.0f)
    , m_jumpscareReady(false)
    , m_windowManipulated(false)
{
    SDL_GetWindowPosition(m_window, &m_originalWindowX, &m_originalWindowY);
    SDL_GetWindowSize(m_window, &m_originalWindowW, &m_originalWindowH);
    m_originalWindowTitle = SDL_GetWindowTitle(m_window);
}

ExtremePCHorror::~ExtremePCHorror() {
    clearAll();
    restoreWindow();
}

void ExtremePCHorror::triggerEffect(ExtremePCHorrorType type, float duration, float intensity) {
    auto effect = std::make_unique<ExtremePCHorrorEffect>(type, duration, intensity);
    effect->isActive = true;

    // Setup effect-specific data
    switch (type) {
        case ExtremePCHorrorType::FAKE_BROWSER_SEARCH:
            effect->data = generateFakeBrowserSearches();
            break;

        case ExtremePCHorrorType::FAKE_FILE_EXPLORER:
            effect->data = generateFakeFiles();
            break;

        case ExtremePCHorrorType::WINDOW_TITLE_HORROR:
            effect->data = {
                "I CAN SEE YOU",
                "DON'T CLOSE ME",
                "I KNOW YOUR NAME",
                "YOU CAN'T ESCAPE",
                "I'M IN YOUR COMPUTER NOW"
            };
            break;

        case ExtremePCHorrorType::FAKE_SOCIAL_MEDIA:
            effect->data = {
                "Echo tagged you in a post",
                "New message from Echo: I found you",
                "Echo shared your location",
                "Echo is watching your profile"
            };
            break;

        default:
            break;
    }

    m_activeEffects.push_back(std::move(effect));

    // Play appropriate horror sound
    if (m_audioManager) {
        switch (type) {
            case ExtremePCHorrorType::FAKE_CRASH_WITH_JUMPSCARE:
            case ExtremePCHorrorType::FULLSCREEN_JUMPSCARE:
                m_audioManager->playSound("jumpscare", 100);
                break;

            case ExtremePCHorrorType::FAKE_WEBCAM_FEED:
            case ExtremePCHorrorType::FAKE_AUDIO_RECORDING:
                m_audioManager->playSound("scare_appear", 80);
                break;

            default:
                m_audioManager->playSound("zap", 60);
                break;
        }
    }

    std::cout << "🔥💀 EXTREME PC HORROR TRIGGERED: Type " << static_cast<int>(type) << std::endl;
}

void ExtremePCHorror::triggerRandomExtreme(int level) {
    std::vector<ExtremePCHorrorType> availableEffects;

    if (level >= 5) {
        availableEffects = {
            ExtremePCHorrorType::FAKE_NOT_RESPONDING,
            ExtremePCHorrorType::WINDOW_TITLE_HORROR,
            ExtremePCHorrorType::FAKE_WEBCAM_FEED,
            ExtremePCHorrorType::FAKE_AUDIO_RECORDING
        };
    }

    if (level >= 7) {
        availableEffects = {
            ExtremePCHorrorType::MULTIPLE_WINDOWS,
            ExtremePCHorrorType::UNSTOPPABLE_GAME,
            ExtremePCHorrorType::FAKE_FILE_EXPLORER,
            ExtremePCHorrorType::FAKE_SCREEN_RECORDING,
            ExtremePCHorrorType::FAKE_BROWSER_SEARCH
        };
    }

    if (level >= 9) {
        availableEffects = {
            ExtremePCHorrorType::FAKE_CRASH_WITH_JUMPSCARE,
            ExtremePCHorrorType::FULLSCREEN_JUMPSCARE,
            ExtremePCHorrorType::DESKTOP_ICON_CHAOS,
            ExtremePCHorrorType::WALLPAPER_HORROR,
            ExtremePCHorrorType::FAKE_SOCIAL_MEDIA
        };
    }

    if (!availableEffects.empty()) {
        int index = rand() % availableEffects.size();
        float duration = 8.0f + (rand() % 12);  // 8-20 seconds
        triggerEffect(availableEffects[index], duration, 1.0f);
    }
}

bool ExtremePCHorror::triggerFakeCrash() {
    m_isCrashed = true;
    m_crashTimer = 0.0f;
    m_jumpscareReady = true;

    std::cout << "💀 FAKE GAME CRASH TRIGGERED!" << std::endl;

    if (m_audioManager) {
        m_audioManager->playSound("whitenoise", 100);
    }

    return true;
}

void ExtremePCHorror::restartFromCrash() {
    m_isCrashed = false;
    m_crashTimer = 0.0f;

    // Trigger jumpscare immediately
    if (m_jumpscareReady) {
        triggerEffect(ExtremePCHorrorType::FULLSCREEN_JUMPSCARE, 2.0f, 1.0f);
        m_jumpscareReady = false;
    }
}

void ExtremePCHorror::update(float deltaTime) {
    // Update crash timer
    if (m_isCrashed) {
        m_crashTimer += deltaTime;
        return;  // Don't update other effects while crashed
    }

    // Update all active effects
    for (auto it = m_activeEffects.begin(); it != m_activeEffects.end();) {
        auto& effect = *it;

        if (!effect->isActive) {
            ++it;
            continue;
        }

        effect->timer += deltaTime;

        // Update effect-specific logic
        switch (effect->type) {
            case ExtremePCHorrorType::MULTIPLE_WINDOWS:
                updateMultipleWindows(effect.get(), deltaTime);
                break;

            case ExtremePCHorrorType::WINDOW_TITLE_HORROR:
                updateWindowTitleHorror(effect.get(), deltaTime);
                break;

            default:
                break;
        }

        // Check if expired
        if (effect->timer >= effect->duration) {
            effect->isActive = false;

            // Cleanup effect-specific state
            if (effect->type == ExtremePCHorrorType::WINDOW_TITLE_HORROR) {
                SDL_SetWindowTitle(m_window, m_originalWindowTitle.c_str());
            }

            it = m_activeEffects.erase(it);
        } else {
            ++it;
        }
    }
}

void ExtremePCHorror::render(SDL_Renderer* renderer) {
    // Render crash screen if crashed
    if (m_isCrashed) {
        // Black screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect fullScreen = {0, 0, 800, 600};
        SDL_RenderFillRect(renderer, &fullScreen);

        // Error message
        UIHelper::renderText(renderer, "ECHOES.exe has stopped working", 200, 200, 20, 255, 255, 255);
        UIHelper::renderText(renderer, "The application crashed unexpectedly", 180, 250, 14, 200, 200, 200);
        UIHelper::renderText(renderer, "[Press SPACE to restart]", 260, 350, 16, 255, 165, 0);

        // Fake error details
        UIHelper::renderText(renderer, "Error Code: 0xC0000005", 250, 420, 12, 150, 150, 150);
        UIHelper::renderText(renderer, "Exception: ACCESS_VIOLATION", 230, 445, 12, 150, 150, 150);

        return;
    }

    // Render all active effects
    for (auto& effect : m_activeEffects) {
        if (!effect->isActive) continue;

        switch (effect->type) {
            case ExtremePCHorrorType::FAKE_NOT_RESPONDING:
                renderFakeNotResponding(renderer, effect.get());
                break;

            case ExtremePCHorrorType::MULTIPLE_WINDOWS:
                renderMultipleWindows(renderer, effect.get());
                break;

            case ExtremePCHorrorType::UNSTOPPABLE_GAME:
                renderUnstoppableGame(renderer, effect.get());
                break;

            case ExtremePCHorrorType::FAKE_FILE_EXPLORER:
                renderFakeFileExplorer(renderer, effect.get());
                break;

            case ExtremePCHorrorType::WALLPAPER_HORROR:
                renderWallpaperHorror(renderer, effect.get());
                break;

            case ExtremePCHorrorType::FAKE_WEBCAM_FEED:
                renderFakeWebcamFeed(renderer, effect.get());
                break;

            case ExtremePCHorrorType::FAKE_AUDIO_RECORDING:
                renderFakeAudioRecording(renderer, effect.get());
                break;

            case ExtremePCHorrorType::FAKE_SCREEN_RECORDING:
                renderFakeScreenRecording(renderer, effect.get());
                break;

            case ExtremePCHorrorType::FAKE_BROWSER_SEARCH:
                renderFakeBrowserSearch(renderer, effect.get());
                break;

            case ExtremePCHorrorType::FAKE_SOCIAL_MEDIA:
                renderFakeSocialMedia(renderer, effect.get());
                break;

            case ExtremePCHorrorType::FULLSCREEN_JUMPSCARE:
                renderFullscreenJumpscare(renderer, effect.get());
                break;

            default:
                break;
        }
    }
}

void ExtremePCHorror::updateWindow() {
    // Window manipulation based on active effects
    for (auto& effect : m_activeEffects) {
        if (!effect->isActive) continue;

        if (effect->type == ExtremePCHorrorType::MULTIPLE_WINDOWS) {
            // Chaotic movement
            float time = effect->timer;
            int newX = m_originalWindowX + static_cast<int>(std::sin(time * 8.0f) * 200.0f);
            int newY = m_originalWindowY + static_cast<int>(std::cos(time * 6.0f) * 150.0f);
            SDL_SetWindowPosition(m_window, newX, newY);
            m_windowManipulated = true;
        }
    }
}

// ============================================================================
// EFFECT RENDERERS
// ============================================================================

void ExtremePCHorror::renderFakeNotResponding(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect) {
    // Darken screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect overlay = {0, 0, 800, 600};
    SDL_RenderFillRect(renderer, &overlay);

    // "Not Responding" message
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_Rect dialog = {200, 200, 400, 200};
    SDL_RenderFillRect(renderer, &dialog);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &dialog);

    UIHelper::renderText(renderer, "ECHOES.exe", 220, 220, 18, 0, 0, 0);
    UIHelper::renderText(renderer, "Not Responding", 220, 250, 14, 80, 80, 80);

    // Fake progress bar
    int progress = static_cast<int>((effect->timer / effect->duration) * 100);
    SDL_Rect progressBg = {220, 300, 360, 20};
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &progressBg);

    SDL_Rect progressBar = {220, 300, (progress * 360) / 100, 20};
    SDL_SetRenderDrawColor(renderer, 0, 120, 215, 255);
    SDL_RenderFillRect(renderer, &progressBar);

    UIHelper::renderText(renderer, "Waiting for program to respond...", 220, 340, 12, 80, 80, 80);

    // Jumpscare at end
    if (effect->timer > effect->duration - 0.5f) {
        UIHelper::renderText(renderer, "I'M NOT FROZEN", 280, 500, 24, 255, 0, 0);
    }
}

void ExtremePCHorror::renderMultipleWindows(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect) {
    // Create fake windows first time
    if (effect->phase == 0 && m_fakeWindows.empty()) {
        createFakeWindows(8);
        effect->phase = 1;
    }

    // Render fake windows
    for (auto& fakeWin : m_fakeWindows) {
        if (!fakeWin.active) continue;

        // Window background
        SDL_SetRenderDrawColor(renderer, 240, 240, 240, 200);
        SDL_RenderFillRect(renderer, &fakeWin.bounds);

        // Window border
        SDL_SetRenderDrawColor(renderer, 0, 120, 215, 255);
        SDL_Rect titleBar = {fakeWin.bounds.x, fakeWin.bounds.y, fakeWin.bounds.w, 30};
        SDL_RenderFillRect(renderer, &titleBar);

        // Title text
        UIHelper::renderText(renderer, fakeWin.title,
                           fakeWin.bounds.x + 10, fakeWin.bounds.y + 8, 12, 255, 255, 255);

        // Content
        UIHelper::renderText(renderer, "I'M EVERYWHERE",
                           fakeWin.bounds.x + 20, fakeWin.bounds.y + 60, 16, 255, 0, 0);
    }
}

void ExtremePCHorror::renderUnstoppableGame(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect) {
    // Message at top
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 230);
    SDL_Rect banner = {0, 0, 800, 60};
    SDL_RenderFillRect(renderer, &banner);

    UIHelper::renderText(renderer, "YOU CAN'T CLOSE ME", 280, 20, 20, 255, 255, 255);

    // Flashing message
    if (static_cast<int>(effect->timer * 2) % 2 == 0) {
        UIHelper::renderText(renderer, "ALT+F4 won't work", 310, 520, 14, 255, 0, 0);
        UIHelper::renderText(renderer, "Task Manager won't work", 290, 545, 14, 255, 0, 0);
        UIHelper::renderText(renderer, "I'm part of your system now", 270, 570, 14, 255, 0, 0);
    }
}

void ExtremePCHorror::renderFakeFileExplorer(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect) {
    // Explorer window
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 240);
    SDL_Rect explorer = {100, 100, 600, 400};
    SDL_RenderFillRect(renderer, &explorer);

    // Title bar
    SDL_SetRenderDrawColor(renderer, 0, 120, 215, 255);
    SDL_Rect titleBar = {100, 100, 600, 30};
    SDL_RenderFillRect(renderer, &titleBar);

    UIHelper::renderText(renderer, "File Explorer - Documents", 115, 108, 12, 255, 255, 255);

    // File list
    int y = 150;
    for (size_t i = 0; i < effect->data.size() && i < 10; i++) {
        // Alternate background
        if (i % 2 == 0) {
            SDL_SetRenderDrawColor(renderer, 245, 245, 245, 255);
            SDL_Rect row = {110, y - 5, 580, 25};
            SDL_RenderFillRect(renderer, &row);
        }

        std::string file = effect->data[i];
        UIHelper::renderText(renderer, file, 120, y, 14, 0, 0, 0);

        // "Deleting..." animation
        if (effect->timer > 3.0f + i * 0.5f) {
            UIHelper::renderText(renderer, "[DELETED]", 500, y, 12, 255, 0, 0);
        }

        y += 30;
    }

    // Warning at bottom
    if (effect->timer > 5.0f) {
        UIHelper::renderText(renderer, "All your files are being scanned...", 150, 470, 14, 255, 0, 0);
    }
}

void ExtremePCHorror::renderWallpaperHorror(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect) {
    // Full screen "desktop"
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 250);
    SDL_Rect fullScreen = {0, 0, 800, 600};
    SDL_RenderFillRect(renderer, &fullScreen);

    // "Wallpaper" text
    UIHelper::renderText(renderer, "I'M YOUR WALLPAPER NOW", 230, 250, 24, 255, 0, 0);

    // Face made of text characters
    int faceY = 150;
    UIHelper::renderText(renderer, "████████████████", 280, faceY, 20, 255, 255, 255);
    UIHelper::renderText(renderer, "██  O     O  ██", 280, faceY + 40, 20, 255, 255, 255);
    UIHelper::renderText(renderer, "██          ██", 280, faceY + 80, 20, 255, 255, 255);
    UIHelper::renderText(renderer, "██ ======== ██", 280, faceY + 120, 20, 255, 0, 0);
    UIHelper::renderText(renderer, "████████████████", 280, faceY + 160, 20, 255, 255, 255);

    UIHelper::renderText(renderer, "I SEE YOU", 330, faceY + 220, 18, 255, 0, 0);
}

void ExtremePCHorror::renderFakeWebcamFeed(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect) {
    // Small window (top-right corner)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 230);
    SDL_Rect webcamWindow = {550, 20, 240, 180};
    SDL_RenderFillRect(renderer, &webcamWindow);

    // Red border (recording)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int i = 0; i < 3; i++) {
        SDL_Rect border = {550 - i, 20 - i, 240 + i * 2, 180 + i * 2};
        SDL_RenderDrawRect(renderer, &border);
    }

    // Recording indicator
    if (static_cast<int>(effect->timer * 3) % 2 == 0) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect recDot = {560, 30, 15, 15};
        SDL_RenderFillRect(renderer, &recDot);
    }

    UIHelper::renderText(renderer, "REC", 580, 28, 14, 255, 255, 255);
    UIHelper::renderText(renderer, "WEBCAM ACTIVE", 620, 28, 12, 255, 255, 255);

    // Fake video feed (static noise)
    for (int i = 0; i < 100; i++) {
        int x = 560 + rand() % 220;
        int y = 50 + rand() % 140;
        int brightness = rand() % 100;
        SDL_SetRenderDrawColor(renderer, brightness, brightness, brightness, 255);
        SDL_Rect pixel = {x, y, 2, 2};
        SDL_RenderFillRect(renderer, &pixel);
    }

    // Message
    UIHelper::renderText(renderer, "I can see you", 590, 120, 16, 255, 0, 0);

    // Timer
    int seconds = static_cast<int>(effect->timer);
    std::string timeText = std::string("00:") + (seconds < 10 ? "0" : "") + std::to_string(seconds);
    UIHelper::renderText(renderer, timeText, 670, 175, 12, 255, 255, 255);
}

void ExtremePCHorror::renderFakeAudioRecording(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect) {
    // Audio recording indicator (top-left)
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 220);
    SDL_Rect audioBox = {10, 60, 280, 100};
    SDL_RenderFillRect(renderer, &audioBox);

    // Microphone icon (simple)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect mic = {30, 80, 20, 40};
    SDL_RenderFillRect(renderer, &mic);

    UIHelper::renderText(renderer, "MICROPHONE ACTIVE", 60, 75, 14, 255, 255, 255);
    UIHelper::renderText(renderer, "Recording...", 60, 95, 12, 255, 255, 255);

    // Fake waveform
    int waveY = 125;
    for (int i = 0; i < 20; i++) {
        int height = (rand() % 30) + 5;
        SDL_Rect bar = {20 + i * 12, waveY - height / 2, 8, height};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &bar);
    }

    UIHelper::renderText(renderer, "I heard what you said...", 20, 145, 10, 255, 255, 255);
}

void ExtremePCHorror::renderFakeScreenRecording(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect) {
    // Screen recording notification
    SDL_SetRenderDrawColor(renderer, 255, 140, 0, 230);
    SDL_Rect notif = {250, 10, 300, 80};
    SDL_RenderFillRect(renderer, &notif);

    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &notif);

    UIHelper::renderText(renderer, "SCREEN RECORDING ACTIVE", 265, 25, 14, 255, 255, 255);
    UIHelper::renderText(renderer, "Your screen is being recorded", 265, 50, 12, 255, 255, 255);

    int seconds = static_cast<int>(effect->timer);
    std::string timeText = "Duration: " + std::to_string(seconds) + "s";
    UIHelper::renderText(renderer, timeText, 265, 70, 10, 255, 255, 255);
}

void ExtremePCHorror::renderFakeBrowserSearch(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect) {
    // Browser window
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 245);
    SDL_Rect browser = {50, 50, 700, 500};
    SDL_RenderFillRect(renderer, &browser);

    // Address bar
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_Rect addressBar = {60, 60, 680, 35};
    SDL_RenderFillRect(renderer, &addressBar);

    UIHelper::renderText(renderer, "Google Search", 75, 70, 14, 80, 80, 80);

    // Search results
    int y = 120;
    for (size_t i = 0; i < effect->data.size() && i < 6; i++) {
        // Link color
        UIHelper::renderText(renderer, effect->data[i], 80, y, 16, 0, 0, 200);

        // Underline (fake link)
        SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255);
        SDL_Rect underline = {80, y + 20, static_cast<int>(effect->data[i].length() * 9), 1};
        SDL_RenderFillRect(renderer, &underline);

        y += 60;
    }

    // Warning at bottom
    if (effect->timer > 3.0f) {
        UIHelper::renderText(renderer, "Someone is searching for information about you...", 100, 520, 12, 255, 0, 0);
    }
}

void ExtremePCHorror::renderFakeSocialMedia(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect) {
    // Notification popup
    SDL_SetRenderDrawColor(renderer, 59, 89, 152, 240);  // Facebook blue
    SDL_Rect notif = {500, 80, 290, 120};
    SDL_RenderFillRect(renderer, &notif);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &notif);

    int messageIndex = static_cast<int>(effect->timer / 2.0f) % effect->data.size();
    std::string message = effect->data[messageIndex];

    UIHelper::renderText(renderer, "Notification", 515, 95, 14, 255, 255, 255);
    UIHelper::renderText(renderer, message.substr(0, 35), 515, 120, 12, 255, 255, 255);

    if (message.length() > 35) {
        UIHelper::renderText(renderer, message.substr(35), 515, 140, 12, 255, 255, 255);
    }

    UIHelper::renderText(renderer, "Just now", 515, 170, 10, 200, 200, 255);
}

void ExtremePCHorror::renderFullscreenJumpscare(SDL_Renderer* renderer, ExtremePCHorrorEffect* effect) {
    // Full screen flash
    Uint8 intensity = 255;
    if (effect->timer < 0.2f) {
        intensity = static_cast<Uint8>((effect->timer / 0.2f) * 255);
    }

    SDL_SetRenderDrawColor(renderer, intensity, 0, 0, 255);
    SDL_Rect fullScreen = {0, 0, 800, 600};
    SDL_RenderFillRect(renderer, &fullScreen);

    // Jumpscare text
    if (effect->timer > 0.2f) {
        UIHelper::renderText(renderer, "I'M HERE", 320, 250, 48, 255, 255, 255);
        UIHelper::renderText(renderer, "YOU THOUGHT YOU COULD RESTART?", 150, 350, 20, 255, 255, 255);
    }

    // Random glitch rectangles
    for (int i = 0; i < 30; i++) {
        SDL_SetRenderDrawColor(renderer, 255, rand() % 100, rand() % 100, 200);
        SDL_Rect glitch = {
            rand() % 800,
            rand() % 600,
            rand() % 150 + 50,
            rand() % 100 + 20
        };
        SDL_RenderFillRect(renderer, &glitch);
    }
}

// ============================================================================
// EFFECT UPDATERS
// ============================================================================

void ExtremePCHorror::updateMultipleWindows(ExtremePCHorrorEffect* effect, float deltaTime) {
    // Animate fake windows
    for (auto& fakeWin : m_fakeWindows) {
        fakeWin.bounds.x += (rand() % 10) - 5;
        fakeWin.bounds.y += (rand() % 10) - 5;

        // Keep on screen
        if (fakeWin.bounds.x < 0) fakeWin.bounds.x = 0;
        if (fakeWin.bounds.y < 0) fakeWin.bounds.y = 0;
        if (fakeWin.bounds.x + fakeWin.bounds.w > 800) fakeWin.bounds.x = 800 - fakeWin.bounds.w;
        if (fakeWin.bounds.y + fakeWin.bounds.h > 600) fakeWin.bounds.y = 600 - fakeWin.bounds.h;
    }
}

void ExtremePCHorror::updateWindowTitleHorror(ExtremePCHorrorEffect* effect, float deltaTime) {
    // Change window title every 2 seconds
    if (static_cast<int>(effect->timer / 2.0f) != effect->phase) {
        effect->phase = static_cast<int>(effect->timer / 2.0f);

        if (effect->phase < static_cast<int>(effect->data.size())) {
            SDL_SetWindowTitle(m_window, effect->data[effect->phase].c_str());
        }
    }
}

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

void ExtremePCHorror::createFakeWindows(int count) {
    clearFakeWindows();

    for (int i = 0; i < count; i++) {
        FakeWindow fakeWin;
        fakeWin.bounds = {
            rand() % 600 + 50,
            rand() % 400 + 50,
            200 + rand() % 100,
            150 + rand() % 100
        };
        fakeWin.title = generateHorrorWindowTitle();
        fakeWin.active = true;
        m_fakeWindows.push_back(fakeWin);
    }
}

void ExtremePCHorror::clearFakeWindows() {
    m_fakeWindows.clear();
}

void ExtremePCHorror::restoreWindow() {
    if (m_windowManipulated && m_window) {
        SDL_SetWindowPosition(m_window, m_originalWindowX, m_originalWindowY);
        SDL_SetWindowSize(m_window, m_originalWindowW, m_originalWindowH);
        SDL_SetWindowTitle(m_window, m_originalWindowTitle.c_str());
        m_windowManipulated = false;
    }
}

std::string ExtremePCHorror::generateHorrorWindowTitle() {
    std::vector<std::string> titles = {
        "I SEE YOU",
        "HELP ME",
        "ERROR",
        "YOU CAN'T CLOSE ME",
        "ECHOES.exe",
        "I'M WATCHING",
        "DON'T LOOK AWAY",
        "RUN"
    };

    return titles[rand() % titles.size()];
}

std::vector<std::string> ExtremePCHorror::generateFakeBrowserSearches() {
    return {
        "How to remove ECHOES.exe from computer",
        "Is ECHOES.exe a virus?",
        "ECHOES.exe won't close help",
        "Can games access my webcam without permission",
        "How to delete game that watches you",
        "Horror game that knows my name"
    };
}

std::vector<std::string> ExtremePCHorror::generateFakeFiles() {
    return {
        "your_secrets.txt",
        "watching_you.mp4",
        "i_know_everything.docx",
        "your_location.dat",
        "dont_delete_me.exe",
        "echo_was_here.txt",
        "all_your_files.zip",
        "personal_data.db",
        "surveillance_log.txt",
        "you_cant_hide.pdf"
    };
}

bool ExtremePCHorror::hasActiveEffects() const {
    return !m_activeEffects.empty() || m_isCrashed;
}

void ExtremePCHorror::clearAll() {
    m_activeEffects.clear();
    clearFakeWindows();
    restoreWindow();
    m_isCrashed = false;
}
