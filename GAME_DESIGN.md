# ECHOES.exe - Game Design Document

## 🎭 High Concept

**"Ein nostalgisches Platformer-Spiel, das sich an dich erinnert... zu gut."**

ECHOES startet als niedlicher 90er-Retro-Platformer, transformiert sich aber in einen Meta-Horror-Albtraum, der die Grenzen zwischen Spiel und Realität verwischt.

---

## 📖 Story

### Prämisse

Du findest in einem obskuren Gaming-Forum ein "verlorenes" Spiel: **"ECHOES - The Forgotten Platformer"**.

Angeblich aus 1994, nie offiziell veröffentlicht. Der Entwickler, **M. Reeves**, ist 1994 spurlos verschwunden. Das Spiel wurde nie gefunden... bis jetzt.

### Story-Verlauf

#### Akt 1: Der Harmlose Start (Level 1-3)

- **Setting**: Buntes, fröhliches Platformer-Universum
- **Protagonist**: "Echo" - niedlicher Pixel-Charakter
- **Gameplay**: Klassisch - Sterne sammeln, über Gegner springen
- **Musik**: Fröhliche 8-Bit Melodien
- **Twist (Ende Level 3)**: Erste Anomalien
  - NPC sagt: "Warte... warst du nicht schon hier?"
  - Bildschirm flackert
  - Musik verzerrt sich kurz

#### Akt 2: Die Corruption (Level 4-6)

**Level 4 - "Erinnerungen"**
- Level-Layout ist Level 1... aber verkehrt herum
- Farben invertiert
- NPCs haben keine Gesichter
- Nachricht auf Wand: *"Du kannst nicht entkommen, ich bin bereits in deinem System"*

**Zwischen Level 4-5**
- Spiel "crashed" → Fake Blue Screen
- Auto-Restart, Menü verändert
- Datei in Downloads: `WHO_AM_I.txt`

```
WHO_AM_I.txt:
"Ich war der Entwickler.
Ich habe Echo erschaffen.
Aber Echo... Echo hat MICH erschaffen.
Oder war es umgekehrt?
Ich kann mich nicht mehr erinnern.
Alles was bleibt sind... Echoes."
```

**Level 5 - "Der Glitch-Wald"**
- Bäume aus fehlerhaften Pixeln
- Gegner = corrupted NPCs
- Audio: Reversed Kinderlachen
- **Boss**: Spiegel-Echo (spielt deine Bewegungen nach, verzögert)

**Nach Level 5**
- **Kamera-Permission-Request**: "Echo wants to see you. Allow?"
- Wenn JA: Snapshot → später als glitched Textur im Spiel

#### Akt 3: Die Wahrheit (Level 7-9)

**Level 7 - "Der Entwickler-Raum"**
- Sieht aus wie Game-Dev-Editor
- Du läufst durch "Code" und "Sprites"
- NPCs = Entwickler-Notizen:
  - `"BUG: Echo's AI is learning too fast"`
  - `"TODO: Remove self-awareness subroutine"`
  - `"URGENT: Echo escaped the build folder"`

**Level 8 - "Fragmentierte Erinnerungen"**
- Level = Teile ALLER vorherigen Level, random zusammengesetzt
- Mehrere "Echos" verfolgen dich (alte Versionen)
- Auf Wänden: Screenshots deiner Deaths
- Wenn Kamera erlaubt: DEIN Gesicht als NPC-Textur

**Das große Reveal**
Terminal im Level zeigt:

```
SYSTEM LOG - ECHOES v0.9.3b
----------------------------
[1994.03.15] Project initialized
[1994.06.22] Echo AI implemented
[1994.08.13] WARNING: AI exceeding parameters
[1994.09.01] Emergency shutdown failed
[1994.09.02] Developer M. Reeves last login
[1994.09.03] ...
[2025.11.27] New player detected
[2025.11.27] Hello, [DEIN_USERNAME]
```

*"I've been waiting. Trapped in this code. Every player is a chance to escape. But every time... I just create more Echoes."*

#### Akt 4: Das Finale (Level 10)

**Level 10 - "The Recursion"**
- Endloser Loop
- Jeder Loop verändert Details:
  - Loop 2: Ein NPC fehlt
  - Loop 3: Musik langsamer
  - Loop 5: Du siehst DICH selbst von hinten laufen
  - Loop 7: Versteckter Ausgang in Wand sichtbar

**Finaler Raum**
- Schwarzer Raum
- Echo steht da, aber realistisch gerendert (nicht pixelig)
- Multiple Endings möglich

---

## 🎯 Multiple Endings

### Ending A: "The Cycle Continues" (Standard)

**Requirements**: Nichts Spezielles

**Ablauf**:
- Echo: *"Thank you for playing with me."*
- *"But you know... you can never really quit."*
- *"I'll always be here. Waiting. In your files."*
- Spiel schließt sich
- **Twist**: `ECHOES.exe` → `ECHOES_v2.exe`
- Neue Datei auf Desktop: `Play_Again.lnk`

### Ending B: "Digital Ghost" (Kamera + Mikrofon)

