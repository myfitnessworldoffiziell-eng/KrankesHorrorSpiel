# 🎨 ECHOES Texture System Implementation Guide

## System Overview

Das ECHOES Texture System besteht aus zwei Hauptkomponenten:

1. **TextureManager** - Zentrales Texture-Management
2. **ProceduralSpriteGenerator** - Runtime Sprite-Generierung

## 📦 Files Created

### Core System
- `src/TextureManager.h` - Header für Texture Manager
- `src/TextureManager.cpp` - Implementation (150+ lines)
- `src/ProceduralSpriteGenerator.h` - Header für Sprite Generator
- `src/ProceduralSpriteGenerator.cpp` - Implementation (650+ lines)

### Documentation
- `assets/sprites/SPRITES_README.md` - Sprite System Dokumentation

### Build System
- `CMakeLists.txt` - Updated mit neuen Source-Files

## 🎮 Usage Example

### In Game.cpp Initialization:

```cpp
#include "TextureManager.h"
#include "ProceduralSpriteGenerator.h"

// In Game::initialize():
m_textureManager = std::make_unique<TextureManager>(m_renderer);

// Generate all player sprites
m_textureManager->createProceduralTexture("player_idle", 32, 32,
    ProceduralSpriteGenerator::generatePlayerIdle);
m_textureManager->createProceduralTexture("player_jump", 32, 32,
    ProceduralSpriteGenerator::generatePlayerJump);
m_textureManager->createProceduralTexture("player_walk1", 32, 32,
    ProceduralSpriteGenerator::generatePlayerWalk1);
m_textureManager->createProceduralTexture("player_walk2", 32, 32,
    ProceduralSpriteGenerator::generatePlayerWalk2);
m_textureManager->createProceduralTexture("player_dead", 32, 32,
    ProceduralSpriteGenerator::generatePlayerDead);

// Generate enemy sprites
m_textureManager->createProceduralTexture("enemy_bug", 32, 32,
    ProceduralSpriteGenerator::generateBugEnemy);
m_textureManager->createProceduralTexture("enemy_bird", 32, 32,
    ProceduralSpriteGenerator::generateBirdEnemy);
m_textureManager->createProceduralTexture("enemy_shadow", 32, 32,
    ProceduralSpriteGenerator::generateShadowEnemy);

// Generate boss sprites (all phases)
for (int phase = 0; phase < 4; phase++) {
    std::string id = "boss_glitch_p" + std::to_string(phase);
    m_textureManager->createProceduralTexture(id, 64, 64,
        [phase](int w, int h) {
            return ProceduralSpriteGenerator::generateGlitchBoss(w, h, phase);
        });
}

// Generate level elements
m_textureManager->createProceduralTexture("platform", 64, 16,
    ProceduralSpriteGenerator::generatePlatform);
m_textureManager->createProceduralTexture("star", 24, 24,
    ProceduralSpriteGenerator::generateStar);
m_textureManager->createProceduralTexture("code_fragment", 24, 24,
    ProceduralSpriteGenerator::generateCodeFragment);
m_textureManager->createProceduralTexture("goal_portal", 48, 48,
    ProceduralSpriteGenerator::generateGoalPortal);
```

### In Player.cpp Rendering:

```cpp
// OLD (rectangles):
void Player::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 100, 255);
    SDL_Rect rect = getBounds();
    SDL_RenderFillRect(renderer, &rect);
}

// NEW (textures):
void Player::render(SDL_Renderer* renderer, TextureManager* textures) {
    SDL_Rect dstRect = getBounds();

    // Choose sprite based on state
    std::string spriteId;
    if (!isAlive()) {
        spriteId = "player_dead";
    } else if (m_velocityY != 0) {
        spriteId = "player_jump";
    } else if (m_velocityX != 0) {
        // Alternate walk frames
        spriteId = (static_cast<int>(m_animTimer * 10) % 2 == 0) ?
                   "player_walk1" : "player_walk2";
    } else {
        spriteId = "player_idle";
    }

    textures->render(spriteId, nullptr, &dstRect);
}
```

### In Enemy.cpp Rendering:

```cpp
// OLD:
void Enemy::render(SDL_Renderer* renderer) {
    SDL_Rect rect = getBounds();
    switch (m_type) {
        case EnemyType::BUG:
            SDL_SetRenderDrawColor(renderer, 150, 50, 200, 255);
            break;
        // ...
    }
    SDL_RenderFillRect(renderer, &rect);
}

// NEW:
void Enemy::render(SDL_Renderer* renderer, TextureManager* textures) {
    SDL_Rect dstRect = getBounds();

    std::string spriteId;
    switch (m_type) {
        case EnemyType::BUG: spriteId = "enemy_bug"; break;
        case EnemyType::BIRD: spriteId = "enemy_bird"; break;
        case EnemyType::SHADOW: spriteId = "enemy_shadow"; break;
    }

    textures->render(spriteId, nullptr, &dstRect);
}
```

