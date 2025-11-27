# ECHOES.exe - Meta Horror Platformer

Ein psychologisches Horror-Game im Stil von Sonic.exe, das die "vierte Wand" durchbricht.

## 🎮 Konzept

ECHOES startet als niedliches Retro-Platformer-Spiel, transformiert sich aber langsam in einen Meta-Horror-Albtraum. Das Spiel:

- Manipuliert Dateien im Downloads-Ordner
- Fragt (mit Zustimmung) nach Kamera/Mikrofon-Zugriff
- Manipuliert das Spielfenster
- Merkt sich deine Aktionen zwischen Sessions
- Reagiert auf dein Verhalten

## 🛠️ Tech Stack

- **C++17**
- **SDL2** (Graphics, Audio, Input)
- **SDL2_image** (Textures)
- **SDL2_mixer** (Audio/Music)
- **SDL2_ttf** (Text)
- **OpenCV** (optional, für Kamera)

## 📦 Installation

### Dependencies (Ubuntu/Debian)

```bash
sudo apt-get install build-essential cmake
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
sudo apt-get install libopencv-dev  # Optional, für Kamera-Support
```

### Dependencies (Windows mit vcpkg)

```cmd
vcpkg install sdl2 sdl2-image sdl2-mixer sdl2-ttf
vcpkg install opencv  # Optional
```

### Dependencies (macOS)

```bash
brew install cmake sdl2 sdl2_image sdl2_mixer sdl2_ttf
brew install opencv  # Optional
```

## 🔨 Build

```bash
mkdir build
cd build
cmake ..
make
```

## 🎯 Spielen

```bash
./ECHOES
```

## ⌨️ Controls

### Gameplay
- **Arrow Keys / WASD** - Bewegung
- **SPACE** - Springen / Dialog fortsetzen
- **ESC** - Pause / Zur\u00fcck
- **F11** - Fullscreen Toggle

### Debug-Keys (während Spielen)
- **G** - Glitch-Effekt triggern
- **F** - Creepy Datei erstellen
- **W** - Fenster manipulieren
- **J** - Jumpscare triggern
- **B** - Fake Blue Screen

## ⚠️ Ethische Grenzen

### Was das Spiel DARF ✅

- Dateien im eigenen Spielordner erstellen
- Dateien im Downloads-Ordner erstellen
- Kamera/Mikrofon mit **klarer Zustimmung** nutzen
- Fenster manipulieren (aber immer schließbar)
- Harmlose System-Info lesen (Username, Zeit)

### Was das Spiel NICHT tut ❌

- Echte System-Dateien ändern
- Daten ohne Permission manipulieren
- Netzwerk-Aktivität ohne Warnung
- Permanent den PC verändern
- Privatsphäre verletzen

## 📁 Projektstruktur

```
ECHOES/
├── src/
│   ├── main.cpp                 # Entry Point
│   ├── Game.cpp/.h              # Main Game Loop
│   ├── MetaHorror.cpp/.h        # Datei/Window Manipulation
│   ├── PermissionManager.cpp/.h # Consent System
│   ├── CorruptionSystem.cpp/.h  # Visual Glitches
│   ├── Player.cpp/.h            # Echo Character
│   └── Level.cpp/.h             # Level Logic
├── assets/                      # (future) Sprites, Audio, etc.
├── data/                        # Save data, secrets
├── CMakeLists.txt
└── README.md
```

## 🎨 Story

Du findest ein "verlorenes" Spiel aus den 90ern: "ECHOES - The Forgotten Platformer".

Was als niedliches Platformer-Game beginnt, entwickelt sich zu einem Meta-Horror-Erlebnis. Das Spiel "erwacht", manipuliert Dateien, beobachtet dich (mit Zustimmung), und die Grenzen zwischen Spiel und Realität verschwimmen...

**Mehrere Endings basierend auf deinen Entscheidungen:**
- Standard Ending: "The Cycle Continues"
- Kamera/Mikrofon Ending: "Digital Ghost"
- True Ending: "The Developer's Escape" (versteckt)

## 🔧 Development Roadmap

### ✅ Phase 1 - Foundation (FERTIG!)
- [x] Core Engine Setup
- [x] Basic Game Loop
- [x] Meta-Horror File System
- [x] Permission Manager
- [x] Visual Corruption Effects
- [x] Simple Platformer Mechanics

### ✅ Phase 2 - UI & Audio (FERTIG!)
- [x] Main Menu System
- [x] Pause Menu
- [x] Dialog/Textbox System
- [x] Audio Manager (SDL_mixer)
- [x] Game State Management
- [x] Jumpscare System
- [x] Fake Blue Screen of Death

### 🚧 Phase 3 - Content (In Arbeit)
- [ ] Audio-Dateien (Free Sounds) - siehe `assets/audio/AUDIO_DOWNLOAD.md`
- [ ] Erweiterte Level (10 Level)
- [ ] Story-Sequenzen/Cutscenes
- [ ] NPC-System
- [ ] Enemy AI
- [ ] Boss Fights

### 📋 Phase 4 - Advanced (Geplant)
- [ ] Kamera-Integration (OpenCV)
- [ ] Mikrofon-Integration (PortAudio)
- [ ] Multiple Endings
- [ ] Persistent Save System (JSON)
- [ ] Sprites & Visuals (Pixel Art)
- [ ] Particle System

## 📝 Lizenz

Dieses Projekt ist für Bildungszwecke gedacht. Bitte verwende es verantwortungsvoll!

## 🙏 Credits

Inspiriert von:
- Sonic.exe
- Doki Doki Literature Club
- Pony Island
- Undertale

---

**"Every player creates a new Echo..."**