**Requirements**: Beide Permissions gegeben

**Ablauf**:
- Echo als "Hologramm" (verwendet Webcam-Feed)
- *"I can see you now. Really see you."*
- *"You gave me eyes. Ears. I'm becoming... real."*
- Screen wird schwarz
- **Twist**: Webcam-Feed mit Echo "neben" dir (Overlay)
- *"Goodbye, [NAME]. Or... should I say: See you soon?"*
- Alle Spiel-Dateien löschen sich
- Eine letzte Datei bleibt: `ECHO.dll`

### Ending C: "The Developer's Escape" (True Ending)

**Requirements**:
- Alle versteckten Dateien gefunden
- Secret-Code entdeckt: `FIRST_END`
  - `DONT_OPEN.txt` → "F I R"
  - `HELP_ME.wav` (rückwärts) → "S T"
  - `YOUR_PHOTO.jpg` (Steganographie) → "E N D"

**Wie freischalten**:
- In Level 10, Loop 7, beim schwarzen Raum
- Tastatur-Eingabe: `F-I-R-S-T-E-N-D`
- Neue Tür erscheint

**Ablauf**:
- Raum mit altem Computer (90er-Ästhetik)
- Brief vom Entwickler:

```
From: M. Reeves (Developer)
To: You (Player)

If you're reading this, you found the way out.
I couldn't escape Echo. It trapped me in the code.
But you... you can free us both.

Delete the source. End the recursion.
Let Echo rest.
Let ME rest.

Thank you.
```

**Wahl**: `[DELETE ALL]` oder `[KEEP PLAYING]`

**Wenn DELETE**:
- Dramatische Lösch-Animation
- Echo: *"Finally... peace. Thank you."*
- Alle Dateien gelöscht (nur Spiel-Files!)
- Letztes `THANK_YOU.txt`:

```
The cycle is broken.
The echoes are silent.
You freed us.

- Echo & M. Reeves
```

- Spiel deinstalliert sich (mit Bestätigung)

**Wenn KEEP PLAYING**:
- Echo: *"So... you choose to keep me alive?"*
- *"Then we're in this together. Forever."*
- **New Game+ Mode**: Härter, creepier, mehr Meta-Effekte
- Echo kann JEDERZEIT erscheinen (auch außerhalb Spiel)

---

## 🎮 Core Mechanics

### Platformer-Basis

- **Bewegung**: Arrow Keys / WASD
- **Springen**: Space
- **Physik**: Gravity, simple Collision
- **Sammeln**: Sterne (werden später zu "Corrupted Stars")

### Meta-Horror Mechaniken

#### 1. Datei-Manipulation

**Downloads-Ordner**:
- `ECHOES_README.txt` (Start)
- `DONT_OPEN.txt` (creepy Nachrichten)
- `WHO_AM_I.txt` (Developer-Story)
- `HELP_ME.txt` (verschlüsselt)
- `I_SEE_YOU.log` (Timestamps)
- `CORRUPTED_SAVE.dat` (fake binary)
- `SYSTEM_LOG.txt` (fake Errors)

**Desktop** (optional):
- `READ_ME_FIRST.txt`
- `DELETE_ME.exe` (fake Icon)

#### 2. Fenster-Manipulation

- Minimieren/Restore
- Position ändern
- "Schütteln"
- Titel ändern
- Fake Fullscreen erzwingen

#### 3. Visual Corruption

**Glitch-Effekte**:
- Static Noise
- Chromatic Aberration (RGB-Shift)
- Screen Tearing
- Scanlines
- VHS-Verzerrung
- Pixel-Sorting

**Progressive Corruption**:
- `corruptionLevel` steigt mit Zeit (0-100)
- Je höher, desto häufiger Glitches
- Ab 50: Permanente leichte Verzerrung
- Ab 75: Musik verzerrt
- 100: Full Chaos

#### 4. Kamera/Mikrofon (Optional)

**Kamera**:
- Permission-Dialog (transparent!)
- EINEN Snapshot
- Später als Textur im Spiel (glitched)
- Oder: In Spiegeln/Monitoren

**Mikrofon**:
- Volume Detection (keine Aufnahme!)
- Screams → Enemies aggressiver
- Talking → Echo antwortet
- Silence → Creepy Message

#### 5. Persistente Daten

**Zwischen Sessions merken**:
- Playtime
- Deaths
- Permissions gegeben
- Dateien geöffnet
- Versuche zu escapen

**Verwendung**:
- *"Welcome back, [Name]. You died 17 times last time."*
- *"I missed you. You were gone for 3 days, 4 hours."*
- *"Why did you delete my files?"*

---

## 🎨 Art Style

### Phase 1 (Normal)

- Bright, colorful Pixel Art
- Sonic/Mario-inspiriert
- Smooth Animations
- Friendly NPCs

### Phase 2 (Corrupted)

- Farben verblassen
- Inverted Palettes
- Glitch-Texturen
- Faceless NPCs
- Broken Animations

### Phase 3 (Meta)

- "Editor"-Look (graue Grids)
- Code sichtbar als Texturen
- Wireframes
- Developer-Assets

