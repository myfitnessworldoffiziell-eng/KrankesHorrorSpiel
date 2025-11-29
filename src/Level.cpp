#include "Level.h"
#include "Enemy.h"
#include "AudioManager.h"

Level::Level()
    : m_levelNumber(0)
{
    // Default background: Bright blue (friendly)
    m_backgroundColor[0] = 135.0f;
    m_backgroundColor[1] = 206.0f;
    m_backgroundColor[2] = 235.0f;

    // Basic platforms
    m_platforms.push_back({0.0f, 550.0f, 800.0f, 50.0f});      // Ground
    m_platforms.push_back({200.0f, 450.0f, 150.0f, 20.0f});    // Platform 1
    m_platforms.push_back({450.0f, 350.0f, 150.0f, 20.0f});    // Platform 2
    m_platforms.push_back({100.0f, 250.0f, 100.0f, 20.0f});    // Platform 3
}

Level::~Level() {
}

void Level::update(float deltaTime) {
    // Update all enemies
    for (auto& enemy : m_enemies) {
        if (enemy) {
            enemy->update(deltaTime);
        }
    }

    // Remove dead enemies
    m_enemies.erase(
        std::remove_if(m_enemies.begin(), m_enemies.end(),
            [](const std::unique_ptr<Enemy>& e) { return e && !e->isAlive(); }),
        m_enemies.end()
    );
}

void Level::render(SDL_Renderer* renderer) {
    // Background
    SDL_SetRenderDrawColor(renderer,
        static_cast<int>(m_backgroundColor[0]),
        static_cast<int>(m_backgroundColor[1]),
        static_cast<int>(m_backgroundColor[2]),
        255);
    SDL_RenderClear(renderer);

    // Platforms
    SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255); // Green grass platforms
    for (const auto& platform : m_platforms) {
        SDL_Rect rect;
        rect.x = static_cast<int>(platform.x);
        rect.y = static_cast<int>(platform.y);
        rect.w = static_cast<int>(platform.width);
        rect.h = static_cast<int>(platform.height);

        SDL_RenderFillRect(renderer, &rect);

        // Outline
        SDL_SetRenderDrawColor(renderer, 50, 100, 50, 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
    }

    // Stars (collectibles)
    for (const auto& star : m_stars) {
        if (!star.collected) {
            // Draw star as yellow diamond
            SDL_SetRenderDrawColor(renderer, 255, 220, 0, 255);

            int centerX = static_cast<int>(star.x);
            int centerY = static_cast<int>(star.y);
            int size = 12;

            // Simple star shape (4 points)
            SDL_Point points[5] = {
                {centerX, centerY - size},      // Top
                {centerX + size, centerY},      // Right
                {centerX, centerY + size},      // Bottom
                {centerX - size, centerY},      // Left
                {centerX, centerY - size}       // Back to top
            };
            SDL_RenderDrawLines(renderer, points, 5);

            // Fill center
            SDL_Rect starRect = {centerX - 4, centerY - 4, 8, 8};
            SDL_RenderFillRect(renderer, &starRect);
        }
    }

    // Enemies
    for (const auto& enemy : m_enemies) {
        if (enemy) {
            enemy->render(renderer);
        }
    }
}

void Level::loadLevel(int levelNumber, AudioManager* audioManager) {
    m_levelNumber = levelNumber;

    // Clear existing data
    m_platforms.clear();
    m_enemies.clear();
    m_stars.clear();

    // Load specific level
    switch (levelNumber) {
        case 1:
            createLevel1(audioManager);
            break;
        case 2:
            createLevel2(audioManager);
            break;
        case 3:
            createLevel3(audioManager);
            break;
        default:
            createLevel1(audioManager); // Default to level 1
            break;
    }
}

