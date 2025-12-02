#!/bin/bash

# 🎮 ECHOES.exe - Automatisches Build & Test Script
# Dieses Script baut und startet das Spiel automatisch

set -e  # Beende bei Fehlern

echo "=================================================="
echo "🎮 ECHOES.exe - Build & Test Script"
echo "=================================================="
echo ""

# Farben für Ausgabe
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Schritt 1: Überprüfe Dependencies
echo -e "${BLUE}[1/5] Überprüfe Dependencies...${NC}"

check_dependency() {
    if pkg-config --exists "$1" 2>/dev/null; then
        version=$(pkg-config --modversion "$1")
        echo -e "${GREEN}  ✅ $1 gefunden (Version: $version)${NC}"
        return 0
    else
        echo -e "${RED}  ❌ $1 NICHT gefunden${NC}"
        return 1
    fi
}

all_deps_ok=true
check_dependency "sdl2" || all_deps_ok=false
check_dependency "SDL2_image" || all_deps_ok=false
check_dependency "SDL2_mixer" || all_deps_ok=false
check_dependency "SDL2_ttf" || all_deps_ok=false

if [ "$all_deps_ok" = false ]; then
    echo ""
    echo -e "${RED}❌ Fehler: Nicht alle Dependencies installiert!${NC}"
    echo -e "${YELLOW}Installiere mit:${NC}"
    echo "  Ubuntu/Debian: sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev"
    echo "  Fedora: sudo dnf install SDL2-devel SDL2_image-devel SDL2_mixer-devel SDL2_ttf-devel"
    echo "  Arch: sudo pacman -S sdl2 sdl2_image sdl2_mixer sdl2_ttf"
    exit 1
fi

echo ""

# Schritt 2: Überprüfe Audio-Assets
echo -e "${BLUE}[2/5] Überprüfe Audio-Assets...${NC}"

audio_files=(
    "assets/audio/menu_music.mp3"
    "assets/audio/level_music.mp3"
    "assets/audio/corrupted_music.mp3"
    "assets/audio/horror_ambient.mp3"
    "assets/audio/jump.wav"
    "assets/audio/collect.wav"
    "assets/audio/death.wav"
    "assets/audio/hit.wav"
    "assets/audio/chirp.wav"
    "assets/audio/zap.wav"
    "assets/audio/jumpscare.wav"
    "assets/audio/whitenoise.wav"
    "assets/audio/scare_appear.wav"
    "assets/audio/scare_audio.wav"
)

missing_audio=false
for file in "${audio_files[@]}"; do
    if [ -f "$file" ]; then
        echo -e "${GREEN}  ✅ $(basename $file)${NC}"
    else
        echo -e "${RED}  ❌ $(basename $file) FEHLT${NC}"
        missing_audio=true
    fi
done

if [ "$missing_audio" = true ]; then
    echo -e "${YELLOW}  ⚠️  Warnung: Einige Audio-Dateien fehlen (Spiel startet trotzdem)${NC}"
fi

echo ""

# Schritt 3: Clean & Create Build Directory
echo -e "${BLUE}[3/5] Erstelle Build-Verzeichnis...${NC}"

if [ -d "build" ]; then
    echo -e "${YELLOW}  🗑️  Lösche altes Build-Verzeichnis...${NC}"
    rm -rf build
fi

mkdir -p build
echo -e "${GREEN}  ✅ Build-Verzeichnis erstellt${NC}"
echo ""

# Schritt 4: CMake Configure
echo -e "${BLUE}[4/5] Konfiguriere CMake...${NC}"
cd build

if cmake .. ; then
    echo -e "${GREEN}  ✅ CMake erfolgreich konfiguriert${NC}"
else
    echo -e "${RED}  ❌ CMake-Konfiguration fehlgeschlagen${NC}"
    exit 1
fi

echo ""

# Schritt 5: Build
echo -e "${BLUE}[5/5] Kompiliere Projekt...${NC}"

# Ermittle Anzahl der CPU-Kerne
if command -v nproc &> /dev/null; then
    cores=$(nproc)
else
    cores=4
fi

echo -e "${YELLOW}  🔨 Nutze $cores CPU-Kerne für parallele Kompilierung...${NC}"

if cmake --build . -j"$cores" ; then
    echo -e "${GREEN}  ✅ Kompilierung erfolgreich!${NC}"
else
    echo -e "${RED}  ❌ Kompilierung fehlgeschlagen${NC}"
    exit 1
fi

cd ..

echo ""
echo "=================================================="
echo -e "${GREEN}✅ Build erfolgreich abgeschlossen!${NC}"
echo "=================================================="
echo ""

# Zeige Binary-Informationen
if [ -f "build/ECHOES" ]; then
    binary_size=$(du -h build/ECHOES | cut -f1)
    echo -e "${GREEN}📦 Binary-Größe: $binary_size${NC}"
    echo -e "${GREEN}📍 Pfad: $(pwd)/build/ECHOES${NC}"
    echo ""
fi

# Frage ob Spiel gestartet werden soll
echo -e "${YELLOW}Möchtest du das Spiel jetzt starten? (j/n)${NC}"
read -r response

if [[ "$response" =~ ^[jJyY]$ ]]; then
    echo ""
    echo "=================================================="
    echo "🚀 Starte ECHOES.exe..."
    echo "=================================================="
    echo ""
    echo -e "${BLUE}Steuerung:${NC}"
    echo "  Bewegen: A/D oder ←/→"
    echo "  Springen: SPACE oder ↑"
    echo "  Pause: ESC"
    echo ""
    echo -e "${YELLOW}Drücke Ctrl+C zum Beenden${NC}"
    echo ""

    # Starte das Spiel und logge Output
    ./build/ECHOES 2>&1 | tee game_log.txt

    echo ""
    echo -e "${GREEN}Spiel beendet. Log gespeichert in: game_log.txt${NC}"
else
    echo ""
    echo -e "${GREEN}Das Spiel kannst du später mit folgendem Befehl starten:${NC}"
    echo -e "${BLUE}  ./build/ECHOES${NC}"
    echo ""
    echo -e "${YELLOW}Oder mit Log-Ausgabe:${NC}"
    echo -e "${BLUE}  ./build/ECHOES 2>&1 | tee game_log.txt${NC}"
fi

echo ""
echo "=================================================="
echo "✨ Fertig!"
echo "=================================================="
