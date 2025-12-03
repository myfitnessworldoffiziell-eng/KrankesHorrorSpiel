#include "MainMenu.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

MainMenu::MainMenu()
    : m_result(MenuResult::NONE)
    , m_glitchTimer(0.0f)
    , m_shouldGlitch(false)
    , m_pulseTimer(0.0f)
    , m_corruptionTimer(0.0f)
    , m_glitchIntensity(0)
    , m_particleSpawnTimer(0.0f)
    , m_shakeIntensity(0.0f)
    , m_shakeX(0.0f)
    , m_shakeY(0.0f)
    , m_mouseX(0)
    , m_mouseY(0)
    , m_mouseClicked(false)
{
    // Seed random
    srand(static_cast<unsigned>(time(nullptr)));

    // Create buttons
    UIHelper::Button startButton;
    startButton.rect = {300, 250, 200, 50};
    startButton.text = "START GAME";
    startButton.hovered = false;
    startButton.clicked = false;
    m_buttons.push_back(startButton);

    UIHelper::Button settingsButton;
    settingsButton.rect = {300, 320, 200, 50};
    settingsButton.text = "SETTINGS";
    settingsButton.hovered = false;
    settingsButton.clicked = false;
    m_buttons.push_back(settingsButton);

    UIHelper::Button quitButton;
    quitButton.rect = {300, 390, 200, 50};
    quitButton.text = "QUIT";
    quitButton.hovered = false;
    quitButton.clicked = false;
    m_buttons.push_back(quitButton);

    // Create creepy background elements
    for (int i = 0; i < 15; i++) {
        BackgroundElement elem;
        elem.x = static_cast<float>(rand() % 800);
        elem.y = static_cast<float>(rand() % 600);
        elem.width = 40.0f + static_cast<float>(rand() % 60);
        elem.height = 40.0f + static_cast<float>(rand() % 60);
        elem.type = rand() % 3;  // 0=shadow, 1=crack, 2=corruption
        elem.pulsePhase = static_cast<float>(rand() % 628) / 100.0f;  // Random phase
        m_backgroundElements.push_back(elem);
    }

    // Spawn initial particles
    for (int i = 0; i < 20; i++) {
        spawnRandomParticle();
    }
}

MainMenu::~MainMenu() {
}

void MainMenu::handleInput(SDL_Event& event) {
    if (event.type == SDL_MOUSEMOTION) {
        m_mouseX = event.motion.x;
        m_mouseY = event.motion.y;
    } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        m_mouseClicked = true;
    }
}

MainMenu::MenuResult MainMenu::update(float deltaTime) {
    m_result = MenuResult::NONE;

    // Update all horror effects
    updateHorrorEffects(deltaTime);
    updateParticles(deltaTime);

    // Update buttons
    if (UIHelper::renderButton(nullptr, m_buttons[0], m_mouseX, m_mouseY, m_mouseClicked)) {
        std::cout << "[MainMenu] START GAME clicked" << std::endl;
        m_result = MenuResult::START_GAME;
    }

    if (UIHelper::renderButton(nullptr, m_buttons[1], m_mouseX, m_mouseY, m_mouseClicked)) {
        std::cout << "[MainMenu] SETTINGS clicked" << std::endl;
        m_result = MenuResult::SETTINGS;
    }

    if (UIHelper::renderButton(nullptr, m_buttons[2], m_mouseX, m_mouseY, m_mouseClicked)) {
        std::cout << "[MainMenu] QUIT clicked" << std::endl;
        m_result = MenuResult::QUIT;
    }

    m_mouseClicked = false; // Reset click state

    return m_result;
}

void MainMenu::updateHorrorEffects(float deltaTime) {
    // Glitch effect timer (more frequent, more intense)
    m_glitchTimer += deltaTime;
    if (m_glitchTimer > 3.0f + static_cast<float>(rand() % 4)) {
        m_shouldGlitch = true;
        m_glitchIntensity = 1 + rand() % 10;
        m_glitchTimer = 0.0f;

        // Occasional screen shake
        if (rand() % 100 < 20) {  // 20% chance
            m_shakeIntensity = 5.0f + static_cast<float>(rand() % 10);
        }
    } else {
        m_shouldGlitch = false;
    }

    // Pulse timer (breathing effect)
    m_pulseTimer += deltaTime * 2.0f;
    if (m_pulseTimer > 6.28f) m_pulseTimer = 0.0f;

    // Corruption timer (spreading corruption animation)
    m_corruptionTimer += deltaTime * 0.5f;
    if (m_corruptionTimer > 6.28f) m_corruptionTimer = 0.0f;

    // Update background elements pulses
    for (auto& elem : m_backgroundElements) {
        elem.pulsePhase += deltaTime * 1.5f;
        if (elem.pulsePhase > 6.28f) elem.pulsePhase = 0.0f;
    }

    // Screen shake decay
    if (m_shakeIntensity > 0.0f) {
        m_shakeIntensity -= deltaTime * 20.0f;
        if (m_shakeIntensity < 0.0f) m_shakeIntensity = 0.0f;

        m_shakeX = (static_cast<float>(rand() % 200) / 100.0f - 1.0f) * m_shakeIntensity;
        m_shakeY = (static_cast<float>(rand() % 200) / 100.0f - 1.0f) * m_shakeIntensity;
    }

    // Spawn new particles periodically
    m_particleSpawnTimer += deltaTime;
    if (m_particleSpawnTimer > 0.5f) {
        spawnRandomParticle();
        m_particleSpawnTimer = 0.0f;
    }
}

