#!/bin/bash

# ECHOES.exe - Automatic Sound Downloader
# Downloads FREE CC0/Public Domain sounds for the game

set -e

AUDIO_DIR="assets/audio"
mkdir -p "$AUDIO_DIR"

echo "========================================"
echo "  ECHOES Sound Downloader"
echo "========================================"
echo ""
echo "Downloading lizenzfreie (CC0) Sounds..."
echo ""

# Funktion zum Downloaden
download_sound() {
    local url="$1"
    local filename="$2"
    local description="$3"

    if [ -f "$AUDIO_DIR/$filename" ]; then
        echo "✓ $filename already exists, skipping..."
        return
    fi

    echo "Downloading: $description..."
    if curl -L -o "$AUDIO_DIR/$filename" "$url" 2>/dev/null; then
        echo "✓ $filename downloaded successfully!"
    else
        echo "✗ Failed to download $filename"
        return 1
    fi
}

# ===========================================
# MUSIC (CC0 - Public Domain)
# ===========================================

echo ""
echo "=== MUSIC ==="
echo ""

# Menu Music - Upbeat Chiptune
download_sound \
    "https://freesound.org/data/previews/635/635837_12657973-lq.mp3" \
    "menu_music.mp3" \
    "Menu Music (Upbeat Chiptune)"

# Level Music - Happy 8-bit
download_sound \
    "https://freesound.org/data/previews/397/397629_7255534-lq.mp3" \
    "level_music.mp3" \
    "Level Music (Happy Platformer)"

# Corrupted Music - Dark Ambient
download_sound \
    "https://freesound.org/data/previews/665/665851_10019005-lq.mp3" \
    "corrupted_music.mp3" \
    "Corrupted Music (Dark Ambient)"

# Horror Ambient
download_sound \
    "https://freesound.org/data/previews/639/639888_10019005-lq.mp3" \
    "horror_ambient.mp3" \
    "Horror Ambient (Drone)"

# ===========================================
# SOUND EFFECTS (CC0)
# ===========================================

echo ""
echo "=== SOUND EFFECTS ==="
echo ""

# Jump Sound
download_sound \
    "https://freesound.org/data/previews/270/270303_5123851-lq.mp3" \
    "jump.wav" \
    "Jump Sound"

# Collect/Coin Sound
download_sound \
    "https://freesound.org/data/previews/341/341695_5858296-lq.mp3" \
    "collect.wav" \
    "Collect Sound"

# Death Sound
download_sound \
    "https://freesound.org/data/previews/587/587196_10510375-lq.mp3" \
    "death.wav" \
    "Death Sound"

# ===========================================
# HORROR SOUNDS (CC0)
# ===========================================

echo ""
echo "=== HORROR SOUNDS ==="
echo ""

# Jumpscare Scream
download_sound \
    "https://freesound.org/data/previews/445/445974_7037-lq.mp3" \
    "jumpscare.wav" \
    "Jumpscare Scream"

# White Noise/Static
download_sound \
    "https://freesound.org/data/previews/39/39562_214281-lq.mp3" \
    "whitenoise.wav" \
    "White Noise"

# Scare Appear
download_sound \
    "https://freesound.org/data/previews/369/369516_4921277-lq.mp3" \
    "scare_appear.wav" \
    "Scare Appear Sound"

# Scare Audio Spike
download_sound \
    "https://freesound.org/data/previews/121/121622_16676-lq.mp3" \
    "scare_audio.wav" \
    "Audio Spike"

echo ""
echo "========================================"
echo "  Download Complete!"
echo "========================================"
echo ""
echo "Downloaded sounds to: $AUDIO_DIR/"
echo ""
echo "All sounds are CC0 (Public Domain) from freesound.org"
echo ""
echo "If any download failed, you can:"
echo "1. Run this script again"
echo "2. Download manually from freesound.org"
echo "3. Use the Python generator (see generate_placeholder_sounds.py)"
echo ""
