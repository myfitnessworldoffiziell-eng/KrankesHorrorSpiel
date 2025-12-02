#pragma once
#include "SDL_Platform.h"


/**
 * Procedural Sprite Generator
 *
 * Generates pixel-art style sprites at runtime.
 * Perfect for horror game aesthetic - glitchy, corrupted, minimalist style.
 */
class ProceduralSpriteGenerator {
public:
    // ===== PLAYER SPRITES =====
    static SDL_Surface* generatePlayerIdle(int width, int height);
    static SDL_Surface* generatePlayerWalk1(int width, int height);
    static SDL_Surface* generatePlayerWalk2(int width, int height);
    static SDL_Surface* generatePlayerJump(int width, int height);
    static SDL_Surface* generatePlayerDead(int width, int height);

    // ===== ENEMY SPRITES =====
    static SDL_Surface* generateBugEnemy(int width, int height);
    static SDL_Surface* generateBirdEnemy(int width, int height);
    static SDL_Surface* generateShadowEnemy(int width, int height);

    // ===== BOSS SPRITES =====
    static SDL_Surface* generateGlitchBoss(int width, int height, int phase);
    static SDL_Surface* generateEchoPrime(int width, int height, int phase);
    static SDL_Surface* generateCorruptedBoss(int width, int height, int phase);

    // ===== NPC SPRITES =====
    static SDL_Surface* generateNPC(int width, int height, int npcType);

    // ===== LEVEL ELEMENTS =====
    static SDL_Surface* generatePlatform(int width, int height);
    static SDL_Surface* generateStar(int width, int height);
    static SDL_Surface* generateCodeFragment(int width, int height);
    static SDL_Surface* generateGoalPortal(int width, int height);

    // ===== UI ELEMENTS =====
    static SDL_Surface* generateHeart(int width, int height);
    static SDL_Surface* generateSkull(int width, int height);

private:
    // Helper functions
    static void setPixel(SDL_Surface* surface, int x, int y, Uint32 color);
    static Uint32 getPixel(SDL_Surface* surface, int x, int y);
    static void drawRect(SDL_Surface* surface, int x, int y, int w, int h, Uint32 color);
    static void drawCircle(SDL_Surface* surface, int cx, int cy, int radius, Uint32 color);
    static void drawLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint32 color);
    static Uint32 makeColor(SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
};
