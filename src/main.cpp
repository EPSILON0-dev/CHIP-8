/**
 * CHIP-8 Interpreter
 * 
 * This program interprets the original CHIP-8 code, the display
 *  is handled by SFML library and errors and warnings are
 *  printed to the terminal.
 * 
 * EPSILON0, 20.08.2021
 */

#include <SFML/Graphics.hpp>
#include <iomanip>
#include <iostream>
#include <random>
#include "interpreter.h"
#include "screen.h"
#include "util.h"

sf::RenderWindow window(sf::VideoMode(640, 320), "CHIP-8");
Interpreter interpreter;

extern sf::VertexArray screenArray;

bool verbose = 0;
unsigned killAddress = 0x1000;

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "[\x1b[38;2;249;38;114mERROR\x1b[0m]: Missing argument\n";
        std::cout << "[\x1b[38;2;166;242;46mINFO\x1b[0m]: Usage: chip8 [-v] [-k kill address (dec)] filename\n";
        return 1;
    }

    generateScreenArray();

    bool started = 0;
    std::cout << "Press [space] to start execution...\n";

    // Parse arguments
    if (parse_arguments(argc, argv) == -1)
        return 1;  

    // Initialize the interpreter
    interpreter.readProgram(argv[argc - 1]);

    // Enable 60 fps vsync
    window.setVerticalSyncEnabled(1);
    window.setFramerateLimit(60);

    while (1) {
        // Event
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                return 0;
                break;
            
            default:
                break;
            }
        }

        // Wait for start
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) started = 1;
        
        // Execute a few instructions
        if (started) {
            for (unsigned i = 0; i < 128; i++) {
                // Check kill address
                if (interpreter.PC == killAddress) {
                    std::cout << "[\x1b[38;2;166;242;46mINFO\x1b[0m]: Kill address reached, exiting...\n";
                    return 0;
                }

                // Execute and check for errors
                if(interpreter.Update() == -1) {
                    std::cout << "[\x1b[38;2;249;38;114mERROR\x1b[0m]: Critical error, exiting...\n";
                    return 1;
                }

                // Log state
                if (verbose) {
                    log();
                }
            }
        }

        // Update timers
        interpreter.timerUpdate();

        // Draw function
        window.clear(sf::Color::Black);
        window.draw(screenArray);
        window.display();
    }
}