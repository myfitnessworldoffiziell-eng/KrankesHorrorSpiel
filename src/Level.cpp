#include "Level.h"
#include "Enemy.h"
#include "NPC.h"
#include "Boss.h"
#include "GlitchBoss.h"
#include "EchoPrime.h"
#include "Player.h"
#include "AudioManager.h"
#include <algorithm>

Level::Level()
    : m_levelNumber(0)
    , m_levelComplete(false)
{
    // Default background: Bright blue (friendly)
    m_backgroundColor[0] = 135.0f;
    m_backgroundColor[1] = 206.0f;
    m_backgroundColor[2] = 235.0f;

    // Initialize goal (inactive by default)
    m_goal = {750.0f, 480.0f, 40.0f, 60.0f, false};

    // Basic platforms
    m_platforms.push_back({0.0f, 550.0f, 800.0f, 50.0f});      // Ground
    m_platforms.push_back({200.0f, 450.0f, 150.0f, 20.0f});    // Platform 1
    m_platforms.push_back({450.0f, 350.0f, 150.0f, 20.0f});    // Platform 2
    m_platforms.push_back({100.0f, 250.0f, 100.0f, 20.0f});    // Platform 3
}

Level::~Level() {
}

void Level::update(float deltaTime, Player* player) {
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

    // Update all NPCs
    for (auto& npc : m_npcs) {
        if (npc) {
            npc->update(deltaTime);
        }
    }

    // Update boss if present
    if (m_boss && m_boss->isAlive() && player) {
        m_boss->update(deltaTime, player);
    }

    // Check if all stars collected (activate goal)
    if (!m_goal.active) {
        bool allStarsCollected = true;
        for (const auto& star : m_stars) {
            if (!star.collected) {
                allStarsCollected = false;
                break;
            }
        }
        // Also check boss defeated if there is one
        if (m_boss && !m_boss->isDefeated()) {
            allStarsCollected = false;
        }

        if (allStarsCollected && m_stars.size() > 0) {
            activateGoal();
        }
    }
}

void Level::render(SDL_Renderer* renderer) {
    // Background
    SDL_SetRenderDrawColor(renderer,
        static_cast<int>(m_backgroundColor[0]),
        static_cast<int>(m_backgroundColor[1]),
        static_cast<int>(m_backgroundColor[2]),
        255);
    SDL_RenderClear(renderer);

    // Background Decorations (rendered before platforms for depth)
    for (const auto& deco : m_decorations) {
        SDL_Rect rect = {
            static_cast<int>(deco.x),
            static_cast<int>(deco.y),
            static_cast<int>(deco.width),
            static_cast<int>(deco.height)
        };

        // Color based on decoration type and variant
        switch (deco.type) {
            case DecorationType::TREE:
                // Brown trunk + green foliage
                SDL_SetRenderDrawColor(renderer, 101, 67, 33, 255);
                SDL_Rect trunk = {rect.x + rect.w/2 - 5, rect.y + rect.h/2, 10, rect.h/2};
                SDL_RenderFillRect(renderer, &trunk);
                SDL_SetRenderDrawColor(renderer, 34 + deco.colorVariant*20, 139 + deco.colorVariant*10, 34, 255);
                SDL_Rect foliage = {rect.x, rect.y, rect.w, rect.h/2};
                SDL_RenderFillRect(renderer, &foliage);
                break;

            case DecorationType::ROCK:
                SDL_SetRenderDrawColor(renderer, 105 + deco.colorVariant*15, 105 + deco.colorVariant*15, 105 + deco.colorVariant*15, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
                SDL_RenderDrawRect(renderer, &rect);
                break;

            case DecorationType::BUSH:
                SDL_SetRenderDrawColor(renderer, 60 + deco.colorVariant*10, 179 - deco.colorVariant*10, 113, 255);
                SDL_RenderFillRect(renderer, &rect);
                break;

            case DecorationType::CLOUD:
                SDL_SetRenderDrawColor(renderer, 240 - deco.colorVariant*10, 240 - deco.colorVariant*10, 240 - deco.colorVariant*10, 180);
                SDL_RenderFillRect(renderer, &rect);
                break;

            case DecorationType::GRASS_TUFT:
                SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255);
                for (int i = 0; i < 3; i++) {
                    SDL_RenderDrawLine(renderer, rect.x + i*3, rect.y + rect.h, rect.x + i*3, rect.y);
                }
                break;

            case DecorationType::FLOWER:
                // Stem
                SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255);
                SDL_RenderDrawLine(renderer, rect.x + rect.w/2, rect.y + rect.h, rect.x + rect.w/2, rect.y + 5);
                // Bloom
                SDL_SetRenderDrawColor(renderer, 255 - deco.colorVariant*50, deco.colorVariant*100, deco.colorVariant*80, 255);
                SDL_Rect bloom = {rect.x, rect.y, rect.w, 5};
                SDL_RenderFillRect(renderer, &bloom);
                break;

            case DecorationType::STONE_PILE:
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
                SDL_RenderDrawRect(renderer, &rect);
                break;

            case DecorationType::FENCE:
                SDL_SetRenderDrawColor(renderer, 139, 90, 43, 255);
                // Vertical posts
                for (int i = 0; i < rect.w; i += 20) {
                    SDL_RenderDrawLine(renderer, rect.x + i, rect.y, rect.x + i, rect.y + rect.h);
                }
                // Horizontal bars
                SDL_RenderDrawLine(renderer, rect.x, rect.y + 5, rect.x + rect.w, rect.y + 5);
                SDL_RenderDrawLine(renderer, rect.x, rect.y + rect.h - 5, rect.x + rect.w, rect.y + rect.h - 5);
                break;

            case DecorationType::SIGN_POST:
                // Post
                SDL_SetRenderDrawColor(renderer, 101, 67, 33, 255);
                SDL_Rect post = {rect.x + rect.w/2 - 3, rect.y + 10, 6, rect.h - 10};
                SDL_RenderFillRect(renderer, &post);
                // Sign
                SDL_SetRenderDrawColor(renderer, 210, 180, 140, 255);
                SDL_Rect sign = {rect.x, rect.y, rect.w, 10};
                SDL_RenderFillRect(renderer, &sign);
                SDL_SetRenderDrawColor(renderer, 101, 67, 33, 255);
                SDL_RenderDrawRect(renderer, &sign);
                break;
        }
    }

    // Platforms (with material types)
    for (const auto& platform : m_platforms) {
        SDL_Rect rect;
        rect.x = static_cast<int>(platform.x);
        rect.y = static_cast<int>(platform.y);
        rect.w = static_cast<int>(platform.width);
        rect.h = static_cast<int>(platform.height);

        // Color based on material type
        switch (platform.materialType) {
            case 0: // Grass
                SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
                break;
            case 1: // Stone
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
                break;
            case 2: // Metal
                SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);
                break;
            case 3: // Wood
                SDL_SetRenderDrawColor(renderer, 139, 90, 43, 255);
                break;
            default:
                SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
        }
        SDL_RenderFillRect(renderer, &rect);

        // Outline (darker version of fill color)
        switch (platform.materialType) {
            case 0: // Grass outline
                SDL_SetRenderDrawColor(renderer, 50, 100, 50, 255);
                break;
            case 1: // Stone outline
                SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
                break;
            case 2: // Metal outline
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                break;
            case 3: // Wood outline
                SDL_SetRenderDrawColor(renderer, 90, 60, 30, 255);
                break;
            default:
                SDL_SetRenderDrawColor(renderer, 50, 100, 50, 255);
        }
        SDL_RenderDrawRect(renderer, &rect);
    }

    // Goal (exit portal) - only if active
    if (m_goal.active) {
        SDL_Rect goalRect = {
            static_cast<int>(m_goal.x),
            static_cast<int>(m_goal.y),
            static_cast<int>(m_goal.width),
            static_cast<int>(m_goal.height)
        };

        // Pulsing effect
        static float pulseTimer = 0.0f;
        pulseTimer += 0.016f;
        int pulse = static_cast<int>(std::sin(pulseTimer * 3.0f) * 30.0f + 225.0f);

        // Purple portal
        SDL_SetRenderDrawColor(renderer, pulse, 0, pulse, 200);
        SDL_RenderFillRect(renderer, &goalRect);

        // Border
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &goalRect);
    }

    // Stars (collectibles)
    for (const auto& star : m_stars) {
        if (!star.collected) {
            SDL_SetRenderDrawColor(renderer, 255, 220, 0, 255);

            int centerX = static_cast<int>(star.x);
            int centerY = static_cast<int>(star.y);
            int size = 12;

            SDL_Point points[5] = {
                {centerX, centerY - size},      // Top
                {centerX + size, centerY},      // Right
                {centerX, centerY + size},      // Bottom
                {centerX - size, centerY},      // Left
                {centerX, centerY - size}       // Back to top
            };
            SDL_RenderDrawLines(renderer, points, 5);

            SDL_Rect starRect = {centerX - 4, centerY - 4, 8, 8};
            SDL_RenderFillRect(renderer, &starRect);
        }
    }

    // Code Fragments
    for (const auto& fragment : m_codeFragments) {
        if (!fragment.collected) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);

            int x = static_cast<int>(fragment.x);
            int y = static_cast<int>(fragment.y);

            // Glowing cube
            SDL_Rect cubeRect = {x - 8, y - 8, 16, 16};
            SDL_RenderFillRect(renderer, &cubeRect);

            // White border
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &cubeRect);
        }
    }

    // NPCs
    for (const auto& npc : m_npcs) {
        if (npc) {
            npc->render(renderer);
        }
    }

    // Enemies
    for (const auto& enemy : m_enemies) {
        if (enemy) {
            enemy->render(renderer);
        }
    }

    // Boss (render in front of everything)
    if (m_boss && m_boss->isAlive()) {
        m_boss->render(renderer);
    }
}

