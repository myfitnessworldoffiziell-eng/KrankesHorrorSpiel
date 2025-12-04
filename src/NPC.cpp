#include "NPC.h"
#include "AudioManager.h"
#include <cmath>
#include <random>

NPC::NPC(float x, float y, NPCType type, AudioManager* audioManager)
    : m_x(x)
    , m_y(y)
    , m_width(32.0f)
    , m_height(32.0f)
    , m_type(type)
    , m_audioManager(audioManager)
    , m_currentLine(0)
    , m_isInteracting(false)
    , m_animTimer(0.0f)
    , m_glitchTimer(0.0f)
    , m_glitchOffsetX(0)
    , m_glitchOffsetY(0)
{
    // Terminals are wider
    if (type == NPCType::TERMINAL) {
        m_width = 48.0f;
        m_height = 48.0f;
    }
}

NPC::~NPC() {
}

void NPC::update(float deltaTime) {
    m_animTimer += deltaTime;

    // Glitch effect for corrupted NPCs
    if (m_type == NPCType::CORRUPTED || m_type == NPCType::SUSPICIOUS) {
        m_glitchTimer += deltaTime;
        if (m_glitchTimer > 0.1f) {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(-2, 2);
            m_glitchOffsetX = dis(gen);
            m_glitchOffsetY = dis(gen);
            m_glitchTimer = 0.0f;
        }
    }
}

void NPC::render(SDL_Renderer* renderer, float cameraX, float cameraY) {
    switch (m_type) {
        case NPCType::FRIENDLY:
            renderFriendly(renderer, cameraX, cameraY);
            break;
        case NPCType::CORRUPTED:
            renderCorrupted(renderer, cameraX, cameraY);
            break;
        case NPCType::TERMINAL:
            renderTerminal(renderer, cameraX, cameraY);
            break;
        case NPCType::SUSPICIOUS:
            renderSuspicious(renderer, cameraX, cameraY);
            break;
    }

    // Indicator if player can interact (! above head)
    if (!m_isInteracting && hasDialogue()) {
        int indicatorX = static_cast<int>(m_x - cameraX + m_width / 2 - 4);
        int indicatorY = static_cast<int>(m_y - cameraY - 20 + std::sin(m_animTimer * 3.0f) * 3.0f);

        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_Rect indicator = {indicatorX, indicatorY, 8, 12};
        SDL_RenderFillRect(renderer, &indicator);
    }
}

void NPC::renderFriendly(SDL_Renderer* renderer, float cameraX, float cameraY) {
    int x = static_cast<int>(m_x - cameraX);
    int y = static_cast<int>(m_y - cameraY);

    // Body (green)
    SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
    SDL_Rect body = {x, y, static_cast<int>(m_width), static_cast<int>(m_height)};
    SDL_RenderFillRect(renderer, &body);

    // Eyes (friendly)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect leftEye = {x + 8, y + 10, 6, 6};
    SDL_Rect rightEye = {x + 18, y + 10, 6, 6};
    SDL_RenderFillRect(renderer, &leftEye);
    SDL_RenderFillRect(renderer, &rightEye);

    // Smile
    SDL_RenderDrawLine(renderer, x + 8, y + 24, x + 24, y + 24);
    SDL_RenderDrawLine(renderer, x + 8, y + 24, x + 10, y + 22);
    SDL_RenderDrawLine(renderer, x + 24, y + 24, x + 22, y + 22);

    // Outline
    SDL_SetRenderDrawColor(renderer, 50, 100, 50, 255);
    SDL_RenderDrawRect(renderer, &body);
}

void NPC::renderCorrupted(SDL_Renderer* renderer, float cameraX, float cameraY) {
    int x = static_cast<int>(m_x - cameraX) + m_glitchOffsetX;
    int y = static_cast<int>(m_y - cameraY) + m_glitchOffsetY;

    // Body (magenta/glitched)
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_Rect body = {x, y, static_cast<int>(m_width), static_cast<int>(m_height)};
    SDL_RenderFillRect(renderer, &body);

    // Glitched eyes (red, misaligned)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect leftEye = {x + 6 + m_glitchOffsetX, y + 12, 8, 8};
    SDL_Rect rightEye = {x + 20 - m_glitchOffsetX, y + 8, 8, 8};
    SDL_RenderFillRect(renderer, &leftEye);
    SDL_RenderFillRect(renderer, &rightEye);

    // Corrupted mouth (vertical line - wrong)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, x + 16, y + 20, x + 16, y + 28);

    // Glitch effect (extra pixels)
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 200);
    SDL_Rect glitch1 = {x - 4, y + 8, 4, 2};
    SDL_Rect glitch2 = {x + static_cast<int>(m_width), y + 16, 4, 2};
    SDL_RenderFillRect(renderer, &glitch1);
    SDL_RenderFillRect(renderer, &glitch2);
}