void MainMenu::updateParticles(float deltaTime) {
    // Update existing particles
    for (auto& p : m_particles) {
        p.x += p.vx * deltaTime;
        p.y += p.vy * deltaTime;
        p.life -= deltaTime;

        // Fade out over time
        p.alpha = p.life / 8.0f;  // 8 seconds lifetime
        if (p.alpha > 1.0f) p.alpha = 1.0f;
        if (p.alpha < 0.0f) p.alpha = 0.0f;
    }

    // Remove dead particles
    m_particles.erase(
        std::remove_if(m_particles.begin(), m_particles.end(),
            [](const HorrorParticle& p) { return p.life <= 0.0f; }),
        m_particles.end()
    );

    // Keep particle count reasonable
    if (m_particles.size() > 50) {
        m_particles.erase(m_particles.begin());
    }
}

void MainMenu::spawnRandomParticle() {
    HorrorParticle p;
    p.x = static_cast<float>(rand() % 800);
    p.y = static_cast<float>(rand() % 600);
    p.vx = (static_cast<float>(rand() % 100) / 100.0f - 0.5f) * 20.0f;
    p.vy = (static_cast<float>(rand() % 100) / 100.0f - 0.5f) * 20.0f;
    p.size = 5.0f + static_cast<float>(rand() % 15);
    p.type = rand() % 3;  // 0=shadow, 1=glitch, 2=eye
    p.life = 5.0f + static_cast<float>(rand() % 6);
    p.alpha = 1.0f;
    m_particles.push_back(p);
}

void MainMenu::render(SDL_Renderer* renderer) {
    // Background (dark with pulsing effect)
    float pulse = std::sin(m_pulseTimer) * 10.0f + 15.0f;
    SDL_SetRenderDrawColor(renderer,
        static_cast<int>(pulse),
        static_cast<int>(pulse),
        static_cast<int>(pulse + 5),
        255);
    SDL_RenderClear(renderer);

    // Render horror background (before everything)
    renderHorrorBackground(renderer);

    // Render particles (floating shadows/glitches)
    renderParticles(renderer);

    // Apply screen shake offset for all UI elements
    int shakeOffsetX = static_cast<int>(m_shakeX);
    int shakeOffsetY = static_cast<int>(m_shakeY);

    // Title with corruption effect
    SDL_Color titleColor;
    std::string titleText;

    if (m_shouldGlitch && m_glitchIntensity > 5) {
        // Strong glitch - red and corrupted
        titleColor = {255, static_cast<Uint8>(rand() % 100), static_cast<Uint8>(rand() % 100), 255};
        titleText = "E̴̢̛̬̩͖̺̺̳͙̬̬̍̌̒̐͑́̕͝C̵̨̨̛̛̰̭̖̩̻̩̪͙̟̟̏̔̏̏̿̚̚͝H̵̨̧̪̦̥̼̰̲̺̦̥͈̿̃̐̑̄̎̚͜͝O̴̧̙̝͇̹̟͖͙̭̙̽̓̋͂͝E̴̡̛̙̺̲̦̿̈́̈͊̆̚͠S̴̬͔͇̘͚̘͉͔̍̇̈́͗̈́̅̚͜͝";
    } else if (m_shouldGlitch) {
        // Mild glitch
        titleColor = {255, 100, 100, 255};
        titleText = "E̴C̷H̶O̸E̷S̸";
    } else {
        // Normal with slight red tint
        titleColor = {220, 200, 200, 255};
        titleText = "ECHOES";
    }

    UIHelper::renderText(renderer, titleText, 400 + shakeOffsetX, 100 + shakeOffsetY, titleColor, 48, true);

    // Subtitle with breathing effect
    float subtitlePulse = std::sin(m_pulseTimer * 0.5f) * 30.0f + 120.0f;
    SDL_Color subtitleColor = {
        static_cast<Uint8>(subtitlePulse),
        static_cast<Uint8>(subtitlePulse * 0.8f),
        static_cast<Uint8>(subtitlePulse * 0.8f),
        255
    };
    UIHelper::renderText(renderer, "The Forgotten Platformer", 400 + shakeOffsetX, 160 + shakeOffsetY, subtitleColor, 18, true);

    // Creepy tagline
    SDL_Color taglineColor = {80, 80, 90, 255};
    UIHelper::renderText(renderer, "...it remembers you...", 400 + shakeOffsetX, 190 + shakeOffsetY, taglineColor, 12, true);

    // Buttons with shake effect
    for (auto& button : m_buttons) {
        // Temporarily offset button for rendering
        button.rect.x += shakeOffsetX;
        button.rect.y += shakeOffsetY;
        UIHelper::renderButton(renderer, button, m_mouseX, m_mouseY, false);
        button.rect.x -= shakeOffsetX;
        button.rect.y -= shakeOffsetY;
    }

    // Render glitch effects (scanlines, artifacts)
    renderGlitchEffects(renderer);

    // Warning text at bottom with pulsing corruption
    float corruptPulse = std::sin(m_corruptionTimer) * 100.0f + 155.0f;
    SDL_Color warningColor = {
        static_cast<Uint8>(corruptPulse + 100),
        static_cast<Uint8>(corruptPulse * 0.3f),
        static_cast<Uint8>(corruptPulse * 0.3f),
        255
    };
    UIHelper::renderText(renderer, "WARNING: This game watches you", 400 + shakeOffsetX, 530 + shakeOffsetY, warningColor, 14, true);

    // Additional creepy message
    SDL_Color creepyColor = {60, 60, 70, 255};
    UIHelper::renderText(renderer, "Do you feel it too?", 400 + shakeOffsetX, 555 + shakeOffsetY, creepyColor, 11, true);
}

