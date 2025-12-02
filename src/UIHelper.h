#pragma once
#include "SDL_Platform.h"

#include <string>
#include <memory>
#include <unordered_map>

/**
 * UIHelper
 * Hilfsklasse für UI-Rendering (Buttons, Text, etc.)
 */
class UIHelper {
public:
    static void init();
    static void shutdown();

    // Text Rendering
    static void renderText(SDL_Renderer* renderer, const std::string& text,
                          int x, int y, SDL_Color color,
                          int fontSize = 24, bool centered = false);

    // Überladene Version mit RGB-Werten
    static void renderText(SDL_Renderer* renderer, const std::string& text,
                          int x, int y, int fontSize,
                          int r, int g, int b);

    // Button Rendering
    struct Button {
        SDL_Rect rect;
        std::string text;
        bool hovered;
        bool clicked;
    };

    static bool renderButton(SDL_Renderer* renderer, Button& button,
                             int mouseX, int mouseY, bool mouseClicked);

    // Box/Panel Rendering
    static void renderBox(SDL_Renderer* renderer, SDL_Rect rect,
                         SDL_Color bgColor, SDL_Color borderColor,
                         int borderWidth = 2);

    // Utility
    static SDL_Rect getTextBounds(const std::string& text, int fontSize);

private:
    static TTF_Font* getFont(int size);
    static std::unordered_map<int, TTF_Font*> s_fonts;
};
