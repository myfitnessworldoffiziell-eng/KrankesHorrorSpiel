#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <queue>

/**
 * DialogSystem
 * Textbox-System für Dialoge und Story-Sequenzen
 */
class DialogSystem {
public:
    struct Dialog {
        std::string speaker; // "" für Narrator
        std::string text;
        float displayTime; // 0 = wait for input
    };

    DialogSystem();
    ~DialogSystem();

    // Queue dialogs
    void addDialog(const std::string& speaker, const std::string& text, float displayTime = 0.0f);
    void addDialogSequence(const std::vector<Dialog>& dialogs);

    // Control
    void start();
    void skip(); // Skip current dialog
    void clear();

    bool isActive() const { return m_active; }
    bool hasDialogs() const { return !m_dialogQueue.empty(); }

    // Update & Render
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    // Input handling
    void handleInput(SDL_Event& event);

private:
    void nextDialog();

    std::queue<Dialog> m_dialogQueue;
    Dialog m_currentDialog;

    bool m_active;
    float m_currentTime;
    bool m_waitingForInput;

    // Typewriter effect
    std::string m_displayedText;
    size_t m_charIndex;
    float m_typewriterTimer;
    float m_typewriterSpeed; // chars per second
};
