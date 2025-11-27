#include "DialogSystem.h"
#include "UIHelper.h"
#include <iostream>

DialogSystem::DialogSystem()
    : m_active(false)
    , m_currentTime(0.0f)
    , m_waitingForInput(false)
    , m_charIndex(0)
    , m_typewriterTimer(0.0f)
    , m_typewriterSpeed(30.0f) // 30 chars/sec
{
}

DialogSystem::~DialogSystem() {
}

void DialogSystem::addDialog(const std::string& speaker, const std::string& text, float displayTime) {
    Dialog dialog;
    dialog.speaker = speaker;
    dialog.text = text;
    dialog.displayTime = displayTime;
    m_dialogQueue.push(dialog);
}

void DialogSystem::addDialogSequence(const std::vector<Dialog>& dialogs) {
    for (const auto& dialog : dialogs) {
        m_dialogQueue.push(dialog);
    }
}

void DialogSystem::start() {
    if (!m_dialogQueue.empty()) {
        m_active = true;
        nextDialog();
        std::cout << "[DialogSystem] Started" << std::endl;
    }
}

void DialogSystem::skip() {
    if (!m_active) return;

    // If typewriter still going, complete text
    if (m_charIndex < m_currentDialog.text.length()) {
        m_displayedText = m_currentDialog.text;
        m_charIndex = m_currentDialog.text.length();
    } else {
        // Move to next dialog
        nextDialog();
    }
}

void DialogSystem::clear() {
    while (!m_dialogQueue.empty()) {
        m_dialogQueue.pop();
    }
    m_active = false;
    m_displayedText.clear();
}

void DialogSystem::update(float deltaTime) {
    if (!m_active) return;

    m_currentTime += deltaTime;

    // Typewriter effect
    if (m_charIndex < m_currentDialog.text.length()) {
        m_typewriterTimer += deltaTime;
        float charsToAdd = m_typewriterTimer * m_typewriterSpeed;
        if (charsToAdd >= 1.0f) {
            int addCount = static_cast<int>(charsToAdd);
            m_typewriterTimer -= addCount / m_typewriterSpeed;

            for (int i = 0; i < addCount && m_charIndex < m_currentDialog.text.length(); ++i) {
                m_displayedText += m_currentDialog.text[m_charIndex];
                m_charIndex++;
            }
        }
    }

    // Auto-advance if displayTime > 0
    if (m_currentDialog.displayTime > 0.0f && m_currentTime >= m_currentDialog.displayTime) {
        if (m_charIndex >= m_currentDialog.text.length()) {
            nextDialog();
        }
    }
}

void DialogSystem::render(SDL_Renderer* renderer) {
    if (!m_active) return;

    // Dialog box at bottom of screen
    SDL_Rect boxRect = {50, 450, 700, 120};
    SDL_Color bgColor = {0, 0, 0, 220};
    SDL_Color borderColor = {255, 255, 255, 255};
    UIHelper::renderBox(renderer, boxRect, bgColor, borderColor, 3);

    // Speaker name (if any)
    if (!m_currentDialog.speaker.empty()) {
        SDL_Color speakerColor = {255, 200, 0, 255};
        UIHelper::renderText(renderer, m_currentDialog.speaker, 70, 460, speakerColor, 18, false);
    }

    // Dialog text (with typewriter effect)
    SDL_Color textColor = {255, 255, 255, 255};
    int textY = m_currentDialog.speaker.empty() ? 470 : 490;

    // Word wrap (simple version)
    std::string wrappedText = m_displayedText;
    // TODO: Proper word wrapping
    UIHelper::renderText(renderer, wrappedText, 70, textY, textColor, 16, false);

    // Continue indicator (if waiting for input)
    if (m_charIndex >= m_currentDialog.text.length() && m_currentDialog.displayTime == 0.0f) {
        SDL_Color indicatorColor = {255, 255, 255, static_cast<Uint8>(128 + 127 * sin(m_currentTime * 5.0f))};
        UIHelper::renderText(renderer, "[SPACE to continue]", 720, 545, indicatorColor, 12, true);
    }
}

void DialogSystem::handleInput(SDL_Event& event) {
    if (!m_active) return;

    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_RETURN) {
            skip();
        }
    }
}

void DialogSystem::nextDialog() {
    if (m_dialogQueue.empty()) {
        m_active = false;
        std::cout << "[DialogSystem] Finished" << std::endl;
        return;
    }

    m_currentDialog = m_dialogQueue.front();
    m_dialogQueue.pop();

    m_displayedText.clear();
    m_charIndex = 0;
    m_typewriterTimer = 0.0f;
    m_currentTime = 0.0f;

    std::cout << "[DialogSystem] Next: " << m_currentDialog.speaker << ": " << m_currentDialog.text << std::endl;
}