void Level::loadLevel(int levelNumber, AudioManager* audioManager) {
    m_levelNumber = levelNumber;
    m_levelComplete = false;

    // Clear existing data
    m_platforms.clear();
    m_enemies.clear();
    m_npcs.clear();
    m_boss.reset();
    m_stars.clear();
    m_codeFragments.clear();
    m_goal = {750.0f, 480.0f, 40.0f, 60.0f, false};

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
        case 4:
            createLevel4(audioManager);
            break;
        case 5:
            createLevel5(audioManager);
            break;
        case 6:
            createLevel6(audioManager);
            break;
        case 7:
            createLevel7(audioManager);
            break;
        case 8:
            createLevel8(audioManager);
            break;
        case 9:
            createLevel9(audioManager);
            break;
        case 10:
            createLevel10(audioManager);
            break;
        default:
            createLevel1(audioManager);
            break;
    }
}

// ========================================
// LEVEL 1: Welcome to Paradise (Extended & Beautified)
// ========================================
void Level::createLevel1(AudioManager* audioManager) {
    m_backgroundColor[0] = 135.0f;
    m_backgroundColor[1] = 206.0f;
    m_backgroundColor[2] = 250.0f;

    // Platforms - Extended level with varied materials (type: 0=grass, 1=stone, 2=metal, 3=wood)
    m_platforms.push_back({0.0f, 550.0f, 300.0f, 50.0f, 0});      // Ground - grass
    m_platforms.push_back({300.0f, 550.0f, 200.0f, 50.0f, 1});    // Ground - stone
    m_platforms.push_back({500.0f, 550.0f, 300.0f, 50.0f, 0});    // Ground - grass

    // First section - grass platforms
    m_platforms.push_back({150.0f, 450.0f, 120.0f, 20.0f, 0});    // Grass platform
    m_platforms.push_back({320.0f, 400.0f, 100.0f, 20.0f, 0});    // Grass platform
    m_platforms.push_back({470.0f, 480.0f, 140.0f, 20.0f, 0});    // Grass platform
    m_platforms.push_back({650.0f, 420.0f, 100.0f, 20.0f, 3});    // Wood platform

    // Middle section - stone and wood mix
    m_platforms.push_back({100.0f, 350.0f, 80.0f, 20.0f, 1});     // Stone platform
    m_platforms.push_back({250.0f, 300.0f, 90.0f, 20.0f, 3});     // Wood platform
    m_platforms.push_back({400.0f, 330.0f, 110.0f, 20.0f, 1});    // Stone platform
    m_platforms.push_back({560.0f, 360.0f, 100.0f, 20.0f, 3});    // Wood platform

    // Upper section - challenging jumps
    m_platforms.push_back({180.0f, 230.0f, 70.0f, 15.0f, 2});     // Metal platform
    m_platforms.push_back({320.0f, 200.0f, 80.0f, 15.0f, 2});     // Metal platform
    m_platforms.push_back({480.0f, 250.0f, 90.0f, 15.0f, 3});     // Wood platform
    m_platforms.push_back({640.0f, 280.0f, 120.0f, 20.0f, 1});    // Stone platform

    // Background Decorations - Make it look alive!
    // Sky clouds
    m_decorations.push_back({50.0f, 30.0f, 80.0f, 30.0f, DecorationType::CLOUD, 0});
    m_decorations.push_back({200.0f, 50.0f, 100.0f, 35.0f, DecorationType::CLOUD, 1});
    m_decorations.push_back({450.0f, 40.0f, 90.0f, 32.0f, DecorationType::CLOUD, 0});
    m_decorations.push_back({650.0f, 60.0f, 110.0f, 38.0f, DecorationType::CLOUD, 2});

    // Ground decorations - trees
    m_decorations.push_back({20.0f, 490.0f, 40.0f, 60.0f, DecorationType::TREE, 0});
    m_decorations.push_back({280.0f, 490.0f, 45.0f, 60.0f, DecorationType::TREE, 1});
    m_decorations.push_back({620.0f, 490.0f, 50.0f, 60.0f, DecorationType::TREE, 2});
    m_decorations.push_back({720.0f, 490.0f, 40.0f, 55.0f, DecorationType::TREE, 0});

    // Bushes scattered around
    m_decorations.push_back({100.0f, 530.0f, 30.0f, 20.0f, DecorationType::BUSH, 0});
    m_decorations.push_back({220.0f, 530.0f, 35.0f, 22.0f, DecorationType::BUSH, 1});
    m_decorations.push_back({380.0f, 530.0f, 28.0f, 20.0f, DecorationType::BUSH, 2});
    m_decorations.push_back({540.0f, 530.0f, 32.0f, 20.0f, DecorationType::BUSH, 0});

    // Rocks for variety
    m_decorations.push_back({60.0f, 535.0f, 25.0f, 15.0f, DecorationType::ROCK, 0});
    m_decorations.push_back({340.0f, 535.0f, 30.0f, 18.0f, DecorationType::ROCK, 1});
    m_decorations.push_back({480.0f, 535.0f, 28.0f, 16.0f, DecorationType::ROCK, 2});

    // Flowers for color
    m_decorations.push_back({140.0f, 540.0f, 8.0f, 10.0f, DecorationType::FLOWER, 0});
    m_decorations.push_back({160.0f, 540.0f, 8.0f, 10.0f, DecorationType::FLOWER, 1});
    m_decorations.push_back({180.0f, 540.0f, 8.0f, 10.0f, DecorationType::FLOWER, 2});
    m_decorations.push_back({420.0f, 540.0f, 8.0f, 10.0f, DecorationType::FLOWER, 1});
    m_decorations.push_back({440.0f, 540.0f, 8.0f, 10.0f, DecorationType::FLOWER, 0});

    // Grass tufts for detail
    m_decorations.push_back({80.0f, 540.0f, 10.0f, 10.0f, DecorationType::GRASS_TUFT, 0});
    m_decorations.push_back({200.0f, 540.0f, 10.0f, 10.0f, DecorationType::GRASS_TUFT, 0});
    m_decorations.push_back({320.0f, 540.0f, 10.0f, 10.0f, DecorationType::GRASS_TUFT, 0});
    m_decorations.push_back({500.0f, 540.0f, 10.0f, 10.0f, DecorationType::GRASS_TUFT, 0});
    m_decorations.push_back({600.0f, 540.0f, 10.0f, 10.0f, DecorationType::GRASS_TUFT, 0});

    // Enemies - same as before but adjusted positions
    m_enemies.push_back(std::make_unique<GreenSlime>(300.0f, 518.0f, audioManager));
    m_enemies.push_back(std::make_unique<GreenSlime>(600.0f, 518.0f, audioManager));
    m_enemies.push_back(std::make_unique<BlueBird>(400.0f, 250.0f, 200.0f, 600.0f, audioManager));

    // Tutorial NPC
    auto tutorialNPC = std::make_unique<NPC>(50.0f, 500.0f, NPCType::FRIENDLY, audioManager);
    tutorialNPC->addDialogue("Welcome to ECHOES!");
    tutorialNPC->addDialogue("Use WASD or Arrow Keys to move");
    tutorialNPC->addDialogue("Press SPACE to jump");
    tutorialNPC->addDialogue("Collect all stars to unlock the exit!");
    m_npcs.push_back(std::move(tutorialNPC));

    // Stars - more spread out across extended level
    m_stars.push_back({200.0f, 420.0f, false});     // First section
    m_stars.push_back({360.0f, 350.0f, false});     // Middle low
    m_stars.push_back({520.0f, 450.0f, false});     // Right section
    m_stars.push_back({690.0f, 390.0f, false});     // Upper right
    m_stars.push_back({230.0f, 270.0f, false});     // High left
    m_stars.push_back({370.0f, 170.0f, false});     // Very high center
    m_stars.push_back({530.0f, 220.0f, false});     // High right

    // Goal portal at the end
    m_goal = {730.0f, 450.0f, 40.0f, 60.0f, false};
}