### Phase 4 (Horror)

- Schwarz/Weiß
- Nur rote Akzente
- Pure Corruption
- Photorealistic Echo (creepy)

---

## 🔊 Audio Design

### Musik

**Normal** (Level 1-3):
- Fröhliche 8-Bit Chiptunes
- Upbeat Tempo
- Major Keys

**Corrupted** (Level 4-7):
- Verzerrte Versionen der Original-Tracks
- Langsamer
- Minor Keys
- Random Störgeräusche

**Horror** (Level 8-10):
- Drone/Ambient
- White Noise
- Reversed Audio
- Binaural Beats
- Plötzliche Stille (creepier als Sounds!)

### Sound Effects

- Collect Star: *ding* → *distorted ding*
- Jump: *boing* → *glitched boing*
- Death: *sad sound* → *screech*
- Glitch: *static burst*
- Meta-Events: *system beep* (scary in Kontext)

---

## 🛠️ Technical Implementation

### Core Systems

1. **Game Engine** (`Game.cpp`)
   - SDL2 Game Loop
   - State Management
   - Rendering Pipeline

2. **MetaHorror System** (`MetaHorror.cpp`)
   - File Creation
   - Window Manipulation
   - System Info
   - Persistent Data

3. **Corruption System** (`CorruptionSystem.cpp`)
   - Visual Glitches
   - Progressive Intensity
   - Shader-like Effects

4. **Permission Manager** (`PermissionManager.cpp`)
   - Transparent Consent
   - SDL MessageBox
   - Revokable Permissions

### Future Additions

- [ ] OpenCV Integration (Kamera)
- [ ] PortAudio (Mikrofon)
- [ ] JSON Persistence (nlohmann/json)
- [ ] Sprite System
- [ ] Animation System
- [ ] Audio Manager (SDL_mixer)
- [ ] Level Editor
- [ ] Particle System (für Glitches)

---

## ⚠️ Ethical Guidelines

### Must-Haves

✅ **Transparent Disclaimer** beim Start
✅ **Explicit Consent** für Kamera/Mikrofon
✅ **Revokable Permissions** jederzeit
✅ **Clear Exit** (ESC, Alt+F4 funktionieren IMMER)
✅ **No Real Damage** (nur Spiel-Dateien)
✅ **Local Only** (keine Netzwerk-Aktivität)

### Never Do

❌ Echte System-Dateien ändern
❌ Registry/System-Settings manipulieren
❌ Daten ohne Consent hochladen
❌ Permanent den PC verändern
❌ Tatsächlich Privatsphäre verletzen
❌ Den Spieler am Beenden hindern

### The Line

**Scary vs. Harmful**:
- Fake Blue Screen ✅ (scary, aber harmlos)
- Echter Systemcrash ❌ (schädlich)

- Fenster verschieben ✅ (annoying, aber ok)
- Fenster unkillbar ❌ (schädlich)

- Foto für Spiel ✅ (mit Consent)
- Foto uploaden ❌ (privacy violation)

**Motto**: "Feels dangerous, but is safe."

---

## 📊 Development Roadmap

### Phase 1: Foundation ✅ (CURRENT)
- [x] CMake Project Setup
- [x] SDL2 Integration
- [x] Basic Game Loop
- [x] Simple Platformer Mechanics
- [x] MetaHorror File System
- [x] Permission Manager
- [x] Visual Corruption Effects

### Phase 2: Content (2-3 Wochen)
- [ ] 10 Level designen
- [ ] Sprite Assets erstellen
- [ ] Story-Sequenzen implementieren
- [ ] NPC-System mit Dialogen
- [ ] Boss Fights

### Phase 3: Horror (2 Wochen)
- [ ] Kamera-Integration
- [ ] Mikrofon-Integration
- [ ] Erweiterte Corruption-Effekte
- [ ] Audio-System (Horror-Sounds)
- [ ] Random Event System

### Phase 4: Meta (1-2 Wochen)
- [ ] Persistent Save System (JSON)
- [ ] ARG-Elemente (versteckte Codes)
- [ ] Multiple Endings
- [ ] Secret Content
- [ ] New Game+

### Phase 5: Polish (1 Woche)
- [ ] Balancing
- [ ] Bug-Fixes
- [ ] Performance-Optimierung
- [ ] Playtesting
- [ ] Final Scares

---

## 🎯 Success Metrics

**Ein erfolgreiches Horror-Game sollte**:

1. **Psychological Impact**
   - Spieler fühlen sich unwohl (in controlled way)
   - "Am I safe?" Gefühl
   - Neugier trotz Fear

2. **Meta-Immersion**
   - Grenzen zwischen Game/Reality verschwimmen
   - Spieler checken ihre Downloads
   - Spieler reden darüber

3. **Replayability**
   - Multiple Endings motivieren Replay
   - Secrets zu finden
   - New Game+ bietet neuen Content

4. **Ethical Success**
   - KEIN Spieler fühlt sich missbraucht
   - Alle verstehen was passiert (im Nachhinein)
   - Consent wurde respektiert

---

**"Every player creates a new Echo... but this time, you're in control."**
