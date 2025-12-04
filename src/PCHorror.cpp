#include "PCHorror.h"
#include "UIHelper.h"
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <iostream>

PCHorror::PCHorror(SDL_Window* window, AudioManager* audioManager, MetaHorror* metaHorror)
    : m_window(window)
    , m_audioManager(audioManager)
    , m_metaHorror(metaHorror)
    , m_windowManipulated(false)
    , m_cursorOffsetX(0)
    , m_cursorOffsetY(0)
{
    // Store original window position and size
    SDL_GetWindowPosition(m_window, &m_originalWindowX, &m_originalWindowY);
    SDL_GetWindowSize(m_window, &m_originalWindowW, &m_originalWindowH);

    initializeFakeLists();
}

PCHorror::~PCHorror() {
    clearAll();
    restoreWindow();
}

void PCHorror::initializeFakeLists() {
    // Fake personal files for deletion/encryption effects
    m_fakePersonalFiles = {
        "Documents/personal_diary.docx",
        "Pictures/family_photos_2024/",
        "Desktop/passwords.txt",
        "Documents/bank_statements.pdf",
        "Pictures/vacation_2024/",
        "Documents/tax_returns_2023.xlsx",
        "Downloads/important_contract.pdf",
        "Music/favorite_playlist/",
        "Videos/memories.mp4",
        "Documents/thesis_final.docx",
        "Desktop/crypto_wallet_backup.dat",
        "Pictures/private/",
        "Documents/resume.pdf",
        "Desktop/notes.txt"
    };

    // Fake system files for corruption effects
    m_fakeSystemFiles = {
        "C:/Windows/System32/kernel32.dll",
        "C:/Windows/System32/ntdll.dll",
        "C:/Windows/System32/user32.dll",
        "C:/Windows/System32/advapi32.dll",
        "C:/Windows/explorer.exe",
        "C:/Windows/System32/drivers/ntfs.sys",
        "C:/Windows/System32/hal.dll",
        "/usr/lib/libc.so.6",
        "/usr/bin/bash",
        "/etc/passwd"
    };

    // Fake virus threats
    m_fakeVirusThreats = {
        "Trojan.Win32.Echoes.A",
        "Backdoor.ECHOES.Gen",
        "Rootkit.Reality.Breach",
        "Spyware.Surveillance.EXE",
        "Ransomware.FileEncryptor.666",
        "Keylogger.Silent.Observer",
        "Worm.NetworkSpreader.Echo",
        "RAT.RemoteAccess.Prime"
    };
}