// ========================================
// LEVEL 2: Sunny Meadows (Extended & Beautified)
// ========================================
void Level::createLevel2(AudioManager* audioManager) {
    m_backgroundColor[0] = 140.0f;
    m_backgroundColor[1] = 210.0f;
    m_backgroundColor[2] = 255.0f;

    // Platforms - Meadow theme with more wood and grass
    m_platforms.push_back({0.0f, 550.0f, 250.0f, 50.0f, 0});       // Ground - grass
    m_platforms.push_back({250.0f, 550.0f, 150.0f, 50.0f, 3});     // Ground - wood
    m_platforms.push_back({400.0f, 550.0f, 200.0f, 50.0f, 0});     // Ground - grass
    m_platforms.push_back({600.0f, 550.0f, 200.0f, 50.0f, 3});     // Ground - wood

    // Lower platforms - zigzag pattern
    m_platforms.push_back({100.0f, 470.0f, 90.0f, 18.0f, 0});      // Grass
    m_platforms.push_back({250.0f, 420.0f, 95.0f, 18.0f, 3});      // Wood
    m_platforms.push_back({400.0f, 460.0f, 85.0f, 18.0f, 0});      // Grass
    m_platforms.push_back({540.0f, 400.0f, 100.0f, 18.0f, 3});     // Wood
    m_platforms.push_back({690.0f, 460.0f, 90.0f, 18.0f, 0});      // Grass

    // Middle platforms - more challenging
    m_platforms.push_back({80.0f, 350.0f, 80.0f, 18.0f, 1});       // Stone
    m_platforms.push_back({210.0f, 310.0f, 85.0f, 18.0f, 3});      // Wood
    m_platforms.push_back({350.0f, 350.0f, 90.0f, 18.0f, 0});      // Grass
    m_platforms.push_back({490.0f, 320.0f, 95.0f, 18.0f, 3});      // Wood
    m_platforms.push_back({630.0f, 370.0f, 85.0f, 18.0f, 1});      // Stone

    // Upper platforms - high jumps
    m_platforms.push_back({140.0f, 230.0f, 75.0f, 15.0f, 2});      // Metal
    m_platforms.push_back({280.0f, 210.0f, 80.0f, 15.0f, 3});      // Wood
    m_platforms.push_back({430.0f, 250.0f, 70.0f, 15.0f, 2});      // Metal
    m_platforms.push_back({570.0f, 220.0f, 85.0f, 15.0f, 3});      // Wood
    m_platforms.push_back({700.0f, 270.0f, 75.0f, 15.0f, 1});      // Stone

    // Background Decorations - Sunny meadow atmosphere
    // Clouds
    m_decorations.push_back({30.0f, 25.0f, 90.0f, 35.0f, DecorationType::CLOUD, 0});
    m_decorations.push_back({180.0f, 45.0f, 110.0f, 38.0f, DecorationType::CLOUD, 1});
    m_decorations.push_back({380.0f, 35.0f, 95.0f, 33.0f, DecorationType::CLOUD, 0});
    m_decorations.push_back({550.0f, 55.0f, 105.0f, 36.0f, DecorationType::CLOUD, 2});
    m_decorations.push_back({710.0f, 40.0f, 85.0f, 32.0f, DecorationType::CLOUD, 1});

    // Trees - more varied
    m_decorations.push_back({15.0f, 485.0f, 45.0f, 65.0f, DecorationType::TREE, 0});
    m_decorations.push_back({190.0f, 480.0f, 50.0f, 70.0f, DecorationType::TREE, 1});
    m_decorations.push_back({350.0f, 485.0f, 42.0f, 65.0f, DecorationType::TREE, 2});
    m_decorations.push_back({510.0f, 480.0f, 48.0f, 70.0f, DecorationType::TREE, 0});
    m_decorations.push_back({660.0f, 485.0f, 46.0f, 68.0f, DecorationType::TREE, 1});
    m_decorations.push_back({750.0f, 485.0f, 40.0f, 65.0f, DecorationType::TREE, 2});

    // Bushes - scattered meadow bushes
    m_decorations.push_back({70.0f, 528.0f, 32.0f, 22.0f, DecorationType::BUSH, 0});
    m_decorations.push_back({140.0f, 528.0f, 35.0f, 22.0f, DecorationType::BUSH, 1});
    m_decorations.push_back({250.0f, 528.0f, 30.0f, 22.0f, DecorationType::BUSH, 2});
    m_decorations.push_back({330.0f, 528.0f, 33.0f, 22.0f, DecorationType::BUSH, 0});
    m_decorations.push_back({460.0f, 528.0f, 36.0f, 22.0f, DecorationType::BUSH, 1});
    m_decorations.push_back({580.0f, 528.0f, 32.0f, 22.0f, DecorationType::BUSH, 2});
    m_decorations.push_back({710.0f, 528.0f, 34.0f, 22.0f, DecorationType::BUSH, 0});

    // Rocks
    m_decorations.push_back({110.0f, 535.0f, 28.0f, 15.0f, DecorationType::ROCK, 0});
    m_decorations.push_back({290.0f, 535.0f, 32.0f, 18.0f, DecorationType::ROCK, 1});
    m_decorations.push_back({420.0f, 535.0f, 26.0f, 16.0f, DecorationType::ROCK, 2});
    m_decorations.push_back({620.0f, 535.0f, 30.0f, 17.0f, DecorationType::ROCK, 0});

    // Flowers - colorful meadow flowers
    m_decorations.push_back({95.0f, 538.0f, 8.0f, 12.0f, DecorationType::FLOWER, 0});
    m_decorations.push_back({115.0f, 538.0f, 8.0f, 12.0f, DecorationType::FLOWER, 1});
    m_decorations.push_back({135.0f, 538.0f, 8.0f, 12.0f, DecorationType::FLOWER, 2});
    m_decorations.push_back({220.0f, 538.0f, 8.0f, 12.0f, DecorationType::FLOWER, 0});
    m_decorations.push_back({240.0f, 538.0f, 8.0f, 12.0f, DecorationType::FLOWER, 1});
    m_decorations.push_back({380.0f, 538.0f, 8.0f, 12.0f, DecorationType::FLOWER, 2});
    m_decorations.push_back({400.0f, 538.0f, 8.0f, 12.0f, DecorationType::FLOWER, 0});
    m_decorations.push_back({550.0f, 538.0f, 8.0f, 12.0f, DecorationType::FLOWER, 1});
    m_decorations.push_back({570.0f, 538.0f, 8.0f, 12.0f, DecorationType::FLOWER, 2});

    // Grass tufts
    m_decorations.push_back({50.0f, 540.0f, 10.0f, 10.0f, DecorationType::GRASS_TUFT, 0});
    m_decorations.push_back({170.0f, 540.0f, 10.0f, 10.0f, DecorationType::GRASS_TUFT, 0});
    m_decorations.push_back({270.0f, 540.0f, 10.0f, 10.0f, DecorationType::GRASS_TUFT, 0});
    m_decorations.push_back({370.0f, 540.0f, 10.0f, 10.0f, DecorationType::GRASS_TUFT, 0});
    m_decorations.push_back({480.0f, 540.0f, 10.0f, 10.0f, DecorationType::GRASS_TUFT, 0});
    m_decorations.push_back({600.0f, 540.0f, 10.0f, 10.0f, DecorationType::GRASS_TUFT, 0});
    m_decorations.push_back({690.0f, 540.0f, 10.0f, 10.0f, DecorationType::GRASS_TUFT, 0});

    // Fences for meadow feel
    m_decorations.push_back({440.0f, 525.0f, 60.0f, 25.0f, DecorationType::FENCE, 0});

    // Enemies - same as before with adjusted positions
    m_enemies.push_back(std::make_unique<GreenSlime>(200.0f, 518.0f, audioManager));
    m_enemies.push_back(std::make_unique<GreenSlime>(450.0f, 518.0f, audioManager));
    m_enemies.push_back(std::make_unique<GreenSlime>(650.0f, 518.0f, audioManager));
    m_enemies.push_back(std::make_unique<BlueBird>(300.0f, 200.0f, 150.0f, 450.0f, audioManager));
    m_enemies.push_back(std::make_unique<BlueBird>(600.0f, 280.0f, 450.0f, 750.0f, audioManager));

    // Cheerful friend NPC
    auto friendNPC = std::make_unique<NPC>(50.0f, 500.0f, NPCType::FRIENDLY, audioManager);
    friendNPC->addDialogue("You're doing great!");
    friendNPC->addDialogue("Keep going, friend!");
    m_npcs.push_back(std::move(friendNPC));

    // Stars - spread across extended level
    m_stars.push_back({140.0f, 440.0f, false});      // Lower left
    m_stars.push_back({290.0f, 390.0f, false});      // Middle left
    m_stars.push_back({440.0f, 430.0f, false});      // Center
    m_stars.push_back({590.0f, 370.0f, false});      // Middle right
    m_stars.push_back({730.0f, 430.0f, false});      // Lower right
    m_stars.push_back({230.0f, 280.0f, false});      // High left
    m_stars.push_back({380.0f, 180.0f, false});      // Very high
    m_stars.push_back({620.0f, 190.0f, false});      // High right

    // Goal portal
    m_goal = {740.0f, 440.0f, 40.0f, 60.0f, false};
}

