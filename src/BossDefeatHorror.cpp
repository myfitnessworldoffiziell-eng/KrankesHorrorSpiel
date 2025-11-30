#include "BossDefeatHorror.h"
#include "AudioManager.h"
#include "PermissionManager.h"
#include "UIHelper.h"
#include <iostream>
#include <cmath>

BossDefeatHorror::BossDefeatHorror(AudioManager* audioManager, PermissionManager* permManager)
    : m_audioManager(audioManager)
    , m_permissionManager(permManager)
    , m_active(false)
    , m_sequenceComplete(false)
    , m_timer(0.0f)
    , m_currentPhase(0)
    , m_bossName("")
    , m_filesDeleted(0)
    , m_glitchIntensity(0.0f)
    , m_glitchOffsetX(0)
    , m_glitchOffsetY(0)
    , m_screenShake(false)
    , m_shakeTimer(0.0f)
    , m_windowManipulation(false)
    , m_windowMoveTimer(0.0f)
    , m_originalWindowX(0)
    , m_originalWindowY(0)
{
}

BossDefeatHorror::~BossDefeatHorror() {
}

void BossDefeatHorror::trigger(const std::string& bossName) {
    m_active = true;
    m_sequenceComplete = false;
    m_timer = 0.0f;
    m_currentPhase = 0;
    m_bossName = bossName;
    m_filesDeleted = 0;
    m_glitchIntensity = 0.0f;
    m_windowManipulation = false;

    generateFakeFiles();

    std::cout << "[BossDefeatHorror] HORROR SEQUENCE TRIGGERED for " << bossName << std::endl;
    std::cout << "[BossDefeatHorror] ⚠️  EXTREME HORROR INCOMING ⚠️" << std::endl;
}

void BossDefeatHorror::generateFakeFiles() {
    m_fakeFiles.clear();
    m_fakeFiles.push_back("C:/Users/Documents/Important.docx");
    m_fakeFiles.push_back("C:/Users/Photos/Family.jpg");
    m_fakeFiles.push_back("C:/Users/Desktop/Work_Project.zip");
    m_fakeFiles.push_back("C:/Windows/System32/critical.dll");
    m_fakeFiles.push_back("C:/Users/Downloads/Memories.mp4");
    m_fakeFiles.push_back("C:/Program Files/[YOUR_GAME_NAME]/save.dat");
    m_fakeFiles.push_back("C:/Users/Documents/Passwords.txt");
    m_fakeFiles.push_back("C:/Users/AppData/browser_history.db");
}

void BossDefeatHorror::update(float deltaTime) {
    if (!m_active) return;

    m_timer += deltaTime;
    m_shakeTimer += deltaTime;

    // Phase progression (each phase lasts ~2-3 seconds)
    if (m_currentPhase == 0 && m_timer > 2.0f) {
        m_currentPhase = 1;
        if (m_audioManager) {
            m_audioManager->playSound("whitenoise", 80);
        }
    }
    else if (m_currentPhase == 1 && m_timer > 4.0f) {
        m_currentPhase = 2;
    }
    else if (m_currentPhase == 2 && m_timer > 8.0f) {
        m_currentPhase = 3;
        // Check if camera permission granted
        if (m_permissionManager && m_permissionManager->hasCameraPermission()) {
            std::cout << "[BossDefeatHorror] CAMERA DETECTED - ACTIVATING SURVEILLANCE" << std::endl;
        }
    }
    else if (m_currentPhase == 3 && m_timer > 11.0f) {
        m_currentPhase = 4;
        if (m_audioManager) {
            m_audioManager->playSound("scare_audio", 100);
        }
    }
    else if (m_currentPhase == 4 && m_timer > 14.0f) {
        m_currentPhase = 5;
        m_screenShake = true;
        m_glitchIntensity = 5.0f;
    }
    else if (m_currentPhase == 5 && m_timer > 17.0f) {
        m_currentPhase = 6;
        m_windowManipulation = true;
    }
    else if (m_currentPhase == 6 && m_timer > 20.0f) {
        m_currentPhase = 7;
        m_windowManipulation = false;
        m_screenShake = false;
    }
    else if (m_currentPhase == 7 && m_timer > 23.0f) {
        // Sequence complete
        m_active = false;
        m_sequenceComplete = true;
        std::cout << "[BossDefeatHorror] Sequence complete. Returning to normal..." << std::endl;
    }

    // Update file deletion counter in phase 2
    if (m_currentPhase == 2) {
        float fileTimer = m_timer - 4.0f;  // Phase 2 starts at 4.0s
        m_filesDeleted = static_cast<int>(fileTimer * 2.0f);  // 2 files per second
        if (m_filesDeleted > static_cast<int>(m_fakeFiles.size())) {
            m_filesDeleted = static_cast<int>(m_fakeFiles.size());
        }
    }

    // Update glitch effects
    if (m_glitchIntensity > 0.0f) {
        m_glitchOffsetX = (rand() % static_cast<int>(m_glitchIntensity * 40)) - static_cast<int>(m_glitchIntensity * 20);
        m_glitchOffsetY = (rand() % static_cast<int>(m_glitchIntensity * 40)) - static_cast<int>(m_glitchIntensity * 20);
    }
}

void BossDefeatHorror::updateWindow(SDL_Window* window) {
    if (!m_active || !window || !m_windowManipulation) return;

    m_windowMoveTimer += 0.016f;  // Assume 60 FPS

    // Wild window movement
    int newX = 400 + static_cast<int>(std::sin(m_windowMoveTimer * 10.0f) * 300.0f);
    int newY = 300 + static_cast<int>(std::cos(m_windowMoveTimer * 8.0f) * 200.0f);

    SDL_SetWindowPosition(window, newX, newY);

    // Random window size changes
    if (static_cast<int>(m_windowMoveTimer * 10) % 5 == 0) {
        int width = 800 + (rand() % 200) - 100;
        int height = 600 + (rand() % 150) - 75;
        SDL_SetWindowSize(window, width, height);
    }
}