void PCHorror::triggerEffect(PCHorrorType type, float duration, float intensity) {
    auto effect = std::make_unique<PCHorrorEffect>(type, duration, intensity);
    effect->isActive = true;

    // Setup effect-specific data
    switch (type) {
        case PCHorrorType::FAKE_NOTIFICATION:
            effect->messages = {
                "ECHOES.exe has accessed your webcam",
                "ECHOES.exe is reading your files",
                "Unusual network activity detected",
                "Your microphone is being recorded"
            };
            break;

        case PCHorrorType::FAKE_RANSOMWARE:
            effect->messages = {
                "YOUR FILES ARE BEING ENCRYPTED",
                "All personal files will be locked in",
                "Pay 0.5 BTC to recover your data"
            };
            if (m_audioManager) m_audioManager->playSound("glitch", 100);
            break;

        case PCHorrorType::FAKE_VIRUS_DETECTED:
            effect->messages = {
                "CRITICAL THREAT DETECTED",
                "8 viruses found in ECHOES.exe",
                "Immediate action required"
            };
            if (m_audioManager) m_audioManager->playSound("jumpscare", 100);
            break;

        case PCHorrorType::FAKE_FILE_DELETION:
            effect->messages = m_fakePersonalFiles;
            if (m_audioManager) m_audioManager->playSound("zap", 80);
            break;

        case PCHorrorType::FAKE_SYSTEM_CORRUPTION:
            effect->messages = m_fakeSystemFiles;
            if (m_audioManager) m_audioManager->playSound("whitenoise", 90);
            break;

        case PCHorrorType::FAKE_SHUTDOWN:
            effect->messages = {
                "Windows is shutting down...",
                "Saving your work...",
                "Closing applications...",
                "Don't turn off your computer"
            };
            if (m_audioManager) m_audioManager->playMusic("horror_ambient");
            break;

        case PCHorrorType::FAKE_WEBCAM_ACCESS:
            effect->messages = {
                "Camera: ON",
                "Recording started",
                "Saving to: unknown_location.mp4"
            };
            // Create fake screenshot file
            if (m_metaHorror) {
                m_metaHorror->createFile("webcam_capture_001.jpg", "I can see you...");
            }
            break;

        case PCHorrorType::FAKE_KEYLOGGER:
            effect->messages = {
                "Keylogger active",
                "Recording all keystrokes",
                "Sending data to: 192.168.xxx.xxx"
            };
            break;

        case PCHorrorType::FAKE_CONTACT_ACCESS:
            effect->messages = {
                "Accessing email contacts...",
                "Accessing social media friends...",
                "Copying phone numbers...",
                "Extracting personal information..."
            };
            break;

        default:
            break;
    }

    m_activeEffects.push_back(std::move(effect));

    std::cout << "🔥 PC HORROR TRIGGERED: Type " << static_cast<int>(type) << std::endl;
}

void PCHorror::triggerRandomForLevel(int level) {
    // Different horror effects based on level progression
    std::vector<PCHorrorType> availableEffects;

    if (level >= 3) {
        // Level 3 - First warnings
        availableEffects = {
            PCHorrorType::FAKE_FILE_SCAN,
            PCHorrorType::FAKE_NOTIFICATION,
            PCHorrorType::CURSOR_GLITCH
        };
    }

    if (level >= 5) {
        // Level 5 - Escalation
        availableEffects = {
            PCHorrorType::FAKE_RANSOMWARE,
            PCHorrorType::FAKE_VIRUS_DETECTED,
            PCHorrorType::WINDOW_CHAOS_MILD,
            PCHorrorType::FAKE_WEBCAM_ACCESS
        };
    }

    if (level >= 7) {
        // Level 7 - Severe
        availableEffects = {
            PCHorrorType::FAKE_FILE_DELETION,
            PCHorrorType::FAKE_SYSTEM_CORRUPTION,
            PCHorrorType::WINDOW_CHAOS_EXTREME,
            PCHorrorType::FAKE_KEYLOGGER,
            PCHorrorType::FAKE_CONTACT_ACCESS
        };
    }

    if (level >= 9) {
        // Level 9 - NIGHTMARE
        availableEffects = {
            PCHorrorType::FAKE_SHUTDOWN,
            PCHorrorType::FAKE_RANSOMWARE_PAYMENT,
            PCHorrorType::DESKTOP_TAKEOVER,
            PCHorrorType::AUDIO_PERSISTENCE
        };
    }

    if (!availableEffects.empty()) {
        int index = rand() % availableEffects.size();
        float duration = 5.0f + (rand() % 10); // 5-15 seconds
        float intensity = 0.5f + (rand() % 50) / 100.0f; // 0.5 - 1.0
        triggerEffect(availableEffects[index], duration, intensity);
    }
}

void PCHorror::update(float deltaTime) {
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
            case PCHorrorType::WINDOW_CHAOS_MILD:
            case PCHorrorType::WINDOW_CHAOS_EXTREME:
                updateWindowChaos(effect.get(), deltaTime);
                break;

            case PCHorrorType::CURSOR_GLITCH:
                updateCursorGlitch(effect.get(), deltaTime);
                break;

            case PCHorrorType::AUDIO_PERSISTENCE:
                updateAudioPersistence(effect.get(), deltaTime);
                break;

            default:
                break;
        }

        // Check if effect expired
        if (effect->timer >= effect->duration) {
            effect->isActive = false;
            it = m_activeEffects.erase(it);
        } else {
            ++it;
        }
    }
}

