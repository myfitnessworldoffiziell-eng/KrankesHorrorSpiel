# 🎮 Settings Menu Integration Guide

## Overview

Complete Settings Menu system with the following features:
- **Music Volume** (0-100%)
- **SFX Volume** (0-100%)
- **Fullscreen Toggle** (ON/OFF)
- **Resolution Selection** (800x600, 1024x768, 1280x720, 1920x1080)
- **Controls Display** (shows all game controls)
- **Reset to Defaults**

## Files Created

- `src/SettingsMenu.h` - Header with all settings options
- `src/SettingsMenu.cpp` - Full implementation (350+ lines)
- `SETTINGS_INTEGRATION.md` - This guide

## Files Modified

- `src/GameState.h` - Added SETTINGS state
- `src/Game.h` - Added SettingsMenu forward declaration, member variable, update/render methods
- `CMakeLists.txt` - Added SettingsMenu to build

## Integration into Game.cpp

### 1. Include Header

```cpp
#include "SettingsMenu.h"
```

### 2. Initialize in Game::initialize()

```cpp
// After AudioManager initialization
m_settingsMenu = std::make_unique<SettingsMenu>(m_audioManager.get());

std::cout << "✓ Settings Menu initialized" << std::endl;
```

### 3. Add to handleEvents() switch

```cpp
case GameState::SETTINGS:
    m_settingsMenu->handleInput(event);
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        // Go back to menu
        setState(GameState::MAIN_MENU);
    }
    break;
```

### 4. Add to update() switch

```cpp
case GameState::SETTINGS:
    updateSettings(deltaTime);
    break;
```

### 5. Add to render() switch

```cpp
case GameState::SETTINGS:
    renderSettings();
    break;
```

### 6. Implement updateSettings()

```cpp
void Game::updateSettings(float deltaTime) {
    auto result = m_settingsMenu->update(deltaTime);

    switch (result) {
        case SettingsMenu::SettingsResult::BACK_TO_MENU:
            setState(GameState::MAIN_MENU);
            break;

        case SettingsMenu::SettingsResult::APPLY_SETTINGS:
            // Apply fullscreen setting
            if (m_settingsMenu->isFullscreen() != m_isFullscreen) {
                m_isFullscreen = m_settingsMenu->isFullscreen();
                SDL_SetWindowFullscreen(m_window,
                    m_isFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
                std::cout << "[Game] Fullscreen: " << (m_isFullscreen ? "ON" : "OFF") << std::endl;
            }

            // Resolution change (requires restart)
            int resIndex = m_settingsMenu->getResolutionIndex();
            // Store for next restart
            break;

        default:
            break;
    }
}
```

### 7. Implement renderSettings()

```cpp
void Game::renderSettings() {
    // Optional: render game in background (dimmed)
    // or just clear to black
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    m_settingsMenu->render(m_renderer);
}
```

### 8. Add to setState() switch

```cpp
case GameState::SETTINGS:
    std::cout << "SETTINGS";
    break;
```

## MainMenu Integration

Update `MainMenu.cpp` to include Settings option:

```cpp
// In MainMenu constructor or initialize():
m_options = {
    "Start Game",
    "Settings",    // NEW
    "Quit"
};

// In selectOption():
case 1: return MenuResult::SETTINGS;  // NEW
case 2: return MenuResult::QUIT;      // was case 1
```

Update `MainMenu.h` enum:

```cpp
enum class MenuResult {
    NONE,
    START_GAME,
    SETTINGS,    // NEW
    QUIT
};
```

Update `Game::updateMainMenu()`:

```cpp
case MainMenu::MenuResult::SETTINGS:
    setState(GameState::SETTINGS);
    break;
```

## PauseMenu Integration (Optional)

Add Settings option to Pause Menu as well:

```cpp
// In PauseMenu:
m_options = {
    "Resume",
    "Settings",    // NEW
    "Main Menu",
    "Quit"
};
```

## Controls Display

The Settings Menu includes a **Controls** screen that shows:

### Movement
- Arrow Keys / WASD - Move
- SPACE - Jump

### Interaction
- E - Talk to NPCs
- ENTER - Advance dialog

### Menu
- ESC - Pause / Back

### Debug (Optional)
- G - Trigger Glitch
- F - Create File
- W - Window Manipulation
- J - Jumpscare
- B - Fake BSOD
- F11 - Toggle Fullscreen

## Settings Persistence (Future Enhancement)

To save settings between sessions, add:

```cpp
// In Game::shutdown():
void Game::shutdown() {
    // Save settings to file
    std::ofstream settingsFile("settings.cfg");
    settingsFile << "music_volume=" << m_settingsMenu->getMusicVolume() << "\n";
    settingsFile << "sfx_volume=" << m_settingsMenu->getSFXVolume() << "\n";
    settingsFile << "fullscreen=" << m_settingsMenu->isFullscreen() << "\n";
    settingsFile << "resolution=" << m_settingsMenu->getResolutionIndex() << "\n";
    settingsFile.close();
}

// In Game::initialize():
bool Game::initialize() {
    // ... SDL init ...

    // Load settings from file
    std::ifstream settingsFile("settings.cfg");
    if (settingsFile.is_open()) {
        std::string line;
        while (std::getline(settingsFile, line)) {
            // Parse and apply settings
            // music_volume=70
            // sfx_volume=80
            // etc.
        }
        settingsFile.close();
    }
}
```

## UI/UX Features

- **Navigation**: Arrow keys or WASD
- **Adjust Settings**: Left/Right arrows
- **Visual Feedback**:
  - Pulsing highlight on selected option
  - Volume bars for audio settings
  - Real-time audio preview when adjusting SFX volume
  - Color-coded text (selected=white, normal=gray, values=green)

## Audio Integration

Settings automatically integrate with AudioManager:

```cpp
// When music volume changes:
m_settingsMenu->setMusicVolume(newVolume);
// -> Calls m_audioManager->setMusicVolume(newVolume) internally

// When SFX volume changes:
m_settingsMenu->setSFXVolume(newVolume);
// -> Calls m_audioManager->setSFXVolume(newVolume)
// -> Plays test sound at new volume
```

## Keyboard Shortcuts

- **Arrow Keys/WASD**: Navigate options
- **Left/Right**: Adjust values
- **Enter/Space**: Select current option
- **ESC**: Back to menu

## Testing

1. Launch game
2. Navigate to Settings from Main Menu
3. Test each option:
   - Adjust music volume → Hear music change
   - Adjust SFX volume → Hear test sound
   - Toggle fullscreen → Window changes
   - Change resolution → Note displayed
   - Show controls → Display all controls
   - Reset defaults → All values reset to defaults
4. ESC back to menu
5. Verify settings persisted

## Future Enhancements

Possible additions:
- ✓ Settings persistence (save/load)
- ✓ Key rebinding
- ✓ Graphics quality options
- ✓ Difficulty selection
- ✓ Language selection
- ✓ Accessibility options (colorblind mode, subtitles)

---

**Settings Menu Complete & Ready to Use!** 🎮⚙️
