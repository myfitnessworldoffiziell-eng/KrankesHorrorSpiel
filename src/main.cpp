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
        std::cout << "  - Fake system crashes (harmless!)" << std::endl;
        std::cout << "  - Jumpscares with audio" << std::endl;
        std::cout << std::endl;
        std::cout << "By continuing, you consent to these mechanics." << std::endl;
        std::cout << "You can quit anytime by pressing ESC or closing the window." << std::endl;
        std::cout << std::endl;
        std::cout << "==== CONTROLS ====" << std::endl;
        std::cout << "  Arrow Keys / WASD - Move" << std::endl;
        std::cout << "  SPACE - Jump / Continue Dialog" << std::endl;
        std::cout << "  ESC - Pause / Back" << std::endl;
        std::cout << "  F11 - Fullscreen" << std::endl;
        std::cout << std::endl;
        std::cout << "==== DEBUG KEYS (while playing) ====" << std::endl;
        std::cout << "  G - Trigger Glitch Effect" << std::endl;
        std::cout << "  F - Create Creepy File" << std::endl;
        std::cout << "  W - Window Manipulation" << std::endl;
        std::cout << "  J - Trigger Jumpscare" << std::endl;
        std::cout << "  B - Fake Blue Screen" << std::endl;
        std::cout << std::endl;
        std::cout << "Press ENTER to start..." << std::endl;
        std::cin.get();
        std::cout << std::endl;

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
        std::cout << std::endl;

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
        return 1;
    }
}
