#include "AudioManager.h"
#include <iostream>

AudioManager::AudioManager()
    : m_musicVolume(64)
    , m_sfxVolume(64)
    , m_initialized(false)
    , m_distortionLevel(0.0f)
{
}

AudioManager::~AudioManager() {
    shutdown();
}

bool AudioManager::initialize() {
    std::cout << "[AudioManager] Initializing..." << std::endl;

    // Mix_OpenAudio should already be called in Game.cpp
    // Allocate channels for sound effects (default is only 8)
    int channels = Mix_AllocateChannels(32);
    std::cout << "[AudioManager] Allocated " << channels << " audio channels" << std::endl;

    // Set default volumes
    Mix_VolumeMusic(m_musicVolume);
    std::cout << "[AudioManager] Music volume: " << m_musicVolume << "/128" << std::endl;
    std::cout << "[AudioManager] SFX volume: " << m_sfxVolume << "/128" << std::endl;

    m_initialized = true;
    std::cout << "[AudioManager] Initialized successfully" << std::endl;
    return true;
}

void AudioManager::shutdown() {
    if (!m_initialized) return;

    std::cout << "[AudioManager] Shutting down..." << std::endl;

    // Stop all audio
    Mix_HaltMusic();
    Mix_HaltChannel(-1);

    // Free music
    for (auto& pair : m_music) {
        if (pair.second) {
            Mix_FreeMusic(pair.second);
        }
    }
    m_music.clear();

    // Free sounds
    for (auto& pair : m_sounds) {
        if (pair.second) {
            Mix_FreeChunk(pair.second);
        }
    }
    m_sounds.clear();

    m_initialized = false;
}

bool AudioManager::loadMusic(const std::string& id, const std::string& filepath) {
    std::cout << "[AudioManager] Loading music '" << id << "' from: " << filepath << std::endl;

    Mix_Music* music = Mix_LoadMUS(filepath.c_str());
    if (!music) {
        std::cerr << "[AudioManager] ❌ FAILED to load music '" << id << "'" << std::endl;
        std::cerr << "[AudioManager]    Path: " << filepath << std::endl;
        std::cerr << "[AudioManager]    SDL_mixer error: " << Mix_GetError() << std::endl;
        return false;
    }

    // Free existing if present
    auto it = m_music.find(id);
    if (it != m_music.end() && it->second) {
        Mix_FreeMusic(it->second);
    }

    m_music[id] = music;
    std::cout << "[AudioManager] ✓ Loaded music: " << id << std::endl;
    return true;
}

void AudioManager::playMusic(const std::string& id, int loops) {
    std::cout << "[AudioManager] 🎵 Attempting to play music: " << id << std::endl;

    auto it = m_music.find(id);
    if (it == m_music.end() || !it->second) {
        std::cerr << "[AudioManager] ❌ Music not found: " << id << std::endl;
        return;
    }

    if (Mix_PlayMusic(it->second, loops) == -1) {
        std::cerr << "[AudioManager] ❌ Failed to play music '" << id << "': " << Mix_GetError() << std::endl;
        return;
    }

    m_currentMusicId = id;
    std::cout << "[AudioManager] ✓ Now playing music: " << id << " (loops: " << loops << ")" << std::endl;
}

void AudioManager::stopMusic() {
    Mix_HaltMusic();
    m_currentMusicId.clear();
}

void AudioManager::pauseMusic() {
    Mix_PauseMusic();
}

void AudioManager::resumeMusic() {
    Mix_ResumeMusic();
}

void AudioManager::setMusicVolume(int volume) {
    m_musicVolume = volume;
    Mix_VolumeMusic(m_musicVolume);
}

bool AudioManager::loadSound(const std::string& id, const std::string& filepath) {
    std::cout << "[AudioManager] Loading sound '" << id << "' from: " << filepath << std::endl;

    Mix_Chunk* sound = Mix_LoadWAV(filepath.c_str());
    if (!sound) {
        std::cerr << "[AudioManager] ❌ FAILED to load sound '" << id << "'" << std::endl;
        std::cerr << "[AudioManager]    Path: " << filepath << std::endl;
        std::cerr << "[AudioManager]    SDL_mixer error: " << Mix_GetError() << std::endl;
        return false;
    }

    // Free existing if present
    auto it = m_sounds.find(id);
    if (it != m_sounds.end() && it->second) {
        Mix_FreeChunk(it->second);
    }

    m_sounds[id] = sound;
    std::cout << "[AudioManager] ✓ Loaded sound: " << id << std::endl;
    return true;
}

void AudioManager::playSound(const std::string& id, int volume) {
    auto it = m_sounds.find(id);
    if (it == m_sounds.end() || !it->second) {
        std::cerr << "[AudioManager] ❌ Sound not found: " << id << std::endl;
        return;
    }

    // Apply global SFX volume
    int adjustedVolume = (volume * m_sfxVolume) / 128;
    Mix_VolumeChunk(it->second, adjustedVolume);

    int channel = Mix_PlayChannel(-1, it->second, 0);
    if (channel == -1) {
        std::cerr << "[AudioManager] ❌ Failed to play sound '" << id << "': " << Mix_GetError() << std::endl;
    } else {
        std::cout << "[AudioManager] 🔊 Playing sound: " << id << " (volume: " << adjustedVolume << "/128, channel: " << channel << ")" << std::endl;
    }
}

void AudioManager::setSFXVolume(int volume) {
    m_sfxVolume = volume;
    if (m_sfxVolume < 0) m_sfxVolume = 0;
    if (m_sfxVolume > 128) m_sfxVolume = 128;
}

void AudioManager::distortMusic(float amount) {
    m_distortionLevel = amount;

    // Simulate distortion durch Volume-Manipulation
    // (echte Distortion würde DSP benötigen)
    int baseVolume = 64;
    int distortedVolume = static_cast<int>(baseVolume * (1.0f - amount * 0.5f));
    Mix_VolumeMusic(distortedVolume);
}

void AudioManager::playJumpscare() {
    // Jumpscare sound (wenn geladen)
    playSound("jumpscare", 128);
}

void AudioManager::playWhiteNoise(int duration_ms) {
    // White noise (wenn geladen)
    playSound("whitenoise");

    // TODO: Stop nach duration_ms
    (void)duration_ms;
}

void AudioManager::update(float deltaTime, int corruptionLevel) {
    (void)deltaTime;

    // Progressive Musik-Distortion basierend auf corruption
    if (corruptionLevel > 30) {
        float distortion = (corruptionLevel - 30) / 70.0f; // 0.0 - 1.0
        distortMusic(distortion);
    }
}