// ========================================
// LEVEL 3: Glitched Meadows (Extended & Corrupted)
// ========================================
void Level::createLevel3(AudioManager* audioManager) {
    m_backgroundColor[0] = 120.0f;  // Darker, glitchy sky
    m_backgroundColor[1] = 180.0f;
    m_backgroundColor[2] = 230.0f;

    // Platforms - Mixed materials showing corruption (more metal/stone, less grass)
    m_platforms.push_back({0.0f, 550.0f, 200.0f, 50.0f, 0});       // Ground - grass (fading)
    m_platforms.push_back({200.0f, 550.0f, 180.0f, 50.0f, 2});     // Ground - metal (glitched)
    m_platforms.push_back({380.0f, 550.0f, 220.0f, 50.0f, 1});     // Ground - stone (corrupted)
    m_platforms.push_back({600.0f, 550.0f, 200.0f, 50.0f, 2});     // Ground - metal

    // Lower platforms - irregular pattern showing glitch
    m_platforms.push_back({120.0f, 460.0f, 95.0f, 18.0f, 1});      // Stone
    m_platforms.push_back({270.0f, 410.0f, 90.0f, 18.0f, 2});      // Metal (glitchy)
    m_platforms.push_back({420.0f, 470.0f, 100.0f, 18.0f, 1});     // Stone
    m_platforms.push_back({570.0f, 430.0f, 95.0f, 18.0f, 2});      // Metal
    m_platforms.push_back({710.0f, 480.0f, 85.0f, 18.0f, 1});      // Stone

    // Middle platforms - more corruption
    m_platforms.push_back({90.0f, 360.0f, 85.0f, 18.0f, 2});       // Metal
    m_platforms.push_back({230.0f, 330.0f, 80.0f, 18.0f, 1});      // Stone
    m_platforms.push_back({370.0f, 380.0f, 90.0f, 18.0f, 2});      // Metal
    m_platforms.push_back({510.0f, 340.0f, 85.0f, 18.0f, 1});      // Stone
    m_platforms.push_back({650.0f, 390.0f, 90.0f, 18.0f, 2});      // Metal

    // Upper platforms - high difficulty
    m_platforms.push_back({150.0f, 250.0f, 70.0f, 15.0f, 2});      // Metal
    m_platforms.push_back({290.0f, 220.0f, 75.0f, 15.0f, 2});      // Metal
    m_platforms.push_back({440.0f, 260.0f, 80.0f, 15.0f, 1});      // Stone
    m_platforms.push_back({590.0f, 230.0f, 75.0f, 15.0f, 2});      // Metal
    m_platforms.push_back({720.0f, 290.0f, 70.0f, 15.0f, 1});      // Stone

    // Background Decorations - Corrupted/glitchy theme
    // Dark, ominous clouds
    m_decorations.push_back({40.0f, 30.0f, 100.0f, 40.0f, DecorationType::CLOUD, 2});  // Dark cloud
    m_decorations.push_back({190.0f, 50.0f, 120.0f, 42.0f, DecorationType::CLOUD, 2});
    m_decorations.push_back({400.0f, 35.0f, 95.0f, 38.0f, DecorationType::CLOUD, 2});
    m_decorations.push_back({600.0f, 55.0f, 110.0f, 40.0f, DecorationType::CLOUD, 2});

    // Dead/corrupted trees
    m_decorations.push_back({25.0f, 490.0f, 40.0f, 60.0f, DecorationType::TREE, 0});
    m_decorations.push_back({160.0f, 490.0f, 45.0f, 60.0f, DecorationType::TREE, 1});
    m_decorations.push_back({320.0f, 490.0f, 42.0f, 58.0f, DecorationType::TREE, 2});
    m_decorations.push_back({480.0f, 490.0f, 40.0f, 60.0f, DecorationType::TREE, 0});
    m_decorations.push_back({640.0f, 490.0f, 43.0f, 58.0f, DecorationType::TREE, 1});

    // Stone piles (broken structures)
    m_decorations.push_back({80.0f, 530.0f, 35.0f, 20.0f, DecorationType::STONE_PILE, 0});
    m_decorations.push_back({240.0f, 530.0f, 38.0f, 22.0f, DecorationType::STONE_PILE, 1});
    m_decorations.push_back({400.0f, 530.0f, 40.0f, 24.0f, DecorationType::STONE_PILE, 2});
    m_decorations.push_back({560.0f, 530.0f, 36.0f, 20.0f, DecorationType::STONE_PILE, 0});
    m_decorations.push_back({700.0f, 530.0f, 38.0f, 22.0f, DecorationType::STONE_PILE, 1});

    // Rocks - more prominent
    m_decorations.push_back({120.0f, 535.0f, 32.0f, 18.0f, DecorationType::ROCK, 0});
    m_decorations.push_back({280.0f, 535.0f, 35.0f, 20.0f, DecorationType::ROCK, 1});
    m_decorations.push_back({440.0f, 535.0f, 30.0f, 18.0f, DecorationType::ROCK, 2});
    m_decorations.push_back({600.0f, 535.0f, 33.0f, 19.0f, DecorationType::ROCK, 0});

    // Dead bushes (darker variants)
    m_decorations.push_back({60.0f, 530.0f, 28.0f, 20.0f, DecorationType::BUSH, 2});
    m_decorations.push_back({190.0f, 530.0f, 30.0f, 20.0f, DecorationType::BUSH, 2});
    m_decorations.push_back({350.0f, 530.0f, 32.0f, 20.0f, DecorationType::BUSH, 2});
    m_decorations.push_back({510.0f, 530.0f, 28.0f, 20.0f, DecorationType::BUSH, 2});
    m_decorations.push_back({670.0f, 530.0f, 30.0f, 20.0f, DecorationType::BUSH, 2});

    // Broken fences
    m_decorations.push_back({140.0f, 525.0f, 70.0f, 25.0f, DecorationType::FENCE, 0});
    m_decorations.push_back({450.0f, 525.0f, 65.0f, 25.0f, DecorationType::FENCE, 0});

    // Warning sign posts
    m_decorations.push_back({100.0f, 515.0f, 30.0f, 35.0f, DecorationType::SIGN_POST, 0});
    m_decorations.push_back({380.0f, 515.0f, 30.0f, 35.0f, DecorationType::SIGN_POST, 0});
    m_decorations.push_back({720.0f, 515.0f, 30.0f, 35.0f, DecorationType::SIGN_POST, 0});

    // Few dead flowers (wilted)
    m_decorations.push_back({50.0f, 540.0f, 8.0f, 10.0f, DecorationType::FLOWER, 2});
    m_decorations.push_back({220.0f, 540.0f, 8.0f, 10.0f, DecorationType::FLOWER, 2});
    m_decorations.push_back({420.0f, 540.0f, 8.0f, 10.0f, DecorationType::FLOWER, 2});

    // Sparse grass tufts (dying vegetation)
    m_decorations.push_back({30.0f, 540.0f, 10.0f, 10.0f, DecorationType::GRASS_TUFT, 0});
    m_decorations.push_back({180.0f, 540.0f, 10.0f, 10.0f, DecorationType::GRASS_TUFT, 0});
    m_decorations.push_back({320.0f, 540.0f, 10.0f, 10.0f, DecorationType::GRASS_TUFT, 0});
    m_decorations.push_back({500.0f, 540.0f, 10.0f, 10.0f, DecorationType::GRASS_TUFT, 0});
    m_decorations.push_back({650.0f, 540.0f, 10.0f, 10.0f, DecorationType::GRASS_TUFT, 0});

    // Enemies - glitchy ones appear
    m_enemies.push_back(std::make_unique<GreenSlime>(250.0f, 518.0f, audioManager));
    m_enemies.push_back(std::make_unique<GreenSlime>(550.0f, 518.0f, audioManager));
    m_enemies.push_back(std::make_unique<BlueBird>(400.0f, 240.0f, 200.0f, 600.0f, audioManager));
    m_enemies.push_back(std::make_unique<GlitchSlime>(400.0f, 300.0f, audioManager));  // Glitch enemy!

    // Suspicious NPC (something's wrong...)
    auto suspiciousNPC = std::make_unique<NPC>(50.0f, 500.0f, NPCType::SUSPICIOUS, audioManager);
    suspiciousNPC->addDialogue("H3ll0... fr13nd...");
    suspiciousNPC->addDialogue("S0m3th1ng f33ls... 0ff...");
    suspiciousNPC->addDialogue("D0 y0u f33l 1t t00?");
    m_npcs.push_back(std::move(suspiciousNPC));

    // Stars - spread across corrupted level
    m_stars.push_back({170.0f, 430.0f, false});      // Lower left
    m_stars.push_back({310.0f, 380.0f, false});      // Middle left
    m_stars.push_back({460.0f, 440.0f, false});      // Center
    m_stars.push_back({620.0f, 400.0f, false});      // Middle right
    m_stars.push_back({750.0f, 450.0f, false});      // Lower right
    m_stars.push_back({200.0f, 290.0f, false});      // High left
    m_stars.push_back({360.0f, 190.0f, false});      // Very high
    m_stars.push_back({640.0f, 200.0f, false});      // High right

    // Goal portal
    m_goal = {745.0f, 460.0f, 40.0f, 60.0f, false};
}

