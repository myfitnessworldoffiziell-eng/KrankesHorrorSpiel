#!/usr/bin/env python3
"""
ECHOES.exe - Sound Generator (Pure Python, keine Dependencies!)
Generates simple placeholder sounds for the game
"""

import wave
import math
import struct
import os
import random

SAMPLE_RATE = 44100
AUDIO_DIR = "assets/audio"

def save_wav(filename, samples):
    """Save audio samples as WAV file"""
    filepath = os.path.join(AUDIO_DIR, filename)

    with wave.open(filepath, 'w') as wav_file:
        wav_file.setnchannels(1)  # Mono
        wav_file.setsampwidth(2)  # 16-bit
        wav_file.setframerate(SAMPLE_RATE)

        # Convert to 16-bit integers
        data = b''.join(struct.pack('<h', int(max(-32767, min(32767, s * 32767)))) for s in samples)
        wav_file.writeframes(data)

    print(f"✓ {filename}")

def sine_wave(frequency, duration, volume=0.5):
    """Generate sine wave"""
    samples = []
    for i in range(int(SAMPLE_RATE * duration)):
        t = i / SAMPLE_RATE
        sample = math.sin(2 * math.pi * frequency * t) * volume
        samples.append(sample)
    return samples

def square_wave(frequency, duration, volume=0.3):
    """Generate square wave (8-bit style)"""
    samples = []
    for i in range(int(SAMPLE_RATE * duration)):
        t = i / SAMPLE_RATE
        sample = 1.0 if math.sin(2 * math.pi * frequency * t) > 0 else -1.0
        samples.append(sample * volume)
    return samples

def noise(duration, volume=0.3):
    """Generate white noise"""
    samples = []
    for _ in range(int(SAMPLE_RATE * duration)):
        samples.append(random.uniform(-volume, volume))
    return samples

def frequency_sweep(freq_start, freq_end, duration, volume=0.5):
    """Generate frequency sweep"""
    samples = []
    num_samples = int(SAMPLE_RATE * duration)
    for i in range(num_samples):
        t = i / SAMPLE_RATE
        progress = i / num_samples
        freq = freq_start + (freq_end - freq_start) * progress
        sample = math.sin(2 * math.pi * freq * t) * volume
        samples.append(sample)
    return samples

def apply_envelope(samples, attack=0.1, release=0.1):
    """Apply attack/release envelope"""
    num_samples = len(samples)
    attack_samples = int(num_samples * attack)
    release_samples = int(num_samples * release)

    result = []
    for i, s in enumerate(samples):
        if i < attack_samples:
            # Attack
            envelope = i / attack_samples
        elif i > num_samples - release_samples:
            # Release
            envelope = (num_samples - i) / release_samples
        else:
            # Sustain
            envelope = 1.0

        result.append(s * envelope)

    return result

def mix(samples_list):
    """Mix multiple sample lists together"""
    max_len = max(len(s) for s in samples_list)
    mixed = []

    for i in range(max_len):
        total = 0
        for samples in samples_list:
            if i < len(samples):
                total += samples[i]
        mixed.append(total / len(samples_list))

    return mixed

def main():
    print("=" * 50)
    print("  ECHOES Sound Generator")
    print("  (Pure Python - No Dependencies!)")
    print("=" * 50)
    print()

    os.makedirs(AUDIO_DIR, exist_ok=True)

    print("🎵 Generating Music...")
    print()

    # MENU MUSIC - Happy chiptune melody
    menu = []
    notes = [523, 659, 784, 659, 523, 659, 784, 523]  # C E G E C E G C
    for note in notes:
        menu.extend(square_wave(note, 0.25, 0.15))
    menu = menu * 3  # Loop 3x
    save_wav("menu_music.mp3", menu)

    # LEVEL MUSIC - Upbeat platformer
    level = []
    melody = [523, 587, 659, 698, 784, 659, 523, 523]  # C D E F G E C C
    for note in melody:
        level.extend(square_wave(note, 0.2, 0.12))
    level = level * 4  # Loop 4x
    save_wav("level_music.mp3", level)

    # CORRUPTED MUSIC - Creepy minor scale
    corrupted = []
    dark_notes = [523, 554, 622, 698, 740, 622, 523, 466]  # Dissonant intervals
    for note in dark_notes:
        tone1 = sine_wave(note, 0.3, 0.08)
        tone2 = sine_wave(note * 0.99, 0.3, 0.08)  # Detuned
        mixed = mix([tone1, tone2])
        # Add noise
        noisy = [m + random.uniform(-0.02, 0.02) for m in mixed]
        corrupted.extend(noisy)
    corrupted = corrupted * 2
    save_wav("corrupted_music.mp3", corrupted)

    # HORROR AMBIENT - Dark drone
    drone1 = sine_wave(55, 8.0, 0.12)   # Low A
    drone2 = sine_wave(58, 8.0, 0.10)   # Dissonant A#
    ambient_noise = noise(8.0, 0.02)
    horror = mix([drone1, drone2, ambient_noise])
    save_wav("horror_ambient.mp3", horror)

    print()
    print("🔊 Generating Sound Effects...")
    print()

    # JUMP - Ascending sweep
    jump = frequency_sweep(200, 600, 0.15, 0.25)
    jump = apply_envelope(jump, 0.05, 0.5)
    save_wav("jump.wav", jump)

    # COLLECT - Happy notes ascending
    collect = []
    collect.extend(sine_wave(523, 0.07, 0.25))  # C
    collect.extend(sine_wave(659, 0.07, 0.25))  # E
    collect.extend(sine_wave(784, 0.10, 0.25))  # G
    save_wav("collect.wav", collect)

    # DEATH - Sad descending
    death = frequency_sweep(400, 100, 0.5, 0.3)
    death = apply_envelope(death, 0.1, 0.6)
    save_wav("death.wav", death)

    print()
    print("😱 Generating Horror Sounds...")
    print()

    # JUMPSCARE - LOUD burst
    scare_noise = noise(0.05, 0.7)
    scare_low = sine_wave(60, 0.3, 0.5)
    scare_high = sine_wave(2000, 0.2, 0.4)
    # Combine
    jumpscare = scare_noise + mix([scare_low[:len(scare_high)], scare_high])
    save_wav("jumpscare.wav", jumpscare)

    # WHITE NOISE - Static
    static = noise(3.0, 0.15)
    save_wav("whitenoise.wav", static)

    # SCARE APPEAR - Quick sting
    appear = frequency_sweep(100, 2000, 0.15, 0.4)
    save_wav("scare_appear.wav", appear)

    # SCARE AUDIO - Sharp spike
    spike = square_wave(1000, 0.05, 0.6)
    save_wav("scare_audio.wav", spike)

    print()
    print("=" * 50)
    print("  ✓ All sounds generated!")
    print("=" * 50)
    print()
    print(f"📁 Location: {AUDIO_DIR}/")
    print()
    print("11 audio files created:")
    print("  • 4x Music (Menu, Level, Corrupted, Ambient)")
    print("  • 3x SFX (Jump, Collect, Death)")
    print("  • 4x Horror (Jumpscare, Noise, 2x Scares)")
    print()
    print("🎮 Your game now has SOUND! 🔊")
    print()

if __name__ == "__main__":
    main()