void PCHorror::render(SDL_Renderer* renderer) {
    for (auto& effect : m_activeEffects) {
        if (!effect->isActive) continue;

        switch (effect->type) {
            case PCHorrorType::FAKE_FILE_SCAN:
                renderFileScan(renderer, effect.get());
                break;

            case PCHorrorType::FAKE_NOTIFICATION:
                renderNotification(renderer, effect.get());
                break;

            case PCHorrorType::CURSOR_GLITCH:
                renderCursorGlitch(renderer, effect.get());
                break;

            case PCHorrorType::FAKE_RANSOMWARE:
                renderRansomware(renderer, effect.get());
                break;

            case PCHorrorType::FAKE_VIRUS_DETECTED:
                renderVirusDetected(renderer, effect.get());
                break;

            case PCHorrorType::FAKE_FILE_DELETION:
                renderFileDeletion(renderer, effect.get());
                break;

            case PCHorrorType::FAKE_SYSTEM_CORRUPTION:
                renderSystemCorruption(renderer, effect.get());
                break;

            case PCHorrorType::FAKE_KEYLOGGER:
                renderKeylogger(renderer, effect.get());
                break;

            case PCHorrorType::FAKE_CONTACT_ACCESS:
                renderContactAccess(renderer, effect.get());
                break;

            case PCHorrorType::FAKE_SHUTDOWN:
                renderShutdown(renderer, effect.get());
                break;

            case PCHorrorType::FAKE_RANSOMWARE_PAYMENT:
                renderRansomwarePayment(renderer, effect.get());
                break;

            case PCHorrorType::DESKTOP_TAKEOVER:
                renderDesktopTakeover(renderer, effect.get());
                break;

            case PCHorrorType::FAKE_WEBCAM_ACCESS:
                renderWebcamAccess(renderer, effect.get());
                break;

            default:
                break;
        }
    }
}

void PCHorror::updateWindow() {
    for (auto& effect : m_activeEffects) {
        if (!effect->isActive) continue;

        if (effect->type == PCHorrorType::WINDOW_CHAOS_MILD ||
            effect->type == PCHorrorType::WINDOW_CHAOS_EXTREME) {

            bool isExtreme = (effect->type == PCHorrorType::WINDOW_CHAOS_EXTREME);
            float shake = isExtreme ? 400.0f : 100.0f;
            float time = effect->timer;

            // Chaotic window movement
            int newX = m_originalWindowX + static_cast<int>(std::sin(time * 5.0f) * shake);
            int newY = m_originalWindowY + static_cast<int>(std::cos(time * 3.0f) * shake);

            SDL_SetWindowPosition(m_window, newX, newY);

            if (isExtreme) {
                // Also change window size
                int sizeOffset = static_cast<int>(std::sin(time * 7.0f) * 100.0f);
                SDL_SetWindowSize(m_window,
                    m_originalWindowW + sizeOffset,
                    m_originalWindowH + sizeOffset);
            }

            m_windowManipulated = true;
        }
    }
}

// ============================================================================
// EFFECT RENDERERS
// ============================================================================

void PCHorror::renderFileScan(SDL_Renderer* renderer, PCHorrorEffect* effect) {
    // Fake virus scan overlay
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect overlay = {0, 0, 800, 600};
    SDL_RenderFillRect(renderer, &overlay);

    UIHelper::renderText(renderer, "SCANNING FOR THREATS...", 200, 50, 24, 255, 165, 0);

    int progress = static_cast<int>((effect->timer / effect->duration) * 100);
    std::string progressText = "Progress: " + std::to_string(progress) + "%";
    UIHelper::renderText(renderer, progressText, 300, 150, 18, 200, 200, 200);

    // Progress bar
    SDL_Rect progressBg = {200, 200, 400, 30};
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &progressBg);

    SDL_Rect progressBar = {200, 200, (progress * 400) / 100, 30};
    SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
    SDL_RenderFillRect(renderer, &progressBar);

    // Fake found threats
    int threatsFound = (progress / 20) + 1;
    std::string threatsText = "Threats found: " + std::to_string(threatsFound);
    UIHelper::renderText(renderer, threatsText, 250, 280, 18, 255, 0, 0);
}