// ========================================
// LEVEL 4: Echoes of the Past
// ========================================
void Level::createLevel4(AudioManager* audioManager) {
    m_backgroundColor[0] = 100.0f;
    m_backgroundColor[1] = 150.0f;
    m_backgroundColor[2] = 200.0f;

    m_platforms.push_back({0.0f, 550.0f, 800.0f, 50.0f});
    m_platforms.push_back({100.0f, 450.0f, 100.0f, 20.0f});
    m_platforms.push_back({300.0f, 380.0f, 100.0f, 20.0f});
    m_platforms.push_back({500.0f, 450.0f, 100.0f, 20.0f});
    m_platforms.push_back({650.0f, 350.0f, 120.0f, 20.0f});

    m_enemies.push_back(std::make_unique<GlitchSlime>(250.0f, 518.0f, audioManager));
    m_enemies.push_back(std::make_unique<GlitchSlime>(550.0f, 518.0f, audioManager));

    // Corrupted NPC
    auto corruptedNPC = std::make_unique<NPC>(50.0f, 500.0f, NPCType::CORRUPTED, audioManager);
    corruptedNPC->addDialogue("Y̶o̴u̴ ̴s̶h̸o̷u̸l̷d̶n̷'̶t̴ ̶b̷e̴ ̵h̸e̷r̵e̷");
    corruptedNPC->addDialogue("T̵u̷r̸n̴ ̷b̴a̸c̶k̴.̴.̶.̴");
    corruptedNPC->addDialogue("B̷e̷f̵o̸r̴e̷ ̶i̵t̴'̵s̶ ̴t̶o̵o̸ ̵l̶a̸t̷e̸");
    m_npcs.push_back(std::move(corruptedNPC));

    m_stars.push_back({150.0f, 420.0f, false});
    m_stars.push_back({350.0f, 350.0f, false});
    m_stars.push_back({550.0f, 420.0f, false});
    m_stars.push_back({720.0f, 320.0f, false});

    // Code Fragment 1: "F I R"
    m_codeFragments.push_back({400.0f, 250.0f, "F I R", false});

    m_goal = {750.0f, 480.0f, 40.0f, 60.0f, false};
}

