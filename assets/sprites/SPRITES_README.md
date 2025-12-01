# 🎨 ECHOES Sprite System

## Overview

ECHOES verwendet ein **Procedural Sprite Generation System**, das alle Sprites zur Laufzeit aus Code generiert.

### Warum Procedural Sprites?

1. **Keine Bilddateien nötig** - Alles wird im Code generiert
2. **Perfekt für Horror** - Glitchy, minimalist, corrupted aesthetic
3. **Klein & Effizient** - Keine großen Asset-Files
4. **Dynamisch** - Sprites können zur Laufzeit modifiziert werden

## System Architecture

### TextureManager
- Zentrales Texture-Management-System
- Lädt und verwaltet alle Texturen
- Unterstützt sowohl File-based als auch Procedural Textures

### ProceduralSpriteGenerator
- Generiert Pixel-Art Sprites zur Laufzeit
- Erstellt SDL_Surface Objekte die dann zu Texturen konvertiert werden
- Alle Sprites im minimalistischen Horror-Stil

## Available Sprites

### Player Sprites
- `player_idle` - Idle stance
- `player_walk1` - Walk animation frame 1
- `player_walk2` - Walk animation frame 2
- `player_jump` - Jumping pose
- `player_dead` - Death animation

### Enemy Sprites
- `enemy_bug` - Purple bug enemy (6 legs, red eyes)
- `enemy_bird` - Yellow bird enemy (flying)
- `enemy_shadow` - Dark shadow enemy (wispy, translucent)

### Boss Sprites
- `boss_glitch_p0/p1/p2/p3` - GlitchBoss (4 phases, increasingly glitchy)
- `boss_echo_p0/p1/p2/p3` - Echo Prime (4 phases, multiple eyes, clones)
- `boss_corrupted_p0/p1/p2/p3` - Corrupted Boss (4 phases, tendrils)

### NPC Sprites
- `npc_0` - Blue NPC
- `npc_1` - Orange NPC
- `npc_2` - Purple NPC

### Level Elements
- `platform` - Stone brick platform
- `star` - Collectible star (yellow, glowing)
- `code_fragment` - Code fragment collectible (cyan document)
- `goal_portal` - Level exit portal (purple swirl)

### UI Elements
- `ui_heart` - Health heart
- `ui_skull` - Death skull

## Integration in Game.cpp

```cpp
// Initialize TextureManager
m_textureManager = std::make_unique<TextureManager>(m_renderer);

// Generate all procedural sprites
m_textureManager->createProceduralTexture("player_idle", 32, 32,
    ProceduralSpriteGenerator::generatePlayerIdle);

m_textureManager->createProceduralTexture("enemy_bug", 32, 32,
    ProceduralSpriteGenerator::generateBugEnemy);

// ... etc for all sprites
```

## Custom Sprites (Optional)

If you want to replace procedural sprites with custom PNG images:

1. Create PNG files in `/assets/sprites/`
2. Use `loadTexture()` instead of `createProceduralTexture()`:

```cpp
m_textureManager->loadTexture("player_idle", "assets/sprites/player_idle.png");
```

## Sprite Generation Tools

If you want to create custom PNG sprites, recommended tools:

- **Aseprite** - Professional pixel art tool
- **Piskel** - Free online pixel art editor
- **GIMP** - Free image editor
- **Krita** - Free digital painting tool

### Recommended Sprite Sizes
- Player: 32x32
- Enemies: 32x32 (small) to 48x48 (large)
- Bosses: 64x64 to 128x128
- NPCs: 32x32
- Level Elements: Variable (8x8 to 64x64)
- UI Elements: 16x16 to 32x32

## Horror Aesthetic Guidelines

For the best horror game aesthetic, keep sprites:

✅ **Minimalist** - Simple shapes, limited details
✅ **High Contrast** - Dark colors with bright accents
✅ **Glitchy** - Corrupted pixels, offset layers
✅ **Unsettling** - Unnatural proportions, too many eyes
✅ **Monochrome with Splashes** - Mostly grayscale with red/cyan/magenta accents

❌ Avoid cute, rounded, colorful sprites
❌ Avoid realistic details
❌ Avoid smooth gradients

## Performance

- Procedural sprites are generated ONCE at startup
- Converted to GPU textures for fast rendering
- No performance impact during gameplay
- Total generation time: ~10-50ms on modern hardware

## Future Enhancements

Potential additions:
- Animation frames (walk cycles, idle animations)
- Particle effects (glitch particles, corruption effects)
- Dynamic sprite corruption (sprites that change with corruption level)
- Boss transformation animations
- NPC portrait sprites for dialogues

---

**Created by Claude - ECHOES Horror Platformer**
