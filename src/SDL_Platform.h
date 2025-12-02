#pragma once

/**
 * Platform-agnostic SDL2 includes
 *
 * On macOS with Homebrew/vcpkg, SDL2 headers are at <SDL.h>
 * On Linux/Windows, SDL2 headers are at <SDL2/SDL.h>
 */

#ifdef __APPLE__
    #include <SDL.h>
    #include <SDL_image.h>
    #include <SDL_mixer.h>
    #include <SDL_ttf.h>
#else
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_mixer.h>
    #include <SDL2/SDL_ttf.h>
#endif
