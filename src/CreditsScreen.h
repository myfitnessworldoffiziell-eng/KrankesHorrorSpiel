#pragma once
#include "SDL_Platform.h"

#include <string>
#include <vector>

/**
 * Credits Screen with GLITCH Effects
 *
 * Shows credits but with increasing corruption/glitches.
 * Echo interrupts the credits to remind you - this isn't over.
 */
class CreditsScreen {
public:
    CreditsScreen();

    // Start the credits
    void start();

    // Update scroll and effects
    void update(float deltaTime);

    // Render credits with glitches
    void render(SDL_Renderer* renderer);

    // Check if active
    bool isActive() const { return m_active; }

    // Check if finished
    bool isFinished() const { return m_finished; }

    // Reset
    void reset();

private:
    bool m_active;
    bool m_finished;

    float m_timer;
    float m_scrollOffset;

    // Credit lines
    std::vector<std::string> m_credits;

    // Glitch intensity (increases over time)
    float m_glitchIntensity;

    // Initialize credit lines
    void initializeCredits();
};
