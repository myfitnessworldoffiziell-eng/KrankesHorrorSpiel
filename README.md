# 👻 ECHOES.exe - Meta Horror Platformer

<div align="center">

![Game Genre](https://img.shields.io/badge/Genre-Horror%20Platformer-red)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-blue)
![Language](https://img.shields.io/badge/Language-C%2B%2B17-orange)

**Ein psychologisches Horror-Game im Stil von Sonic.exe, das die "vierte Wand" durchbricht.**

[📥 Download für Spieler](#-download-für-spieler) • [🔨 Build für Entwickler](#-build)

</div>

---

## ⚡ Current Status: VOLLSTÄNDIG SPIELBAR!

Das Spiel ist **komplett fertig** mit:
- ✅ **10 vollständige Level** mit progressiver Horror-Intensität
- ✅ **3 epische Boss-Kämpfe** (Level 6: GlitchBoss, Level 8: EchoPrime, Level 10: Finale)
- ✅ **Vollständiges Platformer-Gameplay** (Jump, Gravity, Collision, Combat)
- ✅ **Vollständiges Settings-Menü** (Musik/SFX-Lautstärke, Fullscreen, Resolution)
- ✅ **30+ Procedural Sprites** via ProceduralSpriteGenerator
- ✅ **14 Audio-Dateien** (4 Musik-Tracks, 10 SFX)
- ✅ **ExtremePCHorror System** mit 15+ PC-Takeover-Effekten
- ✅ **Boss-Defeat-Horror-Sequenzen**
- ✅ **Health System, Death/Respawn, Collectibles**
- ✅ **Meta-Horror Features** (File Creation, Window Manipulation, Fake BSOD)
- ✅ **Multi-Platform Support** (Windows, Linux, macOS)

---

## 📥 Download für Spieler

### **Einfachste Methode** (Keine Installation nötig!):

1. Gehe zu [**Releases**](https://github.com/myfitnessworldoffiziell-eng/KrankesHorrorSpiel/releases/latest)
2. Download für dein System:
   - 🪟 **Windows**: `ECHOES-Windows-x64.zip` → Entpacken → `ECHOES.exe` starten
   - 🐧 **Linux**: `ECHOES-Linux-x64.tar.gz` → Entpacken → `./ECHOES` ausführen
   - 🍎 **macOS**: `ECHOES-macOS-x64.tar.gz` → Entpacken → `./ECHOES` ausführen
3. **Spielen!** 🎮

**Keine Kompilierung nötig** - einfach runterladen und losspielen!

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

### ✅ Phase 3 - Gameplay Systems (FERTIG!)
- [x] Audio-Dateien (11 Generated Sounds) - siehe `assets/audio/`
- [x] Enemy System (4 enemy types: Green Slime, Blue Bird, Glitch Slime, Spike Ball)
- [x] Player Health & Death System
- [x] Collision Detection (Player vs Enemies, jump-on mechanic)
- [x] Collectibles System (Stars)
- [x] Level System (3 levels implemented)
  - [x] Level 1: Welcome to Paradise (2 slimes, 1 bird, 5 stars)
  - [x] Level 2: Sunny Meadows (3 slimes, 2 birds, 6 stars)
  - [x] Level 3: Glitched Meadows (first corruption!)
- [x] In-Game UI (Health bar, Star counter, Level indicator)
- [x] Death/Respawn System

### ✅ Phase 3.5 - Advanced Content (FERTIG!)
- [x] Levels 4-10 (Boss fights, corrupted levels, finale)
- [x] NPC-System with dialogue
- [x] Boss AI (GlitchBoss, EchoPrime, Final Boss)
- [x] Advanced platforming mechanics
- [x] Settings Menu (Volume, Fullscreen, Resolution)
- [x] Procedural Sprite Generation (30+ sprites)
- [x] ExtremePCHorror System (15+ effects)
- [x] Boss-Defeat-Horror-Sequenzen

### ✅ Phase 4 - Distribution (FERTIG!)
- [x] Multi-Platform Support (Windows, Linux, macOS)
- [x] GitHub Actions CI/CD Pipeline
- [x] Automatic Release Builds
- [x] Downloadable .exe für Windows
- [x] Installation Guides
- [x] Comprehensive README

### 📋 Phase 5 - Future Ideas (Optional)
- [ ] Kamera-Integration (OpenCV) - optional
- [ ] Mikrofon-Integration (PortAudio) - optional
- [ ] Multiple Endings - optional
- [ ] Steam/Itch.io Release - optional
- [ ] Speedrun Mode - optional

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