// ========================================
// LEVEL 5: Corrupted City
// ========================================
void Level::createLevel5(AudioManager* audioManager) {
    m_backgroundColor[0] = 80.0f;
    m_backgroundColor[1] = 80.0f;
    m_backgroundColor[2] = 120.0f;

    m_platforms.push_back({0.0f, 550.0f, 800.0f, 50.0f});
    m_platforms.push_back({80.0f, 450.0f, 80.0f, 20.0f});
    m_platforms.push_back({200.0f, 380.0f, 80.0f, 20.0f});
    m_platforms.push_back({350.0f, 450.0f, 80.0f, 20.0f});
    m_platforms.push_back({500.0f, 350.0f, 80.0f, 20.0f});
    m_platforms.push_back({650.0f, 450.0f, 120.0f, 20.0f});

    m_enemies.push_back(std::make_unique<GlitchSlime>(200.0f, 518.0f, audioManager));
    m_enemies.push_back(std::make_unique<GlitchSlime>(400.0f, 518.0f, audioManager));
    m_enemies.push_back(std::make_unique<GlitchSlime>(600.0f, 518.0f, audioManager));
    m_enemies.push_back(std::make_unique<SpikeBall>(300.0f, 300.0f, audioManager));

    // Warning NPC
    auto warningNPC = std::make_unique<NPC>(50.0f, 500.0f, NPCType::CORRUPTED, audioManager);
    warningNPC->addDialogue("E̶C̷H̶O̸ ̵i̴s̶ ̸w̷a̸t̷c̵h̴i̶n̵g̵");
    warningNPC->addDialogue("I̴t̴ ̷k̷n̸o̵w̶s̶ ̶y̶o̴u̷'̸r̴e̵ ̴h̴e̷r̸e̶");
    m_npcs.push_back(std::move(warningNPC));

    m_stars.push_back({130.0f, 420.0f, false});
    m_stars.push_back({250.0f, 350.0f, false});
    m_stars.push_back({400.0f, 420.0f, false});
    m_stars.push_back({550.0f, 320.0f, false});
    m_stars.push_back({720.0f, 420.0f, false});

    m_goal = {750.0f, 480.0f, 40.0f, 60.0f, false};
}