void MainMenu::renderHorrorBackground(SDL_Renderer* renderer) {
    // Render background elements (shadows, cracks, corruption)
    for (const auto& elem : m_backgroundElements) {
        float pulse = std::sin(elem.pulsePhase) * 0.3f + 0.7f;

        SDL_Rect rect = {
            static_cast<int>(elem.x),
            static_cast<int>(elem.y),
            static_cast<int>(elem.width),
            static_cast<int>(elem.height)
        };

        switch (elem.type) {
            case 0: // Shadow blob
                SDL_SetRenderDrawColor(renderer,
                    0, 0, 0,
                    static_cast<Uint8>(30.0f * pulse));
                SDL_RenderFillRect(renderer, &rect);
                break;

            case 1: // Crack/tear
                SDL_SetRenderDrawColor(renderer,
                    static_cast<Uint8>(50 * pulse),
                    0, 0,
                    static_cast<Uint8>(80 * pulse));
                SDL_RenderDrawRect(renderer, &rect);
                SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h);
                break;

            case 2: // Corruption patch
                SDL_SetRenderDrawColor(renderer,
                    static_cast<Uint8>(80 * pulse),
                    static_cast<Uint8>(20 * pulse),
                    static_cast<Uint8>(20 * pulse),
                    static_cast<Uint8>(40 * pulse));
                SDL_RenderFillRect(renderer, &rect);
                break;
        }
    }
}

void MainMenu::renderParticles(SDL_Renderer* renderer) {
    for (const auto& p : m_particles) {
        SDL_Rect rect = {
            static_cast<int>(p.x - p.size/2),
            static_cast<int>(p.y - p.size/2),
            static_cast<int>(p.size),
            static_cast<int>(p.size)
        };

        Uint8 alpha = static_cast<Uint8>(p.alpha * 255.0f);

        switch (p.type) {
            case 0: // Shadow particle
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha / 2);
                SDL_RenderFillRect(renderer, &rect);
                break;

            case 1: // Glitch particle (red)
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, alpha);
                SDL_RenderDrawRect(renderer, &rect);
                SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h);
                break;

            case 2: // Eye particle (creepy)
                SDL_SetRenderDrawColor(renderer, 200, 200, 220, alpha);
                SDL_RenderFillRect(renderer, &rect);
                // Pupil
                SDL_Rect pupil = {
                    rect.x + rect.w/4,
                    rect.y + rect.h/4,
                    rect.w/2,
                    rect.h/2
                };
                SDL_SetRenderDrawColor(renderer, 20, 20, 30, alpha);
                SDL_RenderFillRect(renderer, &pupil);
                break;
        }
    }
}

void MainMenu::renderGlitchEffects(SDL_Renderer* renderer) {
    if (m_shouldGlitch) {
        // Scanlines
        for (int y = 0; y < 600; y += 4) {
            if (rand() % 10 < m_glitchIntensity) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 30 + m_glitchIntensity * 5);
                SDL_RenderDrawLine(renderer, 0, y, 800, y);
            }
        }

        // Random glitch blocks
        for (int i = 0; i < m_glitchIntensity; i++) {
            SDL_Rect glitchRect = {
                rand() % 800,
                rand() % 600,
                20 + rand() % 100,
                10 + rand() % 30
            };
            SDL_SetRenderDrawColor(renderer,
                rand() % 255,
                rand() % 100,
                rand() % 100,
                100 + rand() % 100);
            SDL_RenderFillRect(renderer, &glitchRect);
        }

        // Vertical corruption lines
        if (m_glitchIntensity > 7) {
            for (int i = 0; i < 3; i++) {
                int x = rand() % 800;
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 150);
                SDL_RenderDrawLine(renderer, x, 0, x, 600);
            }
        }
    }
}
