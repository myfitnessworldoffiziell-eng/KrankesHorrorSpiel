#pragma once

/**
 * Game State Enum
 * Definiert alle möglichen Zustände des Spiels
 */
enum class GameState {
    INTRO,          // Disclaimer & Logo
    MAIN_MENU,      // Hauptmenü
    PLAYING,        // Aktives Gameplay
    PAUSED,         // Pause-Menü
    DIALOG,         // Dialog/Cutscene läuft
    FAKE_CRASH,     // Fake Blue Screen
    BOSS_HORROR,    // Post-boss horror sequence
    GAME_OVER,      // Tod-Screen
    ENDING          // Endings
};