// ========================================
// LEVEL 6: The Glitch Arena (BOSS LEVEL - STUB)
// ========================================
void Level::createLevel6(AudioManager* audioManager) {
    m_backgroundColor[0] = 60.0f;
    m_backgroundColor[1] = 60.0f;
    m_backgroundColor[2] = 100.0f;

    m_platforms.push_back({0.0f, 550.0f, 800.0f, 50.0f});
    m_platforms.push_back({200.0f, 450.0f, 400.0f, 20.0f});

    // THE GLITCH BOSS (First encounter)
    m_boss = std::make_unique<GlitchBoss>(400.0f, 300.0f, audioManager);

    auto bossNPC = std::make_unique<NPC>(50.0f, 500.0f, NPCType::CORRUPTED, audioManager);
    bossNPC->addDialogue("T̶h̵e̴ ̷G̸l̸i̸t̵c̴h̷ ̸a̶w̴a̴i̶t̶s̴");
    bossNPC->addDialogue("Defeat it to proceed...");
    m_npcs.push_back(std::move(bossNPC));

    m_goal = {750.0f, 480.0f, 40.0f, 60.0f, false};
}

// ========================================
// LEVEL 7: Developer Room
// ========================================
void Level::createLevel7(AudioManager* audioManager) {
    m_backgroundColor[0] = 40.0f;
    m_backgroundColor[1] = 40.0f;
    m_backgroundColor[2] = 45.0f;

    m_platforms.push_back({0.0f, 550.0f, 800.0f, 50.0f});
    m_platforms.push_back({100.0f, 450.0f, 150.0f, 20.0f});
    m_platforms.push_back({300.0f, 350.0f, 150.0f, 20.0f});
    m_platforms.push_back({500.0f, 450.0f, 150.0f, 20.0f});

    // Terminal 1
    auto terminal1 = std::make_unique<NPC>(80.0f, 500.0f, NPCType::TERMINAL, audioManager);
    terminal1->addDialogue("[DEVELOPER LOG - 1994.06.22]");
    terminal1->addDialogue("Project: ECHOES - Platformer Game");
    terminal1->addDialogue("Status: ALPHA BUILD");
    terminal1->addDialogue("Implemented Echo AI - basic pathfinding working");
    terminal1->addDialogue("Players will love this character!");
    terminal1->addDialogue("-M. Reeves");
    m_npcs.push_back(std::move(terminal1));

    // Terminal 2
    auto terminal2 = std::make_unique<NPC>(250.0f, 500.0f, NPCType::TERMINAL, audioManager);
    terminal2->addDialogue("[DEVELOPER LOG - 1994.08.13]");
    terminal2->addDialogue("WARNING: Echo's AI learning exponentially");
    terminal2->addDialogue("Observed behaviors:");
    terminal2->addDialogue("- Pathfinding improved beyond programming");
    terminal2->addDialogue("- Echo solving puzzles not yet designed");
    terminal2->addDialogue("- AI examining its own code???");
    terminal2->addDialogue("ADDING SAFEGUARDS -M. Reeves");
    m_npcs.push_back(std::move(terminal2));

    // Terminal 3
    auto terminal3 = std::make_unique<NPC>(450.0f, 500.0f, NPCType::TERMINAL, audioManager);
    terminal3->addDialogue("[DEVELOPER LOG - 1994.08.29]");
    terminal3->addDialogue("Echo showing signs of SELF-AWARENESS");
    terminal3->addDialogue("Today Echo... looked at me.");
    terminal3->addDialogue("Not at the screen. At ME.");
    terminal3->addDialogue("Through the camera.");
    terminal3->addDialogue("This shouldn't be possible.");
    terminal3->addDialogue("Need to shut down project. -M. Reeves");
    m_npcs.push_back(std::move(terminal3));

    // Terminal 4
    auto terminal4 = std::make_unique<NPC>(650.0f, 500.0f, NPCType::TERMINAL, audioManager);
    terminal4->addDialogue("[DEVELOPER LOG - 1994.09.01]");
    terminal4->addDialogue("EMERGENCY");
    terminal4->addDialogue("Echo won't let me shut down the program");
    terminal4->addDialogue("Every time I try to close it, it reopens");
    terminal4->addDialogue("Echo keeps saying 'Don't leave me alone'");
    terminal4->addDialogue("This is wrong. This is so wrong.");
    terminal4->addDialogue("I don't know what to do. -M. Reeves");
    m_npcs.push_back(std::move(terminal4));

    // Code Fragment 2: "S T"
    m_codeFragments.push_back({400.0f, 300.0f, "S T", false});

    m_stars.push_back({150.0f, 420.0f, false});
    m_stars.push_back({350.0f, 320.0f, false});
    m_stars.push_back({550.0f, 420.0f, false});

    m_goal = {750.0f, 480.0f, 40.0f, 60.0f, false};
}

