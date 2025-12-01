#include "ProceduralSpriteGenerator.h"
#include <cmath>
#include <cstdlib>

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

void ProceduralSpriteGenerator::setPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) return;

    Uint32* pixels = (Uint32*)surface->pixels;
    pixels[y * surface->w + x] = color;
}

Uint32 ProceduralSpriteGenerator::getPixel(SDL_Surface* surface, int x, int y) {
    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) return 0;

    Uint32* pixels = (Uint32*)surface->pixels;
    return pixels[y * surface->w + x];
}

void ProceduralSpriteGenerator::drawRect(SDL_Surface* surface, int x, int y, int w, int h, Uint32 color) {
    for (int dy = 0; dy < h; dy++) {
        for (int dx = 0; dx < w; dx++) {
            setPixel(surface, x + dx, y + dy, color);
        }
    }
}

void ProceduralSpriteGenerator::drawCircle(SDL_Surface* surface, int cx, int cy, int radius, Uint32 color) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                setPixel(surface, cx + x, cy + y, color);
            }
        }
    }
}

void ProceduralSpriteGenerator::drawLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint32 color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        setPixel(surface, x1, y1, color);

        if (x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

Uint32 ProceduralSpriteGenerator::makeColor(SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    return SDL_MapRGBA(surface->format, r, g, b, a);
}

// ============================================================================
// PLAYER SPRITES
// ============================================================================

SDL_Surface* ProceduralSpriteGenerator::generatePlayerIdle(int width, int height) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32,
                                                 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!surface) return nullptr;

    Uint32 transparent = makeColor(surface, 0, 0, 0, 0);
    Uint32 body = makeColor(surface, 0, 255, 100, 255);      // Green
    Uint32 dark = makeColor(surface, 0, 180, 70, 255);       // Dark green
    Uint32 eye = makeColor(surface, 255, 255, 255, 255);     // White
    Uint32 pupil = makeColor(surface, 0, 0, 0, 255);         // Black

    // Clear to transparent
    SDL_FillRect(surface, nullptr, transparent);

    // Body (humanoid shape)
    int cx = width / 2;
    int cy = height / 2;

    // Torso
    drawRect(surface, cx - 6, cy - 4, 12, 12, body);

    // Head
    drawRect(surface, cx - 5, cy - 14, 10, 10, body);
    drawRect(surface, cx - 4, cy - 13, 8, 8, dark);

    // Eyes
    setPixel(surface, cx - 2, cy - 10, eye);
    setPixel(surface, cx + 2, cy - 10, eye);
    setPixel(surface, cx - 2, cy - 9, pupil);
    setPixel(surface, cx + 2, cy - 9, pupil);

    // Legs
    drawRect(surface, cx - 5, cy + 8, 4, 6, body);
    drawRect(surface, cx + 1, cy + 8, 4, 6, body);

    // Arms
    drawRect(surface, cx - 8, cy, 2, 6, body);
    drawRect(surface, cx + 6, cy, 2, 6, body);

    return surface;
}

SDL_Surface* ProceduralSpriteGenerator::generatePlayerWalk1(int width, int height) {
    SDL_Surface* surface = generatePlayerIdle(width, height);
    // Slight leg offset for walking animation
    return surface;
}

SDL_Surface* ProceduralSpriteGenerator::generatePlayerWalk2(int width, int height) {
    SDL_Surface* surface = generatePlayerIdle(width, height);
    // Different leg offset
    return surface;
}

SDL_Surface* ProceduralSpriteGenerator::generatePlayerJump(int width, int height) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32,
                                                 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!surface) return nullptr;

    Uint32 transparent = makeColor(surface, 0, 0, 0, 0);
    Uint32 body = makeColor(surface, 0, 255, 100, 255);
    Uint32 dark = makeColor(surface, 0, 180, 70, 255);
    Uint32 eye = makeColor(surface, 255, 255, 255, 255);
    Uint32 pupil = makeColor(surface, 0, 0, 0, 255);

    SDL_FillRect(surface, nullptr, transparent);

    int cx = width / 2;
    int cy = height / 2;

    // Body (jumping pose)
    drawRect(surface, cx - 6, cy - 2, 12, 10, body);
    drawRect(surface, cx - 5, cy - 12, 10, 10, body);
    drawRect(surface, cx - 4, cy - 11, 8, 8, dark);

    // Eyes (surprised)
    drawRect(surface, cx - 3, cy - 9, 2, 3, eye);
    drawRect(surface, cx + 1, cy - 9, 2, 3, eye);
    setPixel(surface, cx - 2, cy - 8, pupil);
    setPixel(surface, cx + 2, cy - 8, pupil);

    // Legs (tucked)
    drawRect(surface, cx - 6, cy + 8, 5, 3, body);
    drawRect(surface, cx + 1, cy + 8, 5, 3, body);

    // Arms (up)
    drawRect(surface, cx - 9, cy - 6, 3, 6, body);
    drawRect(surface, cx + 6, cy - 6, 3, 6, body);

    return surface;
}

