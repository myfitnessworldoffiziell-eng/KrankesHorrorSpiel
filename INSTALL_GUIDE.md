# 🎮 ECHOES.exe - Installation & Test Guide

## Systemanforderungen
- Linux, macOS oder Windows
- CMake 3.15+
- C++17 Compiler (GCC 7+, Clang 5+, MSVC 2017+)
- SDL2, SDL2_image, SDL2_mixer, SDL2_ttf

---

## 📦 Schritt 1: Dependencies installieren

### Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install -y \
    cmake \
    build-essential \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-mixer-dev \
    libsdl2-ttf-dev
```

### Fedora/RHEL:
```bash
sudo dnf install -y \
    cmake \
    gcc-c++ \
    SDL2-devel \
    SDL2_image-devel \
    SDL2_mixer-devel \
    SDL2_ttf-devel
```

### Arch Linux:
```bash
sudo pacman -S \
    cmake \
    base-devel \
    sdl2 \
    sdl2_image \
    sdl2_mixer \
    sdl2_ttf
```

### macOS (mit Homebrew):
```bash
brew install cmake sdl2 sdl2_image sdl2_mixer sdl2_ttf
```

### Windows (mit vcpkg):
```bash
vcpkg install sdl2 sdl2-image sdl2-mixer sdl2-ttf
```

---

## 🔨 Schritt 2: Projekt bauen

### Im Projektverzeichnis:

```bash
# Build-Verzeichnis erstellen
mkdir -p build
cd build

# CMake konfigurieren
cmake ..

# Projekt kompilieren (nutzt alle CPU-Kerne)
cmake --build . -j$(nproc)

# Zurück zum Hauptverzeichnis
cd ..
```

### Erwartete Ausgabe:
```
-- Configuring done
-- Generating done
-- Build files have been written to: /home/user/KrankesHorrorSpiel/build
[ 98%] Building CXX object CMakeFiles/ECHOES.dir/src/Game.cpp.o
[100%] Linking CXX executable ECHOES
```

---

## 🚀 Schritt 3: Spiel starten

### Standard-Start:
```bash
./build/ECHOES
```

### Mit Debug-Ausgabe:
```bash
./build/ECHOES 2>&1 | tee game_log.txt
```

### Im Hintergrund (falls Vollbild-Probleme):
```bash
./build/ECHOES &
```

---

## 🎮 Steuerung

### Hauptmenü:
- **Maus**: Klicke auf Buttons (New Game, Settings, Quit)
- **ESC**: Zurück/Beenden

### Im Spiel:
- **A/D** oder **←/→**: Bewegen
- **SPACE** oder **↑**: Springen
- **ESC**: Pause-Menü

### Pause-Menü:
- **Resume**: Spiel fortsetzen
- **Settings**: Einstellungen ändern
- **Main Menu**: Zurück zum Hauptmenü
- **Quit**: Spiel beenden

### Settings-Menü:
- **↑/↓**: Option wählen
- **←/→**: Werte ändern (Lautstärke, Auflösung)
- **ENTER**: Option aktivieren (Fullscreen, Controls anzeigen)
- **ESC**: Zurück

---

## 🧪 Schritt 4: Test-Checkliste

### ✅ Basis-Funktionalität:
- [ ] Spiel startet ohne Fehler
- [ ] Hauptmenü wird angezeigt
- [ ] Musik spielt im Hauptmenü
- [ ] Settings-Menü öffnet sich
- [ ] Lautstärke-Änderungen funktionieren
- [ ] Fullscreen-Toggle funktioniert

### ✅ Gameplay:
- [ ] Level 1 lädt korrekt
- [ ] Spieler bewegt sich flüssig
- [ ] Sprung funktioniert
- [ ] Kollision mit Plattformen
- [ ] Sterne können eingesammelt werden
- [ ] Sound-Effekte spielen (Jump, Collect)
- [ ] Pause-Menü (ESC) funktioniert
- [ ] Settings aus Pause-Menü erreichbar

### ✅ Horror-Elemente (ab Level 3+):
- [ ] Glitch-Effekte erscheinen
- [ ] Screen-Shake bei Events
- [ ] Jumpscare-Sound funktioniert
- [ ] Horror-Ambient-Musik (höhere Level)

### ✅ Boss-Kämpfe:
- [ ] Level 6: GlitchBoss erscheint
- [ ] Boss kann getroffen werden
- [ ] Boss-Defeat-Horror-Sequenz
- [ ] Level 8: EchoPrime Boss
- [ ] Level 10: Finaler Boss

---

## 🐛 Troubleshooting

### Problem: "SDL2 not found"
```bash
# Überprüfe SDL2-Installation
pkg-config --modversion sdl2
# Falls nicht installiert, siehe Schritt 1
```

### Problem: "Permission denied"
```bash
chmod +x build/ECHOES
```

### Problem: "Cannot open shared object file"
```bash
# Library-Pfad aktualisieren
sudo ldconfig
```

### Problem: Schwarzer Bildschirm
```bash
# Starte ohne Fullscreen
# Ändere in Settings: Fullscreen = OFF
```

### Problem: Kein Sound
```bash
# Überprüfe SDL_mixer-Installation
pkg-config --modversion SDL2_mixer

# Überprüfe Audio-Dateien
ls -lh assets/audio/
```

### Problem: Kompilierungsfehler
```bash
# Clean build
rm -rf build
mkdir build
cd build
cmake ..
cmake --build . -j$(nproc)
```

---

## 📊 Performance-Tipps

### Hohe FPS:
- Deaktiviere Fullscreen, falls vsync-Probleme
- Reduziere Auflösung in Settings

### Niedrige FPS:
- Aktiviere Hardware-Beschleunigung
- Schließe andere Programme
- Nutze dedicated GPU (falls vorhanden)

---

## 📝 Log-Ausgaben verstehen

### Normale Ausgaben:
```
[AudioManager] Initialized successfully
[Game] Initialized successfully
[Game] State changed: MAIN_MENU
```

### Horror-System aktiv:
```
[ExtremePCHorror] Activating effect: SYSTEM_WARNING
[PCHorror] Screen glitch intensity: 0.75
```

### Boss-Kampf:
```
[Boss] GlitchBoss spawned at (400, 300)
[Boss] Health: 5/5
[Boss] Taking damage!
```

---

## 🎯 Schnelltest-Befehl

Alles in einem Befehl:
```bash
cd /home/user/KrankesHorrorSpiel && \
mkdir -p build && \
cd build && \
cmake .. && \
cmake --build . -j$(nproc) && \
cd .. && \
./build/ECHOES
```

---

## 📧 Bei Problemen

Falls Fehler auftreten:
1. Sammle Log-Ausgabe: `./build/ECHOES 2>&1 | tee error.log`
2. Prüfe CMake-Output auf Fehler
3. Überprüfe alle Dependencies sind installiert
4. Prüfe asset-Dateien existieren: `ls assets/audio/`

---

## ✅ Erfolgreicher Start sieht so aus:

```
[AudioManager] Initialized successfully
[AudioManager] Loaded music: menu_music
[AudioManager] Loaded music: level_music
[Game] Initialized successfully
[MainMenu] Created successfully
[SettingsMenu] Created successfully
[Game] State changed: INTRO
[Game] State changed: MAIN_MENU
```

Viel Erfolg beim Testen! 🎮👻