void BossDefeatHorror::render(SDL_Renderer* renderer) {
    if (!m_active) return;

    // Apply screen shake offset
    int shakeX = 0, shakeY = 0;
    if (m_screenShake) {
        shakeX = (rand() % 20) - 10;
        shakeY = (rand() % 20) - 10;
    }

    // Render current phase
    switch (m_currentPhase) {
        case 0: renderPhase0(renderer); break;
        case 1: renderPhase1(renderer); break;
        case 2: renderPhase2(renderer); break;
        case 3: renderPhase3(renderer); break;
        case 4: renderPhase4(renderer); break;
        case 5: renderPhase5(renderer); break;
        case 6: renderPhase6(renderer); break;
        case 7: renderPhase7(renderer); break;
    }
}

void BossDefeatHorror::renderPhase0(SDL_Renderer* renderer) {
    // Black screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void BossDefeatHorror::renderPhase1(SDL_Renderer* renderer) {
    // Black background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Glitchy text: "I'm free..."
    UIHelper::renderText(renderer, "I'm free...", 350 + m_glitchOffsetX, 280 + m_glitchOffsetY, 32, 255, 0, 0);
    UIHelper::renderText(renderer, "I'm free...", 352, 282, 32, 255, 255, 255);  // Offset ghost
}

void BossDefeatHorror::renderPhase2(SDL_Renderer* renderer) {
    // Black background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Title
    UIHelper::renderText(renderer, "DELETING SYSTEM FILES...", 250, 50, 24, 255, 0, 0);

    // File deletion list
    int y = 120;
    for (int i = 0; i < m_filesDeleted && i < static_cast<int>(m_fakeFiles.size()); i++) {
        std::string deleteMsg = "[X] " + m_fakeFiles[i];
        UIHelper::renderText(renderer, deleteMsg, 50, y, 16, 255, 50, 50);
        y += 30;
    }

    // Progress bar
    int progress = (m_filesDeleted * 100) / static_cast<int>(m_fakeFiles.size());
    SDL_Rect progressBg = {200, 500, 400, 30};
    SDL_SetRenderDrawColor(renderer, 50, 0, 0, 255);
    SDL_RenderFillRect(renderer, &progressBg);

    SDL_Rect progressFill = {200, 500, (400 * progress) / 100, 30};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &progressFill);

    std::string progressText = std::to_string(progress) + "%";
    UIHelper::renderText(renderer, progressText, 380, 505, 18, 255, 255, 255);
}

void BossDefeatHorror::renderPhase3(SDL_Renderer* renderer) {
    // Dark red background
    SDL_SetRenderDrawColor(renderer, 20, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (m_permissionManager && m_permissionManager->hasCameraPermission()) {
        UIHelper::renderText(renderer, "CAMERA: ONLINE", 300, 200, 28, 0, 255, 0);
        UIHelper::renderText(renderer, "MICROPHONE: ONLINE", 270, 250, 28, 0, 255, 0);
        UIHelper::renderText(renderer, "SURVEILLANCE: ACTIVE", 260, 300, 28, 255, 0, 0);
    } else {
        UIHelper::renderText(renderer, "Searching for cameras...", 280, 250, 24, 255, 255, 255);
    }
}

void BossDefeatHorror::renderPhase4(SDL_Renderer* renderer) {
    // Black with red text
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Pulsing text
    static float pulse = 0.0f;
    pulse += 0.05f;
    int alpha = static_cast<int>(std::sin(pulse * 5.0f) * 100.0f + 155.0f);

    if (m_permissionManager && m_permissionManager->hasCameraPermission()) {
        UIHelper::renderText(renderer, "I can see you.", 320, 250, 36, 255, 0, 0);
        UIHelper::renderText(renderer, "Outside the game.", 280, 310, 32, 255, 50, 50);
    } else {
        UIHelper::renderText(renderer, "I know you're there.", 280, 280, 32, 255, 0, 0);
    }
}

void BossDefeatHorror::renderPhase5(SDL_Renderer* renderer) {
    // Extreme glitches
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Random colored rectangles (glitch effect)
    for (int i = 0; i < 50; i++) {
        int x = rand() % 800;
        int y = rand() % 600;
        int w = rand() % 200;
        int h = rand() % 100;

        SDL_Rect glitchRect = {x, y, w, h};
        SDL_SetRenderDrawColor(renderer, rand() % 255, rand() % 255, rand() % 255, 150);
        SDL_RenderFillRect(renderer, &glitchRect);
    }

    // Distorted text
    UIHelper::renderText(renderer, "R̴̢̛E̴̡̛A̸̧̛L̶̨̛I̵̢̛T̴̡̛Y̶̧̛ ̸̨̛B̵̢̛R̴̡̛Ȩ̶̛Ą̵̛K̴̢̛I̶̡̛Ņ̵̛G̶̨̛", 200, 250, 40, 255, 0, 255);
}

void BossDefeatHorror::renderPhase6(SDL_Renderer* renderer) {
    // Final warning
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    UIHelper::renderText(renderer, "We'll meet again...", 280, 250, 32, 255, 0, 0);
    UIHelper::renderText(renderer, "Soon.", 370, 310, 28, 150, 0, 0);
}

void BossDefeatHorror::renderPhase7(SDL_Renderer* renderer) {
    // Fade to black
    int alpha = static_cast<int>((m_timer - 20.0f) / 3.0f * 255.0f);
    if (alpha > 255) alpha = 255;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
    SDL_RenderClear(renderer);
}