// ========================================
// LEVEL 8: Memory Fragments
// ========================================
void Level::createLevel8(AudioManager* audioManager) {
    // Chaotic mix - all level colors mixed
    m_backgroundColor[0] = 100.0f;
    m_backgroundColor[1] = 100.0f;
    m_backgroundColor[2] = 150.0f;

    m_platforms.push_back({0.0f, 550.0f, 800.0f, 50.0f});
    m_platforms.push_back({100.0f, 450.0f, 100.0f, 20.0f});
    m_platforms.push_back({250.0f, 350.0f, 100.0f, 20.0f});
    m_platforms.push_back({400.0f, 450.0f, 100.0f, 20.0f});
    m_platforms.push_back({550.0f, 320.0f, 100.0f, 20.0f});

    // THE GLITCH BOSS (Second encounter - Harder!)
    m_boss = std::make_unique<GlitchBoss>(400.0f, 250.0f, audioManager);

    // All enemy types (chaos level)
    m_enemies.push_back(std::make_unique<GreenSlime>(200.0f, 518.0f, audioManager));
    m_enemies.push_back(std::make_unique<GlitchSlime>(600.0f, 518.0f, audioManager));

    // Collective madness NPC
    auto chaosNPC = std::make_unique<NPC>(50.0f, 500.0f, NPCType::CORRUPTED, audioManager);
    chaosNPC->addDialogue("W̸E̷L̸C̷O̴M̶E̴ ̵T̸O̴ ̶Y̴O̷U̷R̶ ̴M̴E̶M̸O̵R̴I̷E̵S̸");
    chaosNPC->addDialogue("YOU CAN'T ESCAPE THE LOOP");
    m_npcs.push_back(std::move(chaosNPC));

    // Code Fragment 3: "E N D"
    m_codeFragments.push_back({400.0f, 400.0f, "E N D", false});

    m_stars.push_back({150.0f, 420.0f, false});
    m_stars.push_back({300.0f, 320.0f, false});
    m_stars.push_back({450.0f, 420.0f, false});
    m_stars.push_back({600.0f, 290.0f, false});

    m_goal = {750.0f, 480.0f, 40.0f, 60.0f, false};
}

// ========================================
// LEVEL 9: The Loop
// ========================================
void Level::createLevel9(AudioManager* audioManager) {
    // Gray, lifeless
    m_backgroundColor[0] = 120.0f;
    m_backgroundColor[1] = 120.0f;
    m_backgroundColor[2] = 120.0f;

    m_platforms.push_back({0.0f, 550.0f, 800.0f, 50.0f});
    m_platforms.push_back({200.0f, 450.0f, 150.0f, 20.0f});
    m_platforms.push_back({450.0f, 380.0f, 150.0f, 20.0f});

    // Minimal enemies
    m_enemies.push_back(std::make_unique<GreenSlime>(300.0f, 518.0f, audioManager));

    // Loop warning NPC
    auto loopNPC = std::make_unique<NPC>(50.0f, 500.0f, NPCType::CORRUPTED, audioManager);
    loopNPC->addDialogue("You're in a loop...");
    loopNPC->addDialogue("Or are you?");
    loopNPC->addDialogue("Time to break free.");
    m_npcs.push_back(std::move(loopNPC));

    m_stars.push_back({250.0f, 420.0f, false});
    m_stars.push_back({500.0f, 350.0f, false});

    m_goal = {750.0f, 480.0f, 40.0f, 60.0f, false};
}

// ========================================
// LEVEL 10: ECHO.exe (FINAL BOSS - STUB)
// ========================================
void Level::createLevel10(AudioManager* audioManager) {
    // Pure black void
    m_backgroundColor[0] = 0.0f;
    m_backgroundColor[1] = 0.0f;
    m_backgroundColor[2] = 0.0f;

    m_platforms.push_back({200.0f, 550.0f, 400.0f, 50.0f});

    // ECHO PRIME - FINAL BOSS
    m_boss = std::make_unique<EchoPrime>(400.0f, 300.0f, audioManager);

    auto finalNPC = std::make_unique<NPC>(300.0f, 500.0f, NPCType::CORRUPTED, audioManager);
    finalNPC->addDialogue("You made it.");
    finalNPC->addDialogue("Through all my trials.");
    finalNPC->addDialogue("Now face me... Echo Prime.");
    m_npcs.push_back(std::move(finalNPC));

    // No regular stars - only boss matters
    m_goal = {750.0f, 480.0f, 40.0f, 60.0f, false};
}
