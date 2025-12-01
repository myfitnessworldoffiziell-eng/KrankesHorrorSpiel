#include "TextureManager.h"
#include <iostream>

TextureManager::TextureManager(SDL_Renderer* renderer)
    : m_renderer(renderer)
{
}

TextureManager::~TextureManager() {
    clearAll();
}

bool TextureManager::loadTexture(const std::string& id, const std::string& filepath) {
    // Check if already loaded
    if (hasTexture(id)) {
        std::cout << "[TextureManager] Texture '" << id << "' already loaded" << std::endl;
        return true;
    }

    // Load image
    SDL_Surface* surface = IMG_Load(filepath.c_str());
    if (!surface) {
        std::cerr << "[TextureManager] Failed to load texture '" << filepath << "': " << IMG_GetError() << std::endl;
        return false;
    }

    // Create texture
    SDL_Texture* texture = createTextureFromSurface(surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "[TextureManager] Failed to create texture from surface" << std::endl;
        return false;
    }

    m_textures[id] = texture;
    std::cout << "[TextureManager] Loaded texture: " << id << " from " << filepath << std::endl;
    return true;
}

bool TextureManager::createProceduralTexture(const std::string& id, int width, int height,
                                              SDL_Surface* (*generator)(int, int)) {
    // Check if already exists
    if (hasTexture(id)) {
        std::cout << "[TextureManager] Texture '" << id << "' already exists" << std::endl;
        return true;
    }

    // Generate surface
    SDL_Surface* surface = generator(width, height);
    if (!surface) {
        std::cerr << "[TextureManager] Failed to generate procedural surface for '" << id << "'" << std::endl;
        return false;
    }

    // Create texture
    SDL_Texture* texture = createTextureFromSurface(surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "[TextureManager] Failed to create texture from procedural surface" << std::endl;
        return false;
    }

    m_textures[id] = texture;
    std::cout << "[TextureManager] Created procedural texture: " << id << " (" << width << "x" << height << ")" << std::endl;
    return true;
}

SDL_Texture* TextureManager::getTexture(const std::string& id) {
    auto it = m_textures.find(id);
    if (it != m_textures.end()) {
        return it->second;
    }
    return nullptr;
}

bool TextureManager::hasTexture(const std::string& id) const {
    return m_textures.find(id) != m_textures.end();
}

void TextureManager::removeTexture(const std::string& id) {
    auto it = m_textures.find(id);
    if (it != m_textures.end()) {
        SDL_DestroyTexture(it->second);
        m_textures.erase(it);
        std::cout << "[TextureManager] Removed texture: " << id << std::endl;
    }
}

void TextureManager::clearAll() {
    for (auto& pair : m_textures) {
        SDL_DestroyTexture(pair.second);
    }
    m_textures.clear();
    std::cout << "[TextureManager] Cleared all textures" << std::endl;
}

void TextureManager::render(const std::string& id, SDL_Rect* srcRect, SDL_Rect* dstRect) {
    SDL_Texture* texture = getTexture(id);
    if (texture) {
        SDL_RenderCopy(m_renderer, texture, srcRect, dstRect);
    }
}

void TextureManager::renderEx(const std::string& id, SDL_Rect* srcRect, SDL_Rect* dstRect,
                               double angle, SDL_Point* center, SDL_RendererFlip flip) {
    SDL_Texture* texture = getTexture(id);
    if (texture) {
        SDL_RenderCopyEx(m_renderer, texture, srcRect, dstRect, angle, center, flip);
    }
}

SDL_Texture* TextureManager::createTextureFromSurface(SDL_Surface* surface) {
    if (!surface) return nullptr;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    if (!texture) {
        std::cerr << "[TextureManager] SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
    }

    return texture;
}
