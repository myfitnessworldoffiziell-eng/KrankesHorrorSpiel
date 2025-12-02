#pragma once
#include "SDL_Platform.h"

#include <string>

/**
 * FakeBlueScreen
 * Simuliert einen Blue Screen of Death (BSOD)
 * Komplett harmlos, aber erschreckend!
 */
class FakeBlueScreen {
public:
    FakeBlueScreen();
    ~FakeBlueScreen();

    void trigger(float duration = 5.0f); // duration in seconds
    void skip(); // Force-Ende

    bool isActive() const { return m_active; }

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

private:
    bool m_active;
    float m_duration;
    float m_timer;

    std::string m_errorMessage;
};
