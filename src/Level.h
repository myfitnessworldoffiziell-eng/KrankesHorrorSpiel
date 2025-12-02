#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <string>

class Enemy;
class NPC;
class Boss;
class AudioManager;
class Player;

/**
 * Level
 * Contains platforms, enemies, NPCs, bosses, collectibles, and level data
 */
class Level {
public:
    Level();
    ~Level();

    void update(float deltaTime, Player* player = nullptr);
    void render(SDL_Renderer* renderer);

    // Level management
    void loadLevel(int levelNumber, AudioManager* audioManager);
    int getLevelNumber() const { return m_levelNumber; }
    bool isComplete() const { return m_levelComplete; }
    void setComplete(bool complete) { m_levelComplete = complete; }

    // Enemy access
    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const { return m_enemies; }
    std::vector<std::unique_ptr<Enemy>>& getEnemies() { return m_enemies; }

    // NPC access
    const std::vector<std::unique_ptr<NPC>>& getNPCs() const { return m_npcs; }
    std::vector<std::unique_ptr<NPC>>& getNPCs() { return m_npcs; }

    // Boss access
    Boss* getBoss() const { return m_boss.get(); }
    bool hasBoss() const { return m_boss != nullptr; }

    // Collectibles
    struct Star {
        float x, y;
        bool collected;
    };
    const std::vector<Star>& getStars() const { return m_stars; }
    std::vector<Star>& getStars() { return m_stars; }

    // Code Fragments (for true ending)
    struct CodeFragment {
        float x, y;
        std::string code;  // Part of the code: "F I R", "S T", "E N D"
        bool collected;

        CodeFragment(float _x, float _y, std::string _code, bool _collected)
            : x(_x), y(_y), code(_code), collected(_collected) {}
    };
    const std::vector<CodeFragment>& getCodeFragments() const { return m_codeFragments; }
    std::vector<CodeFragment>& getCodeFragments() { return m_codeFragments; }

    // Level Goal (exit portal)
    struct Goal {
        float x, y;
        float width, height;
        bool active;
    };
    const Goal& getGoal() const { return m_goal; }
    void activateGoal() { m_goal.active = true; }

private:
    struct Platform {
        float x, y, width, height;
    };

    int m_levelNumber;
    bool m_levelComplete;
    std::vector<Platform> m_platforms;
    std::vector<std::unique_ptr<Enemy>> m_enemies;
    std::vector<std::unique_ptr<NPC>> m_npcs;
    std::unique_ptr<Boss> m_boss;
    std::vector<Star> m_stars;
    std::vector<CodeFragment> m_codeFragments;
    Goal m_goal;
    float m_backgroundColor[3]; // RGB

    // Level creation helpers
    void createLevel1(AudioManager* audioManager);  // Welcome to Paradise
    void createLevel2(AudioManager* audioManager);  // Sunny Meadows
    void createLevel3(AudioManager* audioManager);  // Glitched Meadows
    void createLevel4(AudioManager* audioManager);  // Echoes of the Past (Boss: Mirror Echo)
    void createLevel5(AudioManager* audioManager);  // Corrupted City
    void createLevel6(AudioManager* audioManager);  // The Glitch Arena (Boss Fight)
    void createLevel7(AudioManager* audioManager);  // Developer Room
    void createLevel8(AudioManager* audioManager);  // Memory Fragments
    void createLevel9(AudioManager* audioManager);  // Level 9 Loop
    void createLevel10(AudioManager* audioManager); // ECHO.exe Final Boss
};
