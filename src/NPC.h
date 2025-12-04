#pragma once
#include "SDL_Platform.h"

#include <string>
#include <vector>

class AudioManager;

/**
 * NPC Type
 */
enum class NPCType {
    FRIENDLY,       // Normal friendly NPC
    CORRUPTED,      // Glitched/corrupted NPC
    TERMINAL,       // Computer terminal (read-only)
    SUSPICIOUS      // Seems off
};

/**
 * NPC
 * Interactive character or terminal that can display dialogue
 */
class NPC {
public:
    NPC(float x, float y, NPCType type, AudioManager* audioManager = nullptr);
    ~NPC();

    void update(float deltaTime);
    void render(SDL_Renderer* renderer, float cameraX = 0.0f, float cameraY = 0.0f);

    // Interaction
    bool isPlayerNearby(float playerX, float playerY) const;
    void interact(); // Trigger dialogue
    bool hasDialogue() const { return !m_dialogueLines.empty(); }
    bool isInteracting() const { return m_isInteracting; }
    void stopInteraction() { m_isInteracting = false; m_currentLine = 0; }

    // Dialogue management
    void addDialogue(const std::string& line);
    void clearDialogue();
    std::string getCurrentLine() const;
    void nextLine();
    bool hasMoreLines() const;

    // Getters
    SDL_Rect getBounds() const;
    NPCType getType() const { return m_type; }
    float getX() const { return m_x; }
    float getY() const { return m_y; }

private:
    float m_x, m_y;
    float m_width, m_height;
    NPCType m_type;
    AudioManager* m_audioManager;

    // Dialogue
    std::vector<std::string> m_dialogueLines;
    size_t m_currentLine;
    bool m_isInteracting;

    // Animation
    float m_animTimer;
    float m_glitchTimer;
    int m_glitchOffsetX, m_glitchOffsetY;

    // Visual
    void renderFriendly(SDL_Renderer* renderer, float cameraX, float cameraY);
    void renderCorrupted(SDL_Renderer* renderer, float cameraX, float cameraY);
    void renderTerminal(SDL_Renderer* renderer, float cameraX, float cameraY);
    void renderSuspicious(SDL_Renderer* renderer, float cameraX, float cameraY);
};