void PCHorror::renderNotification(SDL_Renderer* renderer, PCHorrorEffect* effect) {
    // System notification style (top right corner)
    SDL_Rect notifBg = {550, 20, 240, 100};
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 230);
    SDL_RenderFillRect(renderer, &notifBg);

    // Border
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &notifBg);

    // Cycling through messages
    int messageIndex = static_cast<int>(effect->timer * 0.5f) % effect->messages.size();
    std::string message = effect->messages[messageIndex];

    UIHelper::renderText(renderer, "SYSTEM ALERT", 565, 35, 14, 255, 255, 0);
    UIHelper::renderText(renderer, message.substr(0, 25), 565, 60, 12, 255, 255, 255);

    if (message.length() > 25) {
        UIHelper::renderText(renderer, message.substr(25), 565, 80, 12, 255, 255, 255);
    }
}

void PCHorror::renderCursorGlitch(SDL_Renderer* renderer, PCHorrorEffect* effect) {
    // Draw fake cursor trails
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    for (int i = 0; i < 5; i++) {
        int offsetX = (rand() % 60) - 30;
        int offsetY = (rand() % 60) - 30;

        SDL_Rect fakeCursor = {mouseX + offsetX, mouseY + offsetY, 12, 18};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 150);
        SDL_RenderFillRect(renderer, &fakeCursor);
    }
}

void PCHorror::renderRansomware(SDL_Renderer* renderer, PCHorrorEffect* effect) {
    // Full screen ransomware overlay
    SDL_SetRenderDrawColor(renderer, 20, 0, 0, 240);
    SDL_Rect fullScreen = {0, 0, 800, 600};
    SDL_RenderFillRect(renderer, &fullScreen);

    UIHelper::renderText(renderer, "YOUR FILES ARE BEING ENCRYPTED", 150, 100, 24, 255, 0, 0);

    // Countdown timer
    int timeLeft = static_cast<int>(effect->duration - effect->timer);
    std::string timerText = "Time remaining: " + std::to_string(timeLeft) + " seconds";
    UIHelper::renderText(renderer, timerText, 250, 180, 18, 255, 100, 100);

    // Fake file counter
    int filesEncrypted = static_cast<int>(effect->timer * 100);
    std::string fileText = "Files encrypted: " + std::to_string(filesEncrypted);
    UIHelper::renderText(renderer, fileText, 280, 250, 16, 255, 255, 0);

    UIHelper::renderText(renderer, "All your personal files are being locked", 200, 320, 14, 200, 200, 200);
    UIHelper::renderText(renderer, "Pay 0.5 BTC to: " + generateFakeBitcoinAddress(), 180, 360, 14, 255, 165, 0);

    // Flashing warning
    if (static_cast<int>(effect->timer * 2) % 2 == 0) {
        UIHelper::renderText(renderer, "DO NOT CLOSE THIS WINDOW", 240, 450, 18, 255, 0, 0);
    }
}