## 🔄 Migration Steps

### Step 1: Add TextureManager to Game.h

```cpp
// Forward declaration
class TextureManager;

// In private members:
std::unique_ptr<TextureManager> m_textureManager;
```

### Step 2: Initialize in Game.cpp

```cpp
// After renderer creation:
m_textureManager = std::make_unique<TextureManager>(m_renderer);

// Generate all sprites (see examples above)
```

### Step 3: Pass TextureManager to Subsystems

```cpp
// In Game.cpp initialization:
m_player = std::make_unique<Player>();
m_player->setTextureManager(m_textureManager.get());

m_level = std::make_unique<Level>();
m_level->setTextureManager(m_textureManager.get());
```

### Step 4: Update render() Methods

Add TextureManager* parameter to all render methods:

```cpp
// Player.h
void render(SDL_Renderer* renderer, TextureManager* textures);

// Enemy.h
void render(SDL_Renderer* renderer, TextureManager* textures);

// Boss.h
void render(SDL_Renderer* renderer, TextureManager* textures);

// Level.h
void render(SDL_Renderer* renderer, TextureManager* textures);
```

### Step 5: Replace SDL_RenderFillRect with Texture Rendering

See examples above for each class.

## ⚡ Performance Notes

- **Sprite Generation**: ~10-50ms at startup (one-time cost)
- **Rendering**: GPU-accelerated textures (faster than SDL_RenderFillRect)
- **Memory**: ~100-200KB for all sprites (minimal)
- **No file I/O**: No disk reads during gameplay

## 🎨 Available Sprites

### Player (5 sprites)
- player_idle, player_walk1, player_walk2, player_jump, player_dead

### Enemies (3 sprites)
- enemy_bug, enemy_bird, enemy_shadow

### Bosses (12 sprites - 3 bosses × 4 phases)
- boss_glitch_p0/p1/p2/p3
- boss_echo_p0/p1/p2/p3
- boss_corrupted_p0/p1/p2/p3

### NPCs (3 sprites)
- npc_0, npc_1, npc_2

### Level Elements (4 sprites)
- platform, star, code_fragment, goal_portal

### UI Elements (2 sprites)
- ui_heart, ui_skull

## 🔧 Customization

### Adding New Sprites

1. Add generator function to ProceduralSpriteGenerator.cpp:

```cpp
SDL_Surface* ProceduralSpriteGenerator::generateMyNewSprite(int width, int height) {
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32,
                                                 0xFF000000, 0x00FF0000,
                                                 0x0000FF00, 0x000000FF);
    // Draw your sprite using helper functions:
    // drawRect(), drawCircle(), drawLine(), setPixel()

    return surface;
}
```

2. Add declaration to ProceduralSpriteGenerator.h

3. Load in Game::initialize():

```cpp
m_textureManager->createProceduralTexture("my_new_sprite", 32, 32,
    ProceduralSpriteGenerator::generateMyNewSprite);
```

### Using External PNG Files

If you prefer PNG files over procedural generation:

```cpp
// Instead of createProceduralTexture:
m_textureManager->loadTexture("player_idle", "assets/sprites/player_idle.png");
```

## 🐛 Debugging

### Check if texture loaded:

```cpp
if (!m_textureManager->hasTexture("player_idle")) {
    std::cerr << "Failed to load player_idle texture!" << std::endl;
}
```

### Verify texture rendering:

```cpp
SDL_Texture* tex = m_textureManager->getTexture("player_idle");
if (tex) {
    int w, h;
    SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
    std::cout << "Texture size: " << w << "x" << h << std::endl;
}
```

## 📊 Status

✅ **TextureManager** - Fully implemented (150+ lines)
✅ **ProceduralSpriteGenerator** - Fully implemented (650+ lines)
✅ **CMakeLists.txt** - Updated
✅ **Game.h** - TextureManager integrated
✅ **Documentation** - Complete

⏳ **Pending:**
- Update Player.cpp/h rendering
- Update Enemy.cpp/h rendering
- Update Boss classes rendering
- Update Level.cpp rendering
- Full integration testing

## 🚀 Next Steps

1. ✅ System erstellt und dokumentiert
2. ⏳ Integration in Game.cpp (sprite loading)
3. ⏳ Update aller render() Methoden
4. ⏳ Testing und Debugging
5. ⏳ Optional: Animation system für smooth transitions

---

**System Ready for Integration! 🎮✨**