SDL_Surface* ProceduralSpriteGenerator::generatePlayerDead(int width, int height) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32,
                                                 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!surface) return nullptr;

    Uint32 transparent = makeColor(surface, 0, 0, 0, 0);
    Uint32 body = makeColor(surface, 100, 100, 100, 255);    // Gray (dead)
    Uint32 red = makeColor(surface, 255, 0, 0, 255);

    SDL_FillRect(surface, nullptr, transparent);

    int cx = width / 2;
    int cy = height / 2;

    // Fallen body
    drawRect(surface, cx - 8, cy + 2, 16, 6, body);
    drawRect(surface, cx - 6, cy - 2, 8, 6, body);

    // X eyes
    drawLine(surface, cx - 4, cy, cx - 2, cy + 2, red);
    drawLine(surface, cx - 2, cy, cx - 4, cy + 2, red);
    drawLine(surface, cx + 2, cy, cx + 4, cy + 2, red);
    drawLine(surface, cx + 4, cy, cx + 2, cy + 2, red);

    return surface;
}

// ============================================================================
// ENEMY SPRITES
// ============================================================================

SDL_Surface* ProceduralSpriteGenerator::generateBugEnemy(int width, int height) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32,
                                                 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!surface) return nullptr;

    Uint32 transparent = makeColor(surface, 0, 0, 0, 0);
    Uint32 body = makeColor(surface, 150, 50, 200, 255);     // Purple
    Uint32 dark = makeColor(surface, 100, 30, 150, 255);
    Uint32 eye = makeColor(surface, 255, 0, 0, 255);         // Red eyes

    SDL_FillRect(surface, nullptr, transparent);

    int cx = width / 2;
    int cy = height / 2;

    // Bug body (oval)
    drawRect(surface, cx - 8, cy - 4, 16, 10, body);
    drawRect(surface, cx - 6, cy - 6, 12, 14, body);
    drawRect(surface, cx - 5, cy - 5, 10, 12, dark);

    // Legs (6 legs)
    for (int i = 0; i < 3; i++) {
        drawLine(surface, cx - 6, cy - 2 + i * 3, cx - 10, cy + i * 2, dark);
        drawLine(surface, cx + 6, cy - 2 + i * 3, cx + 10, cy + i * 2, dark);
    }

    // Eyes
    setPixel(surface, cx - 2, cy - 2, eye);
    setPixel(surface, cx + 2, cy - 2, eye);

    return surface;
}

SDL_Surface* ProceduralSpriteGenerator::generateBirdEnemy(int width, int height) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32,
                                                 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!surface) return nullptr;

    Uint32 transparent = makeColor(surface, 0, 0, 0, 0);
    Uint32 body = makeColor(surface, 255, 200, 50, 255);     // Yellow
    Uint32 wing = makeColor(surface, 255, 150, 0, 255);      // Orange
    Uint32 eye = makeColor(surface, 0, 0, 0, 255);
    Uint32 beak = makeColor(surface, 255, 100, 0, 255);

    SDL_FillRect(surface, nullptr, transparent);

    int cx = width / 2;
    int cy = height / 2;

    // Body
    drawCircle(surface, cx, cy, 6, body);

    // Wings
    drawRect(surface, cx - 10, cy - 2, 4, 6, wing);
    drawRect(surface, cx + 6, cy - 2, 4, 6, wing);

    // Head
    drawCircle(surface, cx, cy - 6, 4, body);

    // Eye
    setPixel(surface, cx, cy - 6, eye);

    // Beak
    drawLine(surface, cx + 2, cy - 6, cx + 5, cy - 6, beak);

    return surface;
}