void PCHorror::renderVirusDetected(SDL_Renderer* renderer, PCHorrorEffect* effect) {
    // Virus scanner alert
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 220);
    SDL_Rect alertBg = {100, 100, 600, 400};
    SDL_RenderFillRect(renderer, &alertBg);

    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &alertBg);

    UIHelper::renderText(renderer, "CRITICAL THREAT DETECTED", 220, 130, 22, 255, 0, 0);
    UIHelper::renderText(renderer, "Virus Scan Results:", 150, 200, 18, 255, 255, 0);

    // List fake viruses
    int y = 240;
    for (size_t i = 0; i < std::min(m_fakeVirusThreats.size(), size_t(5)); i++) {
        std::string threat = "[X] " + m_fakeVirusThreats[i];
        UIHelper::renderText(renderer, threat, 150, y, 14, 255, 100, 100);
        y += 30;
    }

    UIHelper::renderText(renderer, "Severity: CRITICAL", 150, 430, 16, 255, 0, 0);
    UIHelper::renderText(renderer, "Recommended: Immediate system shutdown", 150, 460, 14, 200, 200, 200);
}

void PCHorror::renderFileDeletion(SDL_Renderer* renderer, PCHorrorEffect* effect) {
    // Fake file deletion process
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 230);
    SDL_Rect bg = {50, 50, 700, 500};
    SDL_RenderFillRect(renderer, &bg);

    UIHelper::renderText(renderer, "DELETING PERSONAL FILES...", 220, 80, 22, 255, 0, 0);

    int filesDeleted = static_cast<int>((effect->timer / effect->duration) * effect->messages.size());

    int y = 140;
    for (int i = 0; i < filesDeleted && i < static_cast<int>(effect->messages.size()); i++) {
        if (y > 500) break;
        std::string deleteMsg = "[DELETED] " + effect->messages[i];
        UIHelper::renderText(renderer, deleteMsg, 80, y, 14, 255, 50, 50);
        y += 25;
    }

    // Progress
    int progress = (filesDeleted * 100) / effect->messages.size();
    std::string progressText = "Progress: " + std::to_string(progress) + "%";
    UIHelper::renderText(renderer, progressText, 320, 520, 16, 255, 255, 0);
}

void PCHorror::renderSystemCorruption(SDL_Renderer* renderer, PCHorrorEffect* effect) {
    // System file corruption screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 50, 240);
    SDL_Rect fullScreen = {0, 0, 800, 600};
    SDL_RenderFillRect(renderer, &fullScreen);

    UIHelper::renderText(renderer, "SYSTEM CORRUPTION DETECTED", 200, 60, 24, 255, 255, 255);

    UIHelper::renderText(renderer, "Critical system files have been corrupted:", 150, 140, 16, 255, 0, 0);

    int y = 180;
    int filesShown = static_cast<int>((effect->timer / effect->duration) * effect->messages.size());

    for (int i = 0; i < filesShown && i < static_cast<int>(effect->messages.size()); i++) {
        if (y > 500) break;
        std::string corruptMsg = "[CORRUPTED] " + effect->messages[i];
        UIHelper::renderText(renderer, corruptMsg, 80, y, 12, 255, 100, 100);
        y += 22;
    }

    // Flashing error
    if (static_cast<int>(effect->timer * 3) % 2 == 0) {
        UIHelper::renderText(renderer, "WINDOWS CANNOT CONTINUE", 240, 550, 18, 255, 0, 0);
    }
}

void PCHorror::renderKeylogger(SDL_Renderer* renderer, PCHorrorEffect* effect) {
    // Keylogger warning
    SDL_Rect warningBox = {150, 200, 500, 200};
    SDL_SetRenderDrawColor(renderer, 40, 0, 0, 220);
    SDL_RenderFillRect(renderer, &warningBox);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &warningBox);

    UIHelper::renderText(renderer, "KEYLOGGER ACTIVE", 300, 230, 20, 255, 0, 0);
    UIHelper::renderText(renderer, "Recording all keystrokes...", 250, 280, 14, 200, 200, 200);

    std::string ip = "Sending to: " + generateFakeIPAddress();
    UIHelper::renderText(renderer, ip, 250, 310, 14, 255, 165, 0);

    // Fake keystroke log
    UIHelper::renderText(renderer, "Recent: p@ssw0rd1234...", 250, 350, 12, 150, 150, 150);
}

