#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <map>
#include <memory>

/**
 * Texture Manager
 *
 * Centralized texture loading and management system.
 * Handles both file-based and procedurally-generated textures.
 */
class TextureManager {
public:
    TextureManager(SDL_Renderer* renderer);
    ~TextureManager();

    // Load texture from file
    bool loadTexture(const std::string& id, const std::string& filepath);

    // Create procedural texture (generated at runtime)
    bool createProceduralTexture(const std::string& id, int width, int height,
                                   SDL_Surface* (*generator)(int, int));

    // Get texture by ID
    SDL_Texture* getTexture(const std::string& id);

    // Check if texture exists
    bool hasTexture(const std::string& id) const;

    // Remove texture
    void removeTexture(const std::string& id);

    // Clear all textures
    void clearAll();

    // Render texture
    void render(const std::string& id, SDL_Rect* srcRect, SDL_Rect* dstRect);
    void renderEx(const std::string& id, SDL_Rect* srcRect, SDL_Rect* dstRect,
                  double angle, SDL_Point* center, SDL_RendererFlip flip);

private:
    SDL_Renderer* m_renderer;
    std::map<std::string, SDL_Texture*> m_textures;

    // Helper: Create texture from surface
    SDL_Texture* createTextureFromSurface(SDL_Surface* surface);
};