SDL_Surface* ProceduralSpriteGenerator::generateShadowEnemy(int width, int height) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32,
                                                 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!surface) return nullptr;

    Uint32 transparent = makeColor(surface, 0, 0, 0, 0);
    Uint32 shadow = makeColor(surface, 20, 20, 40, 200);     // Dark translucent
    Uint32 eye = makeColor(surface, 255, 50, 50, 255);       // Glowing red

    SDL_FillRect(surface, nullptr, transparent);

    int cx = width / 2;
    int cy = height / 2;

    // Wispy shadow form
    for (int y = 0; y < height; y++) {
        int waveOffset = static_cast<int>(std::sin(y * 0.3f) * 3);
        int waveWidth = 10 + static_cast<int>(std::sin(y * 0.2f) * 4);
        drawRect(surface, cx - waveWidth / 2 + waveOffset, y, waveWidth, 1, shadow);
    }

    // Glowing eyes
    drawCircle(surface, cx - 3, cy - 4, 2, eye);
    drawCircle(surface, cx + 3, cy - 4, 2, eye);

    return surface;
}

// ============================================================================
// BOSS SPRITES
// ============================================================================

SDL_Surface* ProceduralSpriteGenerator::generateGlitchBoss(int width, int height, int phase) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32,
                                                 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!surface) return nullptr;

    Uint32 transparent = makeColor(surface, 0, 0, 0, 0);
    Uint32 glitch1 = makeColor(surface, 255, 0, 255, 255);   // Magenta
    Uint32 glitch2 = makeColor(surface, 0, 255, 255, 255);   // Cyan
    Uint32 glitch3 = makeColor(surface, 255, 255, 0, 255);   // Yellow
    Uint32 core = makeColor(surface, 255, 255, 255, 255);

    SDL_FillRect(surface, nullptr, transparent);

    int cx = width / 2;
    int cy = height / 2;

    // Core
    drawCircle(surface, cx, cy, 15, core);

    // Glitchy blocks (more intense with phase)
    int glitchCount = 20 + phase * 10;
    for (int i = 0; i < glitchCount; i++) {
        int x = cx + (rand() % 40) - 20;
        int y = cy + (rand() % 40) - 20;
        int size = 2 + rand() % 6;
        Uint32 color = (i % 3 == 0) ? glitch1 : (i % 3 == 1) ? glitch2 : glitch3;
        drawRect(surface, x, y, size, size, color);
    }

    // Evil eyes
    drawRect(surface, cx - 8, cy - 5, 6, 8, glitch1);
    drawRect(surface, cx + 2, cy - 5, 6, 8, glitch2);

    return surface;
}

SDL_Surface* ProceduralSpriteGenerator::generateEchoPrime(int width, int height, int phase) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32,
                                                 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!surface) return nullptr;

    Uint32 transparent = makeColor(surface, 0, 0, 0, 0);
    Uint32 echo = makeColor(surface, 100, 100, 255, 255);    // Blue
    Uint32 dark = makeColor(surface, 50, 50, 150, 255);
    Uint32 eye = makeColor(surface, 255, 0, 0, 255);

    SDL_FillRect(surface, nullptr, transparent);

    int cx = width / 2;
    int cy = height / 2;

    // Main body (humanoid but distorted)
    drawRect(surface, cx - 12, cy - 8, 24, 20, echo);
    drawRect(surface, cx - 10, cy - 6, 20, 16, dark);

    // Head
    drawRect(surface, cx - 10, cy - 20, 20, 12, echo);

    // Multiple eyes (gets more with phase)
    int eyeCount = 2 + phase;
    for (int i = 0; i < eyeCount; i++) {
        int eyeX = cx - 6 + (i * 4);
        int eyeY = cy - 15 + (rand() % 6);
        drawCircle(surface, eyeX, eyeY, 2, eye);
    }

    // Clones/echoes trailing behind
    for (int i = 1; i <= phase; i++) {
        int offset = i * 8;
        Uint32 fadeColor = makeColor(surface, 100, 100, 255, 255 - i * 50);
        drawRect(surface, cx - 8 - offset, cy - 6, 16, 16, fadeColor);
    }

    return surface;
}

