# 🔊 Audio Setup - Quick Start

## ✅ Was bereits existiert:

### WAV Files (10/10 - FERTIG!):
- ✅ `jump.wav` (stiller Platzhalter)
- ✅ `collect.wav` (stiller Platzhalter)
- ✅ `death.wav` (stiller Platzhalter)
- ✅ `hit.wav` (stiller Platzhalter)
- ✅ `chirp.wav` (stiller Platzhalter)
- ✅ `zap.wav` (stiller Platzhalter)
- ✅ `jumpscare.wav` (stiller Platzhalter)
- ✅ `whitenoise.wav` (stiller Platzhalter)
- ✅ `scare_appear.wav` (stiller Platzhalter)
- ✅ `scare_audio.wav` (stiller Platzhalter)

**Diese Dateien sind gültige WAV-Dateien, aber STILL (keine Audio).**
Das Spiel kann damit kompilieren und laufen!

---

## ⚠️ Was noch fehlt:

### MP3 Files (0/4 - FEHLEN!):
- ❌ `menu_music.mp3`
- ❌ `level_music.mp3`
- ❌ `corrupted_music.mp3`
- ❌ `horror_ambient.mp3`

**Für jede dieser Dateien existiert eine `.txt` Datei mit Beschreibung!**

---

## 🚀 SCHNELLSTE LÖSUNG (3 Optionen):

### Option 1: Leere MP3s erstellen (Spiel läuft sofort)

```bash
# Im assets/audio/ Ordner:
touch menu_music.mp3
touch level_music.mp3
touch corrupted_music.mp3
touch horror_ambient.mp3
```

**Nachteil**: Keine Musik im Spiel, aber es läuft!

---

### Option 2: WAVs zu MP3s konvertieren (als Platzhalter)

```bash
# Mit ffmpeg (wenn installiert):
ffmpeg -f lavfi -i anullsrc=r=44100:cl=mono -t 120 -q:a 9 menu_music.mp3
ffmpeg -f lavfi -i anullsrc=r=44100:cl=mono -t 120 -q:a 9 level_music.mp3
ffmpeg -f lavfi -i anullsrc=r=44100:cl=mono -t 120 -q:a 9 corrupted_music.mp3
ffmpeg -f lavfi -i anullsrc=r=44100:cl=mono -t 180 -q:a 9 horror_ambient.mp3
```

**Vorteil**: Gültige MP3-Dateien, Spiel läuft!
**Nachteil**: Stille MP3s, keine echte Musik.

---

### Option 3: ECHTE SOUNDS VERWENDEN! (Beste Erfahrung)

**5-Minuten-Lösung:**

1. **Gehe zu https://www.bfxr.net**
   - Klicke verschiedene Presets:
     - "Jump" → Download als WAV → Ersetze `jump.wav`
     - "Pickup/Coin" → Download → Ersetze `collect.wav`
     - "Hit/Hurt" → Download → Ersetze `hit.wav`
     - "Explosion" → Download → Ersetze `death.wav`
     - "Laser/Shoot" → Download → Ersetze `zap.wav`
     - "Random" → Download → Ersetze `chirp.wav`

2. **Gehe zu https://freesound.org**
   - Suche "horror scream" → Download → Ersetze `jumpscare.wav`
   - Suche "white noise" → Download → Ersetze `whitenoise.wav`
   - Suche "horror appear" → Download → Ersetze `scare_appear.wav`
   - Suche "distorted voice" → Download → Ersetze `scare_audio.wav`

3. **Gehe zu https://opengameart.org**
   - Suche "chiptune menu"
   - Download eine passende Musik
   - Konvertiere zu MP3 (mit Audacity oder online)
   - Benenne um zu `menu_music.mp3`

4. **Wiederhole für andere Musik:**
   - "8-bit platformer" → `level_music.mp3`
   - "glitch music" → `corrupted_music.mp3`
   - "dark ambient horror" → `horror_ambient.mp3`

---

## 📝 Konvertierung WAV → MP3

**Mit Audacity (kostenlos):**
1. Öffne WAV-Datei
2. File → Export → Export as MP3
3. Fertig!

**Online:**
- https://convertio.co/wav-mp3/
- https://cloudconvert.com/wav-to-mp3

**Mit ffmpeg (Terminal):**
```bash
ffmpeg -i input.wav -q:a 0 output.mp3
```

---

## ✅ Finale Checklist:

Bevor du das Spiel kompilierst, stelle sicher:

```
assets/audio/
├── AUDIO_GUIDE.md          ✅ (Dokumentation)
├── SETUP.md                ✅ (Dieser Guide)
│
├── menu_music.mp3          ⚠️ (Benötigt!)
├── level_music.mp3         ⚠️ (Benötigt!)
├── corrupted_music.mp3     ⚠️ (Benötigt!)
├── horror_ambient.mp3      ⚠️ (Benötigt!)
│
├── jump.wav                ✅ (Platzhalter)
├── collect.wav             ✅ (Platzhalter)
├── death.wav               ✅ (Platzhalter)
├── hit.wav                 ✅ (Platzhalter)
├── chirp.wav               ✅ (Platzhalter)
├── zap.wav                 ✅ (Platzhalter)
├── jumpscare.wav           ✅ (Platzhalter)
├── whitenoise.wav          ✅ (Platzhalter)
├── scare_appear.wav        ✅ (Platzhalter)
└── scare_audio.wav         ✅ (Platzhalter)
```

**10/14 Dateien fertig! (71%)**
**Nur noch 4 MP3s fehlen!**

---

## 🎯 Empfehlung:

**Für sofortiges Testen:**
- Erstelle leere MP3s (Option 1)
- Kompiliere & teste das Spiel
- Ersetze Sounds später

**Für beste Erfahrung:**
- Verwende echte Sounds (Option 3)
- Nimm dir 30-60 Minuten Zeit
- Folge AUDIO_GUIDE.md

---

**Du schaffst das!** 🎮🔊
