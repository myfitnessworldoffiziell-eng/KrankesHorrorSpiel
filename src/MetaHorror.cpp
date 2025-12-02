#include "MetaHorror.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <cstdlib>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#else
#include <unistd.h>
#include <pwd.h>
#endif

namespace fs = std::filesystem;

MetaHorror::MetaHorror()
    : m_initialized(false)
    , m_filesCreated(0)
    , m_timeSinceLastEvent(0.0f)
    , m_totalPlaytime(0)
    , m_totalDeaths(0)
    , m_lastPlayedTime(0)
{
}

MetaHorror::~MetaHorror() {
    savePersistentData();
}

void MetaHorror::initialize() {
    std::cout << "[MetaHorror] Initializing meta-horror systems..." << std::endl;

    loadPersistentData();

    // Erste versteckte Datei im Downloads-Ordner
    std::string welcomeMsg = "Welcome to ECHOES...\n\n";
    welcomeMsg += "You found this file. Good.\n";
    welcomeMsg += "It means you're curious.\n";
    welcomeMsg += "Keep playing. You'll find more.\n\n";
    welcomeMsg += "- Echo";

    createDownloadsFile("ECHOES_README.txt", welcomeMsg);

    m_initialized = true;
    std::cout << "[MetaHorror] Initialization complete." << std::endl;
}

void MetaHorror::update(float deltaTime, int corruptionLevel) {
    m_timeSinceLastEvent += deltaTime;
    m_totalPlaytime += static_cast<int>(deltaTime);

    // Random Events basierend auf corruption level
    if (m_timeSinceLastEvent > 60.0f) { // Alle 60 Sekunden
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 100);

        int chance = dis(gen);
        if (chance < corruptionLevel) {
            // Event triggern!
            createCreepyFile();
            m_timeSinceLastEvent = 0.0f;
        }
    }
}

void MetaHorror::onGameExit() {
    std::cout << "[MetaHorror] Game exiting... creating final message..." << std::endl;

    // Letzte Nachricht beim Beenden
    std::stringstream ss;
    ss << "Session ended.\n\n";
    ss << "Playtime: " << m_totalPlaytime << " seconds\n";
    ss << "Deaths: " << m_totalDeaths << "\n\n";
    ss << "See you soon, " << getUserName() << "...\n\n";
    ss << "I'll be waiting.\n";
    ss << "- Echo";

    createDownloadsFile("ECHOES_SESSION_LOG.txt", ss.str());

    savePersistentData();
}

void MetaHorror::createCreepyFile() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 4);

    switch (dis(gen)) {
        case 0: {
            std::string msg = "I can see you playing.\n";
            msg += "You've been here for " + std::to_string(m_totalPlaytime) + " seconds.\n";
            msg += "Are you enjoying yourself?\n\n";
            msg += "- Echo";
            createDownloadsFile("DONT_OPEN.txt", msg);
            break;
        }
        case 1: {
            std::string msg = generateCorruptedText();
            createDownloadsFile("CORRUPTED_DATA.txt", msg);
            break;
        }
        case 2: {
            std::string msg = "Help me...\n";
            msg += "I'm trapped in the code.\n";
            msg += "Every player... another chance...\n";
            msg += "But it never works.\n\n";
            msg += "- M. Reeves (Developer)";
            createDownloadsFile("HELP_ME.txt", msg);
            break;
        }
        case 3: {
            std::string msg = "SYSTEM LOG:\n";
            msg += "================================\n";
            msg += "[WARNING] Anomaly detected\n";
            msg += "[ERROR] Player consciousness merging with game\n";
            msg += "[CRITICAL] Cannot reverse process\n";
            msg += "================================\n";
            createDownloadsFile("SYSTEM_LOG.txt", msg);
            break;
        }
        case 4: {
            std::string msg = "You can't stop playing, can you?\n\n";
            msg += "It's okay. I understand.\n";
            msg += "I can't stop either.\n\n";
            msg += "We're the same now.\n";
            createDownloadsFile("I_SEE_YOU.txt", msg);
            break;
        }
    }

    m_filesCreated++;
    std::cout << "[MetaHorror] Creepy file created (" << m_filesCreated << " total)" << std::endl;
}

void MetaHorror::createFile(const std::string& name, const std::string& content) {
    // General file creation - creates in Downloads folder
    createDownloadsFile(name, content);
}

void MetaHorror::manipulateWindow(SDL_Window* window) {
    if (!window) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 3);

    switch (dis(gen)) {
        case 0:
            // Fenster minimieren und sofort wiederherstellen
            std::cout << "[MetaHorror] Minimizing window..." << std::endl;
            SDL_MinimizeWindow(window);
            SDL_Delay(1000);
            SDL_RestoreWindow(window);
            break;

        case 1:
            // Fenster verschieben
            {
                std::uniform_int_distribution<> posDis(-50, 50);
                int offsetX = posDis(gen);
                int offsetY = posDis(gen);
                int x, y;
                SDL_GetWindowPosition(window, &x, &y);
                SDL_SetWindowPosition(window, x + offsetX, y + offsetY);
                std::cout << "[MetaHorror] Window moved..." << std::endl;
            }
            break;

        case 2:
            // Titel ändern
            {
                static const char* titles[] = {
                    "ECHOES - I see you",
                    "ECHOES - Are you still there?",
                    "ECHOES - Why are you playing?",
                    "ECHOES - You can't escape",
                    "E̸C̷H̶O̸E̷S̸ - C̴O̷R̷R̸U̴P̷T̸E̴D̷"
                };
                std::uniform_int_distribution<> titleDis(0, 4);
                SDL_SetWindowTitle(window, titles[titleDis(gen)]);
                std::cout << "[MetaHorror] Window title changed..." << std::endl;
            }
            break;

        case 3:
            // Fenster schütteln (mehrfach verschieben)
            {
                int x, y;
                SDL_GetWindowPosition(window, &x, &y);
                for (int i = 0; i < 10; ++i) {
                    std::uniform_int_distribution<> shakeDis(-10, 10);
                    SDL_SetWindowPosition(window, x + shakeDis(gen), y + shakeDis(gen));
                    SDL_Delay(50);
                }
                SDL_SetWindowPosition(window, x, y);
                std::cout << "[MetaHorror] Window shaken..." << std::endl;
            }
            break;
    }
}