SDL_Surface* ProceduralSpriteGenerator::generateCorruptedBoss(int width, int height, int phase) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32,
                                                 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!surface) return nullptr;

    Uint32 transparent = makeColor(surface, 0, 0, 0, 0);
    Uint32 corrupt = makeColor(surface, 200, 0, 0, 255);     // Red
    Uint32 black = makeColor(surface, 0, 0, 0, 255);

    SDL_FillRect(surface, nullptr, transparent);

    int cx = width / 2;
    int cy = height / 2;

    // Corrupted mass
    drawCircle(surface, cx, cy, 20, corrupt);
    drawCircle(surface, cx, cy, 16, black);

    // Tendrils (more with phase)
    for (int i = 0; i < 4 + phase * 2; i++) {
        float angle = (i * 3.14159f * 2.0f) / (4 + phase * 2);
        int endX = cx + static_cast<int>(std::cos(angle) * 25);
        int endY = cy + static_cast<int>(std::sin(angle) * 25);
        drawLine(surface, cx, cy, endX, endY, corrupt);
    }

    return surface;
}

// ============================================================================
// NPC SPRITES
// ============================================================================

SDL_Surface* ProceduralSpriteGenerator::generateNPC(int width, int height, int npcType) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32,
                                                 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!surface) return nullptr;

    Uint32 transparent = makeColor(surface, 0, 0, 0, 0);

    // Different colors for different NPC types
    Uint32 bodyColor;
    switch (npcType) {
        case 0: bodyColor = makeColor(surface, 150, 150, 255, 255); break; // Blue
        case 1: bodyColor = makeColor(surface, 255, 200, 100, 255); break; // Orange
        case 2: bodyColor = makeColor(surface, 200, 100, 255, 255); break; // Purple
        default: bodyColor = makeColor(surface, 150, 150, 150, 255); break;
    }

    Uint32 eye = makeColor(surface, 255, 255, 255, 255);

    SDL_FillRect(surface, nullptr, transparent);

    int cx = width / 2;
    int cy = height / 2;

    // Simple humanoid
    drawRect(surface, cx - 6, cy - 4, 12, 14, bodyColor);
    drawRect(surface, cx - 5, cy - 12, 10, 8, bodyColor);

    // Eyes
    setPixel(surface, cx - 2, cy - 9, eye);
    setPixel(surface, cx + 2, cy - 9, eye);

    // Question mark above head (indicates interaction)
    drawRect(surface, cx - 2, cy - 20, 4, 2, eye);
    setPixel(surface, cx, cy - 18, eye);

    return surface;
}

// ============================================================================
// LEVEL ELEMENTS
// ============================================================================

SDL_Surface* ProceduralSpriteGenerator::generatePlatform(int width, int height) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32,
                                                 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!surface) return nullptr;

    Uint32 transparent = makeColor(surface, 0, 0, 0, 0);
    Uint32 stone = makeColor(surface, 120, 120, 120, 255);
    Uint32 darkStone = makeColor(surface, 80, 80, 80, 255);

    SDL_FillRect(surface, nullptr, transparent);

    // Brick pattern
    for (int y = 0; y < height; y += 8) {
        for (int x = 0; x < width; x += 16) {
            int offset = (y / 8) % 2 == 0 ? 0 : 8;
            drawRect(surface, x + offset, y, 15, 7, stone);
            drawRect(surface, x + offset + 1, y + 1, 13, 5, darkStone);
        }
    }

    return surface;
}

SDL_Surface* ProceduralSpriteGenerator::generateStar(int width, int height) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32,
                                                 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!surface) return nullptr;

    Uint32 transparent = makeColor(surface, 0, 0, 0, 0);
    Uint32 yellow = makeColor(surface, 255, 255, 0, 255);
    Uint32 white = makeColor(surface, 255, 255, 255, 255);

    SDL_FillRect(surface, nullptr, transparent);

    int cx = width / 2;
    int cy = height / 2;

    // Star shape
    drawCircle(surface, cx, cy, 8, yellow);
    drawCircle(surface, cx, cy, 4, white);

    // Rays
    drawLine(surface, cx, cy - 10, cx, cy - 6, yellow);
    drawLine(surface, cx, cy + 6, cx, cy + 10, yellow);
    drawLine(surface, cx - 10, cy, cx - 6, cy, yellow);
    drawLine(surface, cx + 6, cy, cx + 10, cy, yellow);

    return surface;
}

