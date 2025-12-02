#include "UIHelper.h"
#include <iostream>
#include <unordered_map>

std::unordered_map<int, TTF_Font*> UIHelper::s_fonts;

void UIHelper::init() {
    // TTF already initialized in Game.cpp
    std::cout << "[UIHelper] Initialized" << std::endl;
}

void UIHelper::shutdown() {
    // Free all fonts
    for (auto& pair : s_fonts) {
        if (pair.second) {
            TTF_CloseFont(pair.second);
        }
    }
    s_fonts.clear();
}

TTF_Font* UIHelper::getFont(int size) {
    auto it = s_fonts.find(size);
    if (it != s_fonts.end() && it->second) {
        return it->second;
    }

    // Try to load a system font (fallback to default)
    // For now, we'll use a simple mono font path
    // Users should provide their own .ttf file later

    const char* fontPaths[] = {
        "assets/fonts/font.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",
        "/usr/share/fonts/TTF/DejaVuSansMono.ttf",
        "C:\\Windows\\Fonts\\consola.ttf",
        nullptr
    };

    TTF_Font* font = nullptr;
    for (int i = 0; fontPaths[i] != nullptr; ++i) {
        font = TTF_OpenFont(fontPaths[i], size);
        if (font) {
            std::cout << "[UIHelper] Loaded font: " << fontPaths[i] << " (size " << size << ")" << std::endl;
            break;
        }
    }

    if (!font) {
        std::cerr << "[UIHelper] Failed to load any font! UI will not render text." << std::endl;
    }

    s_fonts[size] = font;
    return font;
}

void UIHelper::renderText(SDL_Renderer* renderer, const std::string& text,
                         int x, int y, SDL_Color color,
                         int fontSize, bool centered) {
    TTF_Font* font = getFont(fontSize);
    if (!font) return;

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) {
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect destRect;
    destRect.w = surface->w;
    destRect.h = surface->h;
    destRect.x = centered ? (x - destRect.w / 2) : x;
    destRect.y = y;

    SDL_RenderCopy(renderer, texture, nullptr, &destRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

// Überladene Version mit RGB-Werten
void UIHelper::renderText(SDL_Renderer* renderer, const std::string& text,
                         int x, int y, int fontSize,
                         int r, int g, int b) {
    SDL_Color color = {static_cast<Uint8>(r), static_cast<Uint8>(g), static_cast<Uint8>(b), 255};
    renderText(renderer, text, x, y, color, fontSize, false);
}

bool UIHelper::renderButton(SDL_Renderer* renderer, Button& button,
                            int mouseX, int mouseY, bool mouseClicked) {
    // Check hover
    button.hovered = (mouseX >= button.rect.x && mouseX <= button.rect.x + button.rect.w &&
                     mouseY >= button.rect.y && mouseY <= button.rect.y + button.rect.h);

    // Check click
    button.clicked = button.hovered && mouseClicked;

    // Render button box
    SDL_Color bgColor = button.hovered ? SDL_Color{80, 80, 80, 255} : SDL_Color{50, 50, 50, 255};
    SDL_Color borderColor = button.hovered ? SDL_Color{255, 255, 255, 255} : SDL_Color{150, 150, 150, 255};

    renderBox(renderer, button.rect, bgColor, borderColor, 2);

    // Render text
    SDL_Color textColor = {255, 255, 255, 255};
    int textX = button.rect.x + button.rect.w / 2;
    int textY = button.rect.y + button.rect.h / 2 - 12; // Rough centering
    renderText(renderer, button.text, textX, textY, textColor, 24, true);

    return button.clicked;
}

void UIHelper::renderBox(SDL_Renderer* renderer, SDL_Rect rect,
                        SDL_Color bgColor, SDL_Color borderColor,
                        int borderWidth) {
    // Background
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &rect);

    // Border
    SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    for (int i = 0; i < borderWidth; ++i) {
        SDL_Rect borderRect = {
            rect.x - i,
            rect.y - i,
            rect.w + i * 2,
            rect.h + i * 2
        };
        SDL_RenderDrawRect(renderer, &borderRect);
    }
}

SDL_Rect UIHelper::getTextBounds(const std::string& text, int fontSize) {
    TTF_Font* font = getFont(fontSize);
    if (!font) {
        return {0, 0, 0, 0};
    }

    int w, h;
    TTF_SizeText(font, text.c_str(), &w, &h);
    return {0, 0, w, h};
}
