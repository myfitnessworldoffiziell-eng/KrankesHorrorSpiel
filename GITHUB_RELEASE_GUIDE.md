# 🎯 EINFACHSTE METHODE - Release auf GitHub erstellen

Da automatisches Tag-Pushen nicht funktioniert, hier die manuelle Methode (noch einfacher!):

## Schritt-für-Schritt:

### 1. Gehe zu deinem GitHub Repository:
```
https://github.com/myfitnessworldoffiziell-eng/KrankesHorrorSpiel
```

### 2. Klicke auf "Actions" (oben in der Navigation)

### 3. Links findest du "Build and Release" - klicke darauf

### 4. Rechts findest du einen Button "Run workflow"
- Klicke auf "Run workflow"
- Branch auswählen: `claude/creepy-game-system-hacks-016LkwarLFtVg9MjPhZnxXis`
- Klicke "Run workflow" (grüner Button)

### 5. Warte 10-15 Minuten
Der Build läuft jetzt! Du siehst:
- 🟡 Gelb = Build läuft
- ✅ Grün = Build erfolgreich
- ❌ Rot = Build fehlgeschlagen

### 6. Wenn der Build fertig ist (✅ grün):
- Gehe zu "Actions" → Klicke auf den erfolgreichen Workflow-Run
- Scrolle nach unten zu "Artifacts"
- Download:
  - **ECHOES-Windows-x64.zip**
  - ECHOES-Linux-x64.tar.gz
  - ECHOES-macOS-x64.tar.gz

### 7. Teste die .exe:
- Entpacke `ECHOES-Windows-x64.zip`
- Doppelklick auf `ECHOES.exe`
- **SPIELEN!** 🎮

---

## 🔄 Alternative: Release manuell erstellen

Falls du einen richtigen GitHub Release willst:

1. Gehe zu: https://github.com/myfitnessworldoffiziell-eng/KrankesHorrorSpiel/releases
2. Klicke "Create a new release"
3. Tag: `v1.0.1`
4. Title: "ECHOES.exe v1.0.1 - Vollständiges Horror-Platformer-Spiel"
5. Upload die .zip-Dateien aus den Artifacts (von Schritt 6 oben)
6. Klicke "Publish release"

---

## ✅ Schnellste Methode:

**GitHub Actions → Run workflow → Warte 10 Min → Download Artifacts → Fertig!**

Die .exe ist dann direkt spielbar! 🎮👻