void PCHorror::renderContactAccess(SDL_Renderer* renderer, PCHorrorEffect* effect) {
    // Contact harvesting screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 230);
    SDL_Rect bg = {100, 100, 600, 400};
    SDL_RenderFillRect(renderer, &bg);

    UIHelper::renderText(renderer, "ACCESSING PERSONAL DATA", 240, 130, 20, 255, 0, 0);

    int phase = static_cast<int>(effect->timer / 2.0f) % effect->messages.size();

    int y = 200;
    for (int i = 0; i <= phase && i < static_cast<int>(effect->messages.size()); i++) {
        std::string msg = "[✓] " + effect->messages[i];
        UIHelper::renderText(renderer, msg, 150, y, 14, 0, 255, 0);
        y += 40;
    }

    // Fake data counter
    int dataStolen = static_cast<int>(effect->timer * 50);
    std::string dataText = "Contacts harvested: " + std::to_string(dataStolen);
    UIHelper::renderText(renderer, dataText, 250, 450, 14, 255, 165, 0);
}

void PCHorror::renderShutdown(SDL_Renderer* renderer, PCHorrorEffect* effect) {
    // Fake Windows shutdown screen
    SDL_SetRenderDrawColor(renderer, 0, 120, 215, 255);  // Windows blue
    SDL_Rect fullScreen = {0, 0, 800, 600};
    SDL_RenderFillRect(renderer, &fullScreen);

    int phase = static_cast<int>(effect->timer / 2.0f) % effect->messages.size();
    std::string message = effect->messages[phase];

    UIHelper::renderText(renderer, message, 250, 250, 20, 255, 255, 255);

    // Spinning circle (fake loading)
    int cx = 400;
    int cy = 320;
    int radius = 30;
    float angle = effect->timer * 2.0f;

    for (int i = 0; i < 8; i++) {
        float a = angle + (i * 3.14159f / 4.0f);
        int x = cx + static_cast<int>(std::cos(a) * radius);
        int y = cy + static_cast<int>(std::sin(a) * radius);

        SDL_Rect dot = {x - 3, y - 3, 6, 6};
        int alpha = 255 - (i * 30);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
        SDL_RenderFillRect(renderer, &dot);
    }

    UIHelper::renderText(renderer, "Please don't turn off your PC", 260, 400, 14, 220, 220, 220);
}

void PCHorror::renderRansomwarePayment(SDL_Renderer* renderer, PCHorrorEffect* effect) {
    // Full ransomware payment demand
    SDL_SetRenderDrawColor(renderer, 10, 0, 0, 250);
    SDL_Rect fullScreen = {0, 0, 800, 600};
    SDL_RenderFillRect(renderer, &fullScreen);

    UIHelper::renderText(renderer, "YOUR FILES HAVE BEEN ENCRYPTED", 140, 60, 24, 255, 0, 0);

    UIHelper::renderText(renderer, "All your documents, photos, and files are locked.", 150, 140, 14, 200, 200, 200);
    UIHelper::renderText(renderer, "The only way to recover them is to pay the ransom.", 150, 170, 14, 200, 200, 200);

    UIHelper::renderText(renderer, "Send 0.5 BTC to:", 150, 240, 18, 255, 165, 0);
    UIHelper::renderText(renderer, generateFakeBitcoinAddress(), 150, 270, 14, 255, 255, 0);

    // Countdown
    int hoursLeft = static_cast<int>((effect->duration - effect->timer) / 3600.0f) + 48;
    std::string countdown = "Time until price doubles: " + std::to_string(hoursLeft) + " hours";
    UIHelper::renderText(renderer, countdown, 200, 340, 16, 255, 0, 0);

    UIHelper::renderText(renderer, "Files encrypted: 1,247", 150, 400, 14, 150, 150, 150);
    UIHelper::renderText(renderer, "Files deleted permanently in: 72 hours", 150, 430, 14, 255, 0, 0);

    // Warning
    UIHelper::renderText(renderer, "Any attempt to remove this software will result", 150, 500, 12, 200, 200, 200);
    UIHelper::renderText(renderer, "in immediate destruction of your private key.", 150, 520, 12, 200, 200, 200);
}