SDL_Surface* ProceduralSpriteGenerator::generateCodeFragment(int width, int height) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32,
                                                 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!surface) return nullptr;

    Uint32 transparent = makeColor(surface, 0, 0, 0, 0);
    Uint32 cyan = makeColor(surface, 0, 255, 255, 255);
    Uint32 darkCyan = makeColor(surface, 0, 150, 150, 255);
    Uint32 white = makeColor(surface, 255, 255, 255, 255);

    SDL_FillRect(surface, nullptr, transparent);

    int cx = width / 2;
    int cy = height / 2;

    // Document/paper shape
    drawRect(surface, cx - 8, cy - 10, 16, 20, cyan);
    drawRect(surface, cx - 7, cy - 9, 14, 18, darkCyan);

    // "Code" lines
    for (int i = 0; i < 5; i++) {
        drawLine(surface, cx - 5, cy - 6 + i * 3, cx + 5, cy - 6 + i * 3, white);
    }

    return surface;
}

SDL_Surface* ProceduralSpriteGenerator::generateGoalPortal(int width, int height) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32,
                                                 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!surface) return nullptr;

    Uint32 transparent = makeColor(surface, 0, 0, 0, 0);
    Uint32 purple = makeColor(surface, 200, 0, 255, 255);
    Uint32 darkPurple = makeColor(surface, 100, 0, 150, 255);
    Uint32 white = makeColor(surface, 255, 255, 255, 200);

    SDL_FillRect(surface, nullptr, transparent);

    int cx = width / 2;
    int cy = height / 2;

    // Portal rings
    for (int r = 20; r > 0; r -= 4) {
        Uint32 color = (r % 8 == 0) ? purple : darkPurple;
        drawCircle(surface, cx, cy, r, color);
    }

    // Swirls
    for (int angle = 0; angle < 360; angle += 45) {
        float rad = angle * 3.14159f / 180.0f;
        int x = cx + static_cast<int>(std::cos(rad) * 12);
        int y = cy + static_cast<int>(std::sin(rad) * 12);
        drawCircle(surface, x, y, 2, white);
    }

    return surface;
}

// ============================================================================
// UI ELEMENTS
// ============================================================================

SDL_Surface* ProceduralSpriteGenerator::generateHeart(int width, int height) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32,
                                                 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!surface) return nullptr;

    Uint32 transparent = makeColor(surface, 0, 0, 0, 0);
    Uint32 red = makeColor(surface, 255, 0, 0, 255);

    SDL_FillRect(surface, nullptr, transparent);

    int cx = width / 2;
    int cy = height / 2;

    // Heart shape (simplified)
    drawCircle(surface, cx - 3, cy - 2, 4, red);
    drawCircle(surface, cx + 3, cy - 2, 4, red);
    drawLine(surface, cx - 6, cy, cx, cy + 8, red);
    drawLine(surface, cx + 6, cy, cx, cy + 8, red);

    // Fill
    for (int y = cy - 2; y < cy + 6; y++) {
        int halfWidth = 6 - (y - cy);
        drawRect(surface, cx - halfWidth, y, halfWidth * 2, 1, red);
    }

    return surface;
}

SDL_Surface* ProceduralSpriteGenerator::generateSkull(int width, int height) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32,
                                                 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!surface) return nullptr;

    Uint32 transparent = makeColor(surface, 0, 0, 0, 0);
    Uint32 white = makeColor(surface, 255, 255, 255, 255);
    Uint32 black = makeColor(surface, 0, 0, 0, 255);

    SDL_FillRect(surface, nullptr, transparent);

    int cx = width / 2;
    int cy = height / 2;

    // Skull
    drawCircle(surface, cx, cy - 2, 8, white);
    drawRect(surface, cx - 6, cy + 4, 12, 6, white);

    // Eyes
    drawCircle(surface, cx - 3, cy - 3, 2, black);
    drawCircle(surface, cx + 3, cy - 3, 2, black);

    // Nose
    setPixel(surface, cx, cy + 1, black);

    // Teeth
    for (int i = 0; i < 6; i++) {
        setPixel(surface, cx - 3 + i, cy + 5, black);
    }

    return surface;
}
