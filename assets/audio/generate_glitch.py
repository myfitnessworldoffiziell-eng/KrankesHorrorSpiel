#!/usr/bin/env python3
"""
Generate a creepy glitch sound effect using only standard Python libraries
Creates intense, short glitch bursts with:
- Bit crushing (digital distortion)
- Random frequency shifts
- White noise bursts
- Metallic/robotic tones
"""

import wave
import random
import math
import struct

# Sound parameters
SAMPLE_RATE = 44100  # CD quality
DURATION = 0.8  # Short, intense 0.8 seconds
NUM_SAMPLES = int(SAMPLE_RATE * DURATION)
MAX_AMPLITUDE = 32767  # 16-bit audio max

def generate_glitch_sound():
    """Generate an intense, creepy glitch sound effect"""
    samples = []

    # Generate glitch in segments for variation
    segment_size = NUM_SAMPLES // 8  # 8 segments

    for segment in range(8):
        segment_start = segment * segment_size

        # Randomize parameters for each segment
        glitch_type = random.randint(0, 4)

        for i in range(segment_size):
            t = (segment_start + i) / SAMPLE_RATE
            sample = 0

            if glitch_type == 0:
                # WHITE NOISE BURST (digital static)
                sample = random.uniform(-1.0, 1.0) * 0.8

            elif glitch_type == 1:
                # BIT-CRUSHED TONE (robotic/digital)
                freq = random.choice([100, 150, 200, 300, 500, 800])
                bit_depth = random.choice([4, 6, 8])  # Lower = more crushed
                raw_sample = math.sin(2 * math.pi * freq * t)
                # Bit crush: quantize to fewer levels
                levels = 2 ** bit_depth
                sample = math.floor(raw_sample * levels) / levels

            elif glitch_type == 2:
                # FREQUENCY SWEEP (glitchy pitch change)
                start_freq = random.randint(200, 800)
                end_freq = random.randint(50, 200)
                freq = start_freq + (end_freq - start_freq) * (i / segment_size)
                sample = math.sin(2 * math.pi * freq * t) * 0.6

            elif glitch_type == 3:
                # HARSH SQUARE WAVE (digital buzz)
                freq = random.choice([150, 250, 400, 600])
                raw = math.sin(2 * math.pi * freq * t)
                sample = 1.0 if raw > 0 else -1.0
                sample *= 0.5

            else:
                # CHAOTIC NOISE (random clicks and pops)
                if random.random() < 0.3:  # 30% chance of spike
                    sample = random.choice([-1.0, 1.0]) * random.uniform(0.7, 1.0)
                else:
                    sample = random.uniform(-0.2, 0.2)

            # Apply envelope (fade in/out to avoid clicks)
            envelope = 1.0
            fade_samples = SAMPLE_RATE // 100  # 10ms fade
            if i < fade_samples:
                envelope = i / fade_samples
            elif (segment_start + i) > (NUM_SAMPLES - fade_samples):
                envelope = (NUM_SAMPLES - (segment_start + i)) / fade_samples

            sample *= envelope

            # Random amplitude variation (makes it more chaotic)
            sample *= random.uniform(0.6, 1.0)

            # Convert to 16-bit integer
            sample_int = int(sample * MAX_AMPLITUDE)
            sample_int = max(-MAX_AMPLITUDE, min(MAX_AMPLITUDE, sample_int))

            samples.append(sample_int)

    return samples

def save_wav(filename, samples):
    """Save samples as WAV file"""
    with wave.open(filename, 'w') as wav_file:
        # Set WAV parameters (mono, 16-bit, 44.1kHz)
        wav_file.setnchannels(1)  # Mono
        wav_file.setsampwidth(2)  # 2 bytes = 16 bit
        wav_file.setframerate(SAMPLE_RATE)

        # Pack samples as 16-bit signed integers (little endian)
        packed_samples = struct.pack('<' + 'h' * len(samples), *samples)
        wav_file.writeframes(packed_samples)

    print(f"✓ Generated creepy glitch sound: {filename}")
    print(f"  Duration: {DURATION}s")
    print(f"  Sample Rate: {SAMPLE_RATE}Hz")
    print(f"  Samples: {len(samples)}")

if __name__ == "__main__":
    print("Generating intense horror glitch sound effect...")

    # Generate the glitch sound
    glitch_samples = generate_glitch_sound()

    # Save to file
    output_file = "glitch.wav"
    save_wav(output_file, glitch_samples)

    print("\n🎵 Glitch sound ready! Use it in the game.")
