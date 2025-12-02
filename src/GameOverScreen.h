#pragma once
#include "SDL_Platform.h"

#include <string>
#include <vector>
#include "AudioManager.h"

/**
 * Game Over Screen with META-HORROR Elements
 *
 * Not a normal game over screen - Echo taunts the player,
 * shows statistics, and hints that respawning has consequences.
 */
class GameOverScreen {
public:
    GameOverScreen(AudioManager* audioManager);

    // Trigger the game over screen
    void trigger(int deathCount, int currentLevel);

    // Update animation/effects
    void update(float deltaTime);

    // Render the screen
    void render(SDL_Renderer* renderer);

    // Check if active
    bool isActive() const { return m_active; }

    // Reset for next death
    void reset();

private:
    AudioManager* m_audioManager;

    bool m_active;
    float m_timer;
    int m_deathCount;
    int m_currentLevel;

    // Horror messages that change based on death count
    std::vector<std::string> m_horrorMessages;

    // Generate message based on death count
    void generateHorrorMessage();

    // Static counter for total deaths across all sessions
    static int s_totalDeaths;
};