void Level::createLevel1(AudioManager* audioManager) {
    // LEVEL 1: "Welcome to Paradise"
    // Bright, happy, tutorial level with simple enemies

    m_backgroundColor[0] = 135.0f;  // Sky blue
    m_backgroundColor[1] = 206.0f;
    m_backgroundColor[2] = 250.0f;

    // Platforms (gentle introduction)
    m_platforms.push_back({0.0f, 550.0f, 800.0f, 50.0f});       // Ground
    m_platforms.push_back({150.0f, 450.0f, 120.0f, 20.0f});     // Platform 1
    m_platforms.push_back({350.0f, 380.0f, 100.0f, 20.0f});     // Platform 2
    m_platforms.push_back({520.0f, 450.0f, 120.0f, 20.0f});     // Platform 3
    m_platforms.push_back({650.0f, 350.0f, 140.0f, 20.0f});     // Platform 4 (high)

    // Enemies: 2 Green Slimes, 1 Blue Bird
    m_enemies.push_back(std::make_unique<GreenSlime>(300.0f, 518.0f, audioManager));  // Ground slime 1
    m_enemies.push_back(std::make_unique<GreenSlime>(600.0f, 518.0f, audioManager));  // Ground slime 2
    m_enemies.push_back(std::make_unique<BlueBird>(400.0f, 250.0f, 200.0f, 600.0f, audioManager));  // Flying bird

    // Collectible Stars (5 stars)
    m_stars.push_back({200.0f, 420.0f, false});   // On platform 1
    m_stars.push_back({400.0f, 320.0f, false});   // On platform 2
    m_stars.push_back({570.0f, 420.0f, false});   // On platform 3
    m_stars.push_back({720.0f, 320.0f, false});   // On platform 4
    m_stars.push_back({450.0f, 200.0f, false});   // High in the air (need to jump)
}

void Level::createLevel2(AudioManager* audioManager) {
    // LEVEL 2: "Sunny Meadows"
    // Still happy but slightly more challenging

    m_backgroundColor[0] = 140.0f;
    m_backgroundColor[1] = 210.0f;
    m_backgroundColor[2] = 255.0f;

    // More complex platforming
    m_platforms.push_back({0.0f, 550.0f, 800.0f, 50.0f});       // Ground
    m_platforms.push_back({100.0f, 450.0f, 80.0f, 20.0f});
    m_platforms.push_back({250.0f, 380.0f, 80.0f, 20.0f});
    m_platforms.push_back({400.0f, 450.0f, 80.0f, 20.0f});
    m_platforms.push_back({550.0f, 350.0f, 80.0f, 20.0f});
    m_platforms.push_back({680.0f, 450.0f, 100.0f, 20.0f});

    // More enemies: 3 slimes, 2 birds
    m_enemies.push_back(std::make_unique<GreenSlime>(200.0f, 518.0f, audioManager));
    m_enemies.push_back(std::make_unique<GreenSlime>(450.0f, 518.0f, audioManager));
    m_enemies.push_back(std::make_unique<GreenSlime>(650.0f, 518.0f, audioManager));
    m_enemies.push_back(std::make_unique<BlueBird>(300.0f, 200.0f, 150.0f, 450.0f, audioManager));
    m_enemies.push_back(std::make_unique<BlueBird>(600.0f, 280.0f, 450.0f, 750.0f, audioManager));

    // 6 stars
    m_stars.push_back({140.0f, 420.0f, false});
    m_stars.push_back({290.0f, 350.0f, false});
    m_stars.push_back({440.0f, 420.0f, false});
    m_stars.push_back({590.0f, 320.0f, false});
    m_stars.push_back({730.0f, 420.0f, false});
    m_stars.push_back({400.0f, 250.0f, false});
}

void Level::createLevel3(AudioManager* audioManager) {
    // LEVEL 3: "Glitched Meadows"
    // First corruption - Glitch Slime appears!

    m_backgroundColor[0] = 120.0f;  // Slightly darker sky
    m_backgroundColor[1] = 180.0f;
    m_backgroundColor[2] = 230.0f;

    // Platforms
    m_platforms.push_back({0.0f, 550.0f, 800.0f, 50.0f});
    m_platforms.push_back({120.0f, 420.0f, 100.0f, 20.0f});
    m_platforms.push_back({300.0f, 350.0f, 80.0f, 20.0f});
    m_platforms.push_back({480.0f, 420.0f, 100.0f, 20.0f});
    m_platforms.push_back({650.0f, 320.0f, 120.0f, 20.0f});

    // Enemies: Mix of normal + 1 Glitch Slime (first corruption!)
    m_enemies.push_back(std::make_unique<GreenSlime>(250.0f, 518.0f, audioManager));
    m_enemies.push_back(std::make_unique<GreenSlime>(550.0f, 518.0f, audioManager));
    m_enemies.push_back(std::make_unique<BlueBird>(400.0f, 240.0f, 200.0f, 600.0f, audioManager));
    m_enemies.push_back(std::make_unique<GlitchSlime>(400.0f, 300.0f, audioManager));  // CORRUPTED!

    // 5 stars
    m_stars.push_back({170.0f, 390.0f, false});
    m_stars.push_back({340.0f, 320.0f, false});
    m_stars.push_back({530.0f, 390.0f, false});
    m_stars.push_back({710.0f, 290.0f, false});
    m_stars.push_back({400.0f, 180.0f, false});
}
