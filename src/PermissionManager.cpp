#include "PermissionManager.h"
#include <SDL2/SDL.h>
#include <iostream>

PermissionManager::PermissionManager()
    : m_cameraAllowed(false)
    , m_microphoneAllowed(false)
    , m_fileCreationAllowed(true) // Dateien sind standardmäßig erlaubt (nur Downloads)
{
}

PermissionManager::~PermissionManager() {
}

void PermissionManager::requestCameraPermission() {
    std::string message =
        "ECHOES wants to access your camera.\n\n"
        "This will be used to:\n"
        "- Take ONE snapshot for horror effects\n"
        "- Display your image in-game (glitched)\n\n"
        "Your image will ONLY be stored locally.\n"
        "You can revoke this permission anytime.\n\n"
        "Allow camera access?";

    m_cameraAllowed = showPermissionDialog("Camera Permission", message);

    if (m_cameraAllowed) {
        std::cout << "[Permission] Camera access GRANTED" << std::endl;
    } else {
        std::cout << "[Permission] Camera access DENIED" << std::endl;
    }
}

void PermissionManager::requestMicrophonePermission() {
    std::string message =
        "ECHOES wants to access your microphone.\n\n"
        "This will be used to:\n"
        "- Detect loud sounds (screams, talking)\n"
        "- Make the game react to your voice\n\n"
        "Audio is NOT recorded or saved.\n"
        "Only volume levels are detected.\n\n"
        "Allow microphone access?";

    m_microphoneAllowed = showPermissionDialog("Microphone Permission", message);

    if (m_microphoneAllowed) {
        std::cout << "[Permission] Microphone access GRANTED" << std::endl;
    } else {
        std::cout << "[Permission] Microphone access DENIED" << std::endl;
    }
}

bool PermissionManager::showPermissionDialog(const std::string& title, const std::string& message) {
    // SDL Message Box mit Ja/Nein
    const SDL_MessageBoxButtonData buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Allow" },
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Deny" },
    };

    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_WARNING,
        NULL,
        title.c_str(),
        message.c_str(),
        SDL_arraysize(buttons),
        buttons,
        NULL
    };

    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        std::cerr << "[Permission] Failed to show dialog: " << SDL_GetError() << std::endl;
        return false;
    }

    return (buttonid == 1); // 1 = Allow
}