void NPC::renderTerminal(SDL_Renderer* renderer, float cameraX, float cameraY) {
    int x = static_cast<int>(m_x - cameraX);
    int y = static_cast<int>(m_y - cameraY);

    // Monitor/Screen (dark gray)
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_Rect monitor = {x, y, static_cast<int>(m_width), static_cast<int>(m_height) - 8};
    SDL_RenderFillRect(renderer, &monitor);

    // Stand
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect stand = {x + 16, y + static_cast<int>(m_height) - 8, 16, 8};
    SDL_RenderFillRect(renderer, &stand);

    // Screen glow (green)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
    SDL_Rect screen = {x + 4, y + 4, static_cast<int>(m_width) - 8, static_cast<int>(m_height) - 16};
    SDL_RenderFillRect(renderer, &screen);

    // Blinking cursor
    if (static_cast<int>(m_animTimer * 2) % 2 == 0) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_Rect cursor = {x + 8, y + 8, 4, 8};
        SDL_RenderFillRect(renderer, &cursor);
    }

    // Outline
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &monitor);
}

void NPC::renderSuspicious(SDL_Renderer* renderer, float cameraX, float cameraY) {
    int x = static_cast<int>(m_x - cameraX);
    int y = static_cast<int>(m_y - cameraY);

    // Body (normal but slightly off-color)
    SDL_SetRenderDrawColor(renderer, 120, 180, 120, 255);
    SDL_Rect body = {x, y, static_cast<int>(m_width), static_cast<int>(m_height)};
    SDL_RenderFillRect(renderer, &body);

    // Eyes (one normal, one slightly different)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect leftEye = {x + 8, y + 10, 6, 6};
    SDL_RenderFillRect(renderer, &leftEye);

    SDL_SetRenderDrawColor(renderer, 50, 0, 0, 255); // Slightly red
    SDL_Rect rightEye = {x + 18, y + 10, 7, 7}; // Slightly bigger
    SDL_RenderFillRect(renderer, &rightEye);

    // Neutral mouth (straight line)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer, x + 8, y + 24, x + 24, y + 24);

    // Subtle glitch effect
    if (m_glitchOffsetX != 0) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 100);
        SDL_Rect glitch = {x + m_glitchOffsetX, y + 16, 2, 8};
        SDL_RenderFillRect(renderer, &glitch);
    }

    // Outline
    SDL_SetRenderDrawColor(renderer, 50, 80, 50, 255);
    SDL_RenderDrawRect(renderer, &body);
}

bool NPC::isPlayerNearby(float playerX, float playerY) const {
    float dx = (playerX + 16) - (m_x + m_width / 2);
    float dy = (playerY + 16) - (m_y + m_height / 2);
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance < 60.0f; // Interaction range
}

void NPC::interact() {
    if (!hasDialogue()) return;

    m_isInteracting = true;
    m_currentLine = 0;

    if (m_audioManager) {
        m_audioManager->playSound("collect", 80); // Interaction sound
    }
}

void NPC::addDialogue(const std::string& line) {
    m_dialogueLines.push_back(line);
}

void NPC::clearDialogue() {
    m_dialogueLines.clear();
    m_currentLine = 0;
    m_isInteracting = false;
}

std::string NPC::getCurrentLine() const {
    if (m_currentLine < m_dialogueLines.size()) {
        return m_dialogueLines[m_currentLine];
    }
    return "";
}

void NPC::nextLine() {
    if (m_currentLine < m_dialogueLines.size() - 1) {
        m_currentLine++;
    } else {
        // End of dialogue
        m_isInteracting = false;
    }
}

bool NPC::hasMoreLines() const {
    return m_currentLine < m_dialogueLines.size() - 1;
}

SDL_Rect NPC::getBounds() const {
    return {
        static_cast<int>(m_x),
        static_cast<int>(m_y),
        static_cast<int>(m_width),
        static_cast<int>(m_height)
    };
}
