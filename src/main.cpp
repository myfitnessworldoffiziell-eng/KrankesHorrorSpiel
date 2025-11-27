#include "Game.h"
#include <iostream>
#include <exception>

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    try {
        // Disclaimer anzeigen
        std::cout << "========================================" << std::endl;
        std::cout << "     ECHOES - The Forgotten Platformer  " << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << std::endl;
        std::cout << "WARNING: This game uses meta-horror techniques:" << std::endl;
        std::cout << "  - File creation in Downloads folder" << std::endl;
        std::cout << "  - Optional camera/microphone access" << std::endl;
        std::cout << "  - Window manipulation" << std::endl;
        std::cout << "  - Psychological horror elements" << std::endl;
        std::cout << std::endl;
        std::cout << "By continuing, you consent to these mechanics." << std::endl;
        std::cout << "You can quit anytime by pressing ESC or closing the window." << std::endl;
        std::cout << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "  Arrow Keys / WASD - Move" << std::endl;
        std::cout << "  SPACE - Jump" << std::endl;
        std::cout << "  F11 - Fullscreen" << std::endl;
        std::cout << "  ESC - Quit" << std::endl;
        std::cout << std::endl;
        std::cout << "Debug Keys:" << std::endl;
        std::cout << "  G - Trigger Glitch" << std::endl;
        std::cout << "  F - Create File" << std::endl;
        std::cout << "  W - Window Manipulation" << std::endl;
        std::cout << std::endl;
        std::cout << "Press ENTER to start..." << std::endl;
        std::cin.get();

        // Game erstellen und starten
        Game game;

        if (!game.initialize()) {
            std::cerr << "Failed to initialize game!" << std::endl;
            return 1;
        }

        game.run();
        game.shutdown();

        std::cout << std::endl;
        std::cout << "Thank you for playing ECHOES..." << std::endl;
        std::cout << "Or did you?" << std::endl;

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
        return 1;
    }
}