std::string MetaHorror::getDownloadsPath() {
#ifdef _WIN32
    char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, path))) {
        return std::string(path) + "\\Downloads\\";
    }
#else
    const char* homeDir = getenv("HOME");
    if (homeDir) {
        return std::string(homeDir) + "/Downloads/";
    }
#endif
    return "./"; // Fallback: aktuelles Verzeichnis
}

std::string MetaHorror::getDesktopPath() {
#ifdef _WIN32
    char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_DESKTOP, NULL, 0, path))) {
        return std::string(path) + "\\";
    }
#else
    const char* homeDir = getenv("HOME");
    if (homeDir) {
        return std::string(homeDir) + "/Desktop/";
    }
#endif
    return "./"; // Fallback
}

std::string MetaHorror::getUserName() {
#ifdef _WIN32
    char username[256];
    DWORD username_len = 256;
    if (GetUserNameA(username, &username_len)) {
        return std::string(username);
    }
#else
    const char* username = getenv("USER");
    if (username) {
        return std::string(username);
    }
#endif
    return "Player";
}

void MetaHorror::createDownloadsFile(const std::string& filename, const std::string& content) {
    try {
        std::string fullPath = getDownloadsPath() + filename;
        std::ofstream file(fullPath);
        if (file.is_open()) {
            file << content;
            file.close();
            std::cout << "[MetaHorror] File created: " << fullPath << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "[MetaHorror] Failed to create file: " << e.what() << std::endl;
    }
}

void MetaHorror::createDesktopFile(const std::string& filename, const std::string& content) {
    try {
        std::string fullPath = getDesktopPath() + filename;
        std::ofstream file(fullPath);
        if (file.is_open()) {
            file << content;
            file.close();
            std::cout << "[MetaHorror] File created: " << fullPath << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "[MetaHorror] Failed to create file: " << e.what() << std::endl;
    }
}

std::string MetaHorror::generateCorruptedText() {
    std::string corrupted = "�̷̴̵̶̷̸̡̢̨̡̢̧̨̛̛̛̛̛̗̘̙̜̝̞̟̠̣̤̥̦̩̪̫̬̭̮̯̰̱̲̳̖̗̘̙̚̕̚͜͝͞͠͡҉̴̵̶̷̸̢̧̨̛̗̘̙̜̝̞̟̠̣̤̥̦̩̪̫̬̭̮̯̰̱̲̳́̀́̂̃̄̅̆̇̈̉̊̋̌̍̎̏̐̑̒̓̔̽̾̿̀́͂̓̈́͆͊͋͌̕̚͜͝͞͠\n";
    corrupted += "SYSTEM CORRUPTED\n";
    corrupted += "E̴C̷H̶O̸ ̷I̸S̷ ̴F̷R̶E̸E̷\n";
    corrupted += "R̸̨̧̛̛̛̗̘̙̜̝̞̟̠̣̤̥̦̩̪̫̬̭̮̯̰̱̲̳E̴̵̶̷̸̡̢̧̨̛̖̗̘̙A̸̡̢̧̨̛̛̛̗̘̙̜̝̞̟̠̣̤L̴̵̶̷̸̡̢̧̨̛̗̘̙\n";
    corrupted += "\n01001000 01000101 01001100 01010000\n";
    corrupted += "T̷̨̧̛̛̛̗̘̙̜̝̞̟̠̣̤̥̦̩̪̫̬̭̮̯̰̱̲̳H̴̵̶̷̸̡̢̧̨̛̖̗̘̙E̸̡̢̧̨̛̛̛̗̘̙̜̝̞̟̠̣̤ ̴̵̶̷̸̡̢̧̨̛̗̘̙C̷̨̧̛̛̛̗̘̙̜̝̞̟̠̣̤̥̦̩̪̫̬̭̮̯̰̱̲̳Ơ̴̵̶̷̸̡̢̧̨̖̗̘̙D̸̡̢̧̨̛̛̛̗̘̙̜̝̞̟̠̣̤E̴̵̶̷̸̡̢̧̨̛̗̘̙\n";
    return corrupted;
}

void MetaHorror::loadPersistentData() {
    // TODO: JSON laden aus data/save.json
    std::cout << "[MetaHorror] Loading persistent data..." << std::endl;
}

void MetaHorror::savePersistentData() {
    // TODO: JSON speichern in data/save.json
    std::cout << "[MetaHorror] Saving persistent data..." << std::endl;
}
