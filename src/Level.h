#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <memory>

class Enemy;
class AudioManager;

/**
 * Level
 * Contains platforms, enemies, collectibles, and level data
 */
class Level {
public:
    Level();
    ~Level();

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    // Level management
    void loadLevel(int levelNumber, AudioManager* audioManager);
    int getLevelNumber() const { return m_levelNumber; }

    // Enemy access
    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const { return m_enemies; }
    std::vector<std::unique_ptr<Enemy>>& getEnemies() { return m_enemies; }

    // Collectibles
    struct Star {
        float x, y;
        bool collected;
    };
    const std::vector<Star>& getStars() const { return m_stars; }
    std::vector<Star>& getStars() { return m_stars; }

private:
    struct Platform {
        float x, y, width, height;
    };

    int m_levelNumber;
    std::vector<Platform> m_platforms;
    std::vector<std::unique_ptr<Enemy>> m_enemies;
    std::vector<Star> m_stars;
    float m_backgroundColor[3]; // RGB

    // Level creation helpers
    void createLevel1(AudioManager* audioManager);  // Welcome to Paradise
    void createLevel2(AudioManager* audioManager);  // Sunny Meadows
    void createLevel3(AudioManager* audioManager);  // Glitched Meadows
};
