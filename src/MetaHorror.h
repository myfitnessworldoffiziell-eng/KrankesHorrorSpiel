#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <ctime>

/**
 * MetaHorror-System
 * Verantwortlich für alle "meta" Horror-Effekte:
 * - Datei-Erstellung (Downloads, Desktop)
 * - Fenster-Manipulation
 * - System-Info Auslesen
 * - Persistente Daten
 */
class MetaHorror {
public:
    MetaHorror();
    ~MetaHorror();

    // Initialisierung (erstellt erste versteckte Dateien)
    void initialize();

    // Update (zufällige Events basierend auf corruption level)
    void update(float deltaTime, int corruptionLevel);

    // Beim Spiel-Exit
    void onGameExit();

    // Manuelle Trigger
    void createCreepyFile();
    void createFile(const std::string& name, const std::string& content); // General file creation
    void manipulateWindow(SDL_Window* window);

private:
    // Datei-Operationen
    void createDownloadsFile(const std::string& filename, const std::string& content);
    void createDesktopFile(const std::string& filename, const std::string& content);
    std::string getDownloadsPath();
    std::string getDesktopPath();
    std::string getUserName();

    // Persistenz
    void loadPersistentData();
    void savePersistentData();

    // Horror Content Generation
    std::string generateCreepyMessage();
    std::string generateCorruptedText();

    // State
    bool m_initialized;
    int m_filesCreated;
    float m_timeSinceLastEvent;

    // Persistent Data
    int m_totalPlaytime;      // in Sekunden
    int m_totalDeaths;
    time_t m_lastPlayedTime;
    std::vector<std::string> m_playerActions; // Was der Spieler getan hat
};
