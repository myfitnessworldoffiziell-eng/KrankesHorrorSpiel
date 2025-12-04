# Audio Assets Folder

This folder contains all game audio files.

## Required Audio Files

### Music (MP3 format)
- `horror_ambient.mp3` - Main menu background music
- `level_music.mp3` - Level background music
- `corrupted_music.mp3` - Corrupted/glitch area music
- `menu_music.mp3` - Alternative menu music

### Sound Effects (WAV format)

#### Gameplay SFX
- `jump.wav` - Jump sound
- `collect.wav` - Item collection sound
- `death.wav` - Player death sound
- `hit.wav` - Damage/hit sound
- `chirp.wav` - UI notification sound
- `zap.wav` - Electrical zap sound

#### Horror SFX
- `jumpscare.wav` - Jumpscare sound effect
- `whitenoise.wav` - Static/white noise
- `scare_appear.wav` - Enemy appearance sound
- `scare_audio.wav` - General scary ambience
- `glitch.wav` - Digital glitch effect

## File Format Requirements
- **Music**: MP3, 128-320 kbps, stereo
- **SFX**: WAV, 44.1kHz, 16-bit, mono or stereo
- **File names**: Must match exactly as listed above

## Adding New Sounds
1. Place audio files in this folder with exact filenames
2. Commit to git repository
3. GitHub Actions will automatically build with new sounds
