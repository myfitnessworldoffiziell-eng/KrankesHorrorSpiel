# 🚀 Release-Anleitung - ECHOES.exe erstellen

## So erstellst du automatisch Windows .exe + Linux + macOS Builds:

### **Methode 1: Automatisches Release (Empfohlen)**

1. **Tag erstellen und pushen:**
   ```bash
   git tag v1.0.0
   git push origin v1.0.0
   ```

2. **Warten (ca. 5-10 Minuten)**
   - GitHub Actions baut automatisch:
     - ✅ Windows .exe
     - ✅ Linux Binary
     - ✅ macOS Binary

3. **Release herunterladen:**
   - Gehe zu: https://github.com/myfitnessworldoffiziell-eng/KrankesHorrorSpiel/releases
   - Lade `ECHOES-Windows-x64.zip` herunter
   - Entpacken und `ECHOES.exe` starten!

---

### **Methode 2: Manuelles Triggern**

1. Gehe zu GitHub → **Actions** Tab
2. Klicke auf **"Build and Release"**
3. Klicke **"Run workflow"**
4. Warte 5-10 Minuten
5. Download die Artifacts

---

## 📦 Was wird erstellt?

Nach dem Build findest du in den Releases:

### **ECHOES-Windows-x64.zip**
```
ECHOES-Windows/
├── ECHOES.exe          ← Fertige Windows-Anwendung!
├── assets/             ← Alle Audio-Dateien
│   └── audio/
│       ├── menu_music.mp3
│       ├── level_music.mp3
│       └── ... (alle 14 Audio-Dateien)
├── README.md
└── INSTALL_GUIDE.md
```

**Windows-User müssen nur:**
1. ZIP entpacken
2. `ECHOES.exe` doppelklicken
3. Spielen! 🎮

### **ECHOES-Linux-x64.tar.gz**
```
ECHOES-Linux/
├── ECHOES              ← Linux Binary
├── assets/
└── ...
```

**Linux-User:**
```bash
tar -xzf ECHOES-Linux-x64.tar.gz
cd ECHOES-Linux
./ECHOES
```

### **ECHOES-macOS-x64.tar.gz**
```
ECHOES-macOS/
├── ECHOES              ← macOS Binary
├── assets/
└── ...
```

**macOS-User:**
```bash
tar -xzf ECHOES-macOS-x64.tar.gz
cd ECHOES-macOS
./ECHOES
```

---

## 🎯 Quick Release Erstellen:

```bash
# Im Projektverzeichnis:
git add .
git commit -m "Release v1.0.0"
git tag v1.0.0
git push origin claude/creepy-game-system-hacks-016LkwarLFtVg9MjPhZnxXis
git push origin v1.0.0
```

Dann auf GitHub unter **Releases** schauen! 🚀

---

## 🔄 Neue Version erstellen:

Einfach neue Version-Nummer nutzen:
```bash
git tag v1.1.0
git push origin v1.1.0
```

Versionsnummern:
- `v1.0.0` - Erste Release
- `v1.1.0` - Neue Features
- `v1.0.1` - Bugfixes
- `v2.0.0` - Große Änderungen

---

## ✅ Checklist vor Release:

- [ ] Alle Änderungen committed
- [ ] Spiel getestet (funktioniert lokal)
- [ ] Alle Audio-Dateien vorhanden
- [ ] Version-Tag erstellt
- [ ] Tag gepusht

---

## 🐛 Troubleshooting:

### "Workflow läuft nicht"
- Prüfe ob `.github/workflows/build-release.yml` existiert
- Prüfe GitHub Actions Tab
- Prüfe ob Tag mit `v` beginnt (z.B. `v1.0.0`)

### "Build failed"
- Klicke auf den fehlgeschlagenen Build
- Schau welcher Job fehlschlug (Windows/Linux/macOS)
- Lies die Fehlermeldung

### "Keine .exe in Release"
- Prüfe ob der `create-release` Job lief
- Manchmal dauert es 1-2 Minuten nach Build

---

## 📤 Spiel teilen:

Nach erfolgreichem Release kannst du den Link teilen:
```
https://github.com/myfitnessworldoffiziell-eng/KrankesHorrorSpiel/releases/latest
```

User können dann einfach die .exe für ihr System runterladen! 🎮👻