void PCHorror::renderDesktopTakeover(SDL_Renderer* renderer, PCHorrorEffect* effect) {
    // Fake desktop with multiple windows
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_Rect fullScreen = {0, 0, 800, 600};
    SDL_RenderFillRect(renderer, &fullScreen);

    UIHelper::renderText(renderer, "I AM EVERYWHERE", 280, 50, 24, 255, 0, 0);

    // Multiple fake error windows
    for (int i = 0; i < 4; i++) {
        int x = 100 + (i % 2) * 350;
        int y = 150 + (i / 2) * 200;

        SDL_Rect window = {x, y, 300, 150};
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &window);

        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &window);

        std::string title = "ERROR_" + std::to_string(i + 1);
        UIHelper::renderText(renderer, title, x + 10, y + 10, 14, 255, 0, 0);
        UIHelper::renderText(renderer, "I can see you", x + 20, y + 60, 12, 200, 200, 200);
    }
}

void PCHorror::renderWebcamAccess(SDL_Renderer* renderer, PCHorrorEffect* effect) {
    // Webcam recording indicator (top left corner)
    SDL_Rect indicator = {10, 10, 200, 80};
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 230);
    SDL_RenderFillRect(renderer, &indicator);

    // Blinking red dot
    if (static_cast<int>(effect->timer * 3) % 2 == 0) {
        SDL_Rect redDot = {20, 25, 15, 15};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &redDot);
    }

    UIHelper::renderText(renderer, "REC", 45, 23, 16, 255, 255, 255);
    UIHelper::renderText(renderer, "Camera: ON", 20, 50, 12, 255, 255, 255);

    int seconds = static_cast<int>(effect->timer);
    std::string timeText = std::string("00:") + (seconds < 10 ? "0" : "") + std::to_string(seconds);
    UIHelper::renderText(renderer, timeText, 20, 70, 12, 255, 255, 255);
}

// ============================================================================
// EFFECT UPDATERS
// ============================================================================

void PCHorror::updateWindowChaos(PCHorrorEffect* effect, float deltaTime) {
    // Window manipulation is handled in updateWindow()
}

void PCHorror::updateCursorGlitch(PCHorrorEffect* effect, float deltaTime) {
    // Randomly offset cursor position (visual only)
    if (static_cast<int>(effect->timer * 10) % 5 == 0) {
        m_cursorOffsetX = (rand() % 40) - 20;
        m_cursorOffsetY = (rand() % 40) - 20;
    }
}

void PCHorror::updateAudioPersistence(PCHorrorEffect* effect, float deltaTime) {
    // Play random horror sounds
    if (m_audioManager && static_cast<int>(effect->timer) % 3 == 0) {
        m_audioManager->playSound("whitenoise", 50);
    }
}

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

bool PCHorror::hasActiveEffects() const {
    return !m_activeEffects.empty();
}

void PCHorror::clearAll() {
    m_activeEffects.clear();
    restoreWindow();
}

void PCHorror::restoreWindow() {
    if (m_windowManipulated && m_window) {
        SDL_SetWindowPosition(m_window, m_originalWindowX, m_originalWindowY);
        SDL_SetWindowSize(m_window, m_originalWindowW, m_originalWindowH);
        m_windowManipulated = false;
    }
}

std::string PCHorror::generateFakeBitcoinAddress() {
    // Generate fake but realistic-looking Bitcoin address
    const char* chars = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
    std::string address = "1";

    for (int i = 0; i < 33; i++) {
        address += chars[rand() % 58];
    }

    return address;
}

std::string PCHorror::generateFakeIPAddress() {
    std::stringstream ss;
    ss << (rand() % 255) << "."
       << (rand() % 255) << "."
       << (rand() % 255) << "."
       << (rand() % 255);
    return ss.str();
}
