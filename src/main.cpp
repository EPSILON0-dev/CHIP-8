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
#include "color.h"

sf::RenderWindow window(sf::VideoMode(640, 320), "CHIP-8");
Interpreter interpreter;

extern sf::VertexArray screenArray;

extern bool verbose;
extern bool stepByStep;
extern unsigned cyclesPerFrame;
extern unsigned killAddress;
extern std::string CC, CW, CG, CB, CY, CO, CLO, CGR, CBL, CR;

#define INFO '[' << CG << "INFO" << CC << "]:"
#define ERROR '[' << CR << "ERROR" << CC << "]:"
#define WARNING '[' << CY << "WARNING" << CC << "]:"

unsigned prevPressed = 0;

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << ERROR << " Missing argument\n";
        std::cout << INFO << " Usage: chip8 [-v] [-k kill address (dec)] filename\n";
        return 1;
    }

    // Parse arguments
    if (parse_arguments(argc, argv) == -1) {
        std::cout << ERROR << " Critical error, exiting...\n";
        return 1;  
    }

    // Set color scheme
    setColorScheme();

    // Generate screen array 
    generateScreenArray();

    // Initialize the interpreter
    if (interpreter.readProgram(argv[argc - 1]))
        return 1;

    // Prin waiting for start message
    bool started = 0;
    if (stepByStep) {
        std::cout << "Press [space] to execute a instruction\n";
    } else {
        std::cout << "Press [space] to start execution...\n";
    }

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

            // Normal execution
            if (!stepByStep) {
                // Execute 8 instruction per frame (480Hz)
                for (unsigned i = 0; i < cyclesPerFrame; i++) {
                    // Check kill address
                    if (interpreter.PC == killAddress) {
                        std::cout << INFO << " Kill address reached, exiting...\n";
                        return 0;
                    }

                    // Execute and check for errors
                    if(interpreter.Update() == -1) {
                        std::cout << ERROR << " Critical error, exiting...\n";
                        return 1;
                    }

                    // Log state
                    if (verbose) {
                        log();
                    }
                }

            // Step by step execution
            } else {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                    if (!prevPressed) {
                        // Check kill address
                        if (interpreter.PC == killAddress) {
                            std::cout << INFO << " Kill address reached, exiting...\n";
                            return 0;
                        }

                        // Execute and check for errors
                        if(interpreter.Update() == -1) {
                            std::cout << ERROR << " Critical error, exiting...\n";
                            return 1;
                        }

                        // Log state
                        if (verbose) {
                            log();
                        }

                        prevPressed = 6;
                    } else {
                        prevPressed--;
                    }
                } else {
                    prevPressed = 0;
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