/**
 * INTERPRETER.CPP
 * 
 * This file contains the functions responsible for interpreting and executing
 *  CHIP-8 instructions.
 * 
 * EPSILON0, 19.08.2021
 */

#include <SFML/Graphics.hpp>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <random>
#include "interpreter.h"
using namespace std;

// External variables
extern bool hideWarnings;
extern bool flipControls;
extern sf::RenderWindow window;
extern sf::VertexArray screen;
extern unsigned char screenData[];
extern sf::VertexArray screenArray;
extern sf::Color foreground;
extern sf::Color background;

// Terminal logs color
extern std::string CC, CW, CG, CB, CY, CO, CLO, CGR, CBL, CR;
#define ERROR '[' << CR << "ERROR" << CC << "]:"
#define WARNING '[' << CY << "WARNING" << CC << "]:"

// Key bindings
sf::Keyboard::Key keysLUT[] = {
    sf::Keyboard::Key::X, sf::Keyboard::Key::Num1, sf::Keyboard::Key::Num2, sf::Keyboard::Key::Num3,
    sf::Keyboard::Key::Q, sf::Keyboard::Key::W, sf::Keyboard::Key::E, sf::Keyboard::Key::A,
    sf::Keyboard::Key::S, sf::Keyboard::Key::D, sf::Keyboard::Key::Num4, sf::Keyboard::Key::R,
    sf::Keyboard::Key::F, sf::Keyboard::Key::V, sf::Keyboard::Key::C, sf::Keyboard::Key::Z
};
sf::Keyboard::Key keysLUTflipped[] = {
    sf::Keyboard::Key::X, sf::Keyboard::Key::A, sf::Keyboard::Key::S, sf::Keyboard::Key::D,
    sf::Keyboard::Key::Q, sf::Keyboard::Key::W, sf::Keyboard::Key::E, sf::Keyboard::Key::Num1,
    sf::Keyboard::Key::Num2, sf::Keyboard::Key::Num3, sf::Keyboard::Key::Num4, sf::Keyboard::Key::R,
    sf::Keyboard::Key::F, sf::Keyboard::Key::V, sf::Keyboard::Key::C, sf::Keyboard::Key::Z
};

// Character set
static const unsigned char characters[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

/**
 * @brief This constructor just clears the VM state
 * 
 */
Interpreter::Interpreter(void)
{
    I = 0;
    PC = 0;
    SP = 0;

    for (unsigned i = 0; i < 16; i++)
        V[i] = 0; 

    for (unsigned i = 0; i < 12; i++)
        callStack[i] = 0;
    
    for (unsigned i = 0; i < 4096; i++)
        memoryArray[i] = 0;
}

/**
 * @brief This function reads a program from a file
 * 
 */
int Interpreter::readProgram(char* filename)
{
    // Open a file
    std::ifstream program(filename);
    if (program.fail()) {
        cout << ERROR << " File does not exist\n";
        return -1;
    }

    // Load the program
    program.read((char*)(memoryArray + 512), 3584);

    // Load a small "Bootloader"
    memoryArray[0] = 0x00;
    memoryArray[1] = 0xE0;
    memoryArray[2] = 0x12;
    memoryArray[3] = 0x00;

    // Load the character set
    for (unsigned i = 0; i < 80; i++) 
        memoryArray[0x1B0 + i] = characters[i];

    // Close a file
    program.close();
    return 0;
}

/**
 * @brief This function handles all math opcodes (0x8xxx)
 * 
 */
void Interpreter::handleMathOpcode(void)
{
    switch (opcode & 0x000F) {
        case 0x0: // Assignment
            V[(opcode >> 8) & 0xF] = V[(opcode >> 4) & 0xF];
            break;

        case 0x1: // Logical OR
            V[(opcode >> 8) & 0xF] |= V[(opcode >> 4) & 0xF];
            break;

        case 0x2: // Logical AND
            V[(opcode >> 8) & 0xF] &= V[(opcode >> 4) & 0xF];
            break;

        case 0x3: // Logical XOR
            V[(opcode >> 8) & 0xF] ^= V[(opcode >> 4) & 0xF];
            break;

        case 0x4: // Addition
            if ((uint16)V[(opcode >> 8) & 0xF] + (uint16)V[(opcode >> 4) & 0xF] > 0xFF)
                V[0xF] = 1; else V[0xF] = 0;
            V[(opcode >> 8) & 0xF] += V[(opcode >> 4) & 0xF];
            break;

        case 0x5: // Subtraction
            if (V[(opcode >> 8) & 0xF] >= V[(opcode >> 4) & 0xF])
                V[0xF] = 1; else V[0xF] = 0;
            V[(opcode >> 8) & 0xF] -= V[(opcode >> 4) & 0xF];
            break;

        case 0x6: // Logical shift right
            V[0xF] = V[(opcode >> 8) & 0xF] & 0x1;
            V[(opcode >> 8) & 0xF] = V[(opcode >> 8) & 0xF] >> 1;
            break;

        case 0x7: // Subtraction
            if (V[(opcode >> 8) & 0xF] >= V[(opcode >> 4) & 0xF])
                V[0xF] = 1; else V[0xF] = 0;
            V[(opcode >> 8) & 0xF] -= V[(opcode >> 4) & 0xF];
            break;

        case 0xE: // Logical shift left
            V[0xF] = ((V[(opcode >> 8) & 0xF] & 0x80) != 0);
            V[(opcode >> 8) & 0xF] = V[(opcode >> 8) & 0xF] << 1;
            break;

        default:
            if (!hideWarnings) cout << WARNING << " Invalid misc opcode: " << uppercase << hex << opcode << "h, skipping\n";
            break;
    }
}

/**
 * @brief This function handles miscellaneous instructions (0xFxxx)
 * 
 */
void Interpreter::handleMiscOpcode(void) 
{
    // Some variables
    unsigned reg = (opcode >> 8) & 0xF;
    bool gotKey = 0;

    // Check for index register overflow
    if (I > 0xFFF) {
        if (!hideWarnings) cout << WARNING << " Index register out of range: " << uppercase << hex << I << "h\n";
        I &= 0xFFF;
    }

    switch (opcode & 0xFF) {
        case 0x07: // Read delay timer
            V[reg] = delayTimer;
            break;

        case 0x0A: // Wait for the key
            for (unsigned i = 0; i < 16; i++) {
                if (sf::Keyboard::isKeyPressed(((flipControls)? keysLUTflipped : keysLUT)[i])) {
                    gotKey = 1;
                    V[reg] = i;
                }
            }
            // If still waiting for the key move the PC back to the same instruction
            if (!gotKey) PC -= 2;
            break;

        case 0x15: // Set delay timer
            delayTimer = V[reg];
            break;
            
        case 0x18: // Set sound timer
            soundTimer = V[reg];
            break;

        case 0x1E: // Add to index register
            I += V[reg];
            break;

        case 0x29: // Set I to the location of the sprite
            I = 0x1B0 + (V[reg] & 0xF) * 5;
            break;

        case 0x33: // Set BCD
            if (I > 0xFFD) {
                if (!hideWarnings) cout << WARNING << " Unable to complete BCD call, I to high: " << uppercase << hex << I << "h\n";
                I = 0xFFD;
            }
            memoryArray[I+0] = V[reg] / 100;
            memoryArray[I+1] = V[reg] / 10 % 10;
            memoryArray[I+2] = V[reg] % 10;
            break;

        case 0x55: // Dump registers to memory
            if (I > 0xFFF - reg) {
                if (!hideWarnings) cout << WARNING << " Unable to complete register dump, I to high: " << uppercase << hex << I << "h\n";
                I = 0xFFF - reg;
            }
            for (unsigned i = 0; i <= reg; i++)
                memoryArray[I + i] = V[i];
            break;

        case 0x65: // Read memory to registers
            if (I > 0xFFF - reg) {
                if (!hideWarnings) cout << WARNING << " Unable to complete memory read, I to high: " << uppercase << hex << I << "h\n";
                I = 0xFFF - reg;
            }
            for (unsigned i = 0; i <= reg; i++)
                V[i] = memoryArray[I + i];
            break;
    }
}

/**
 * @brief This function draws a sprite on the screen
 * 
 */
void Interpreter::drawSprite(void)
{
    // Generate variables
    unsigned X = V[(opcode >> 8) & 0xF];
    unsigned Y = V[(opcode >> 4) & 0xF];
    unsigned N = opcode & 0xF;

    // Clear colision flag
    V[0xF] = 0;

    // Check if coordinates are inside of the screen space
    if (X > 63) {
        if (!hideWarnings) cout << WARNING << " Sprite X too high: " << uppercase << hex << X << "h\n";
        X = 63;
    }
    if (Y > 31) {
        if (!hideWarnings) cout << WARNING << " Sprite X too high: " << uppercase << hex << Y << "h\n";
        Y = 31;
    }

    for (unsigned y = 0; y < N; y++) {
        for (unsigned x = 0; x < 8; x++) {
            if (X + x < 64 && Y + y < 32) {
                if ((memoryArray[I + y] >> (7 - x)) & 0x1) {
                    // Save the data to screen data
                    screenData[(Y + y) * 64 + X + x] ^= 1;
                    // If we unset anything set colision flag
                    if (!(screenData[(Y + y) * 64 + X + x])) V[0xF] = 1;
                    // Set the color
                    sf::Color color = (screenData[(Y + y) * 64 + X + x]) ? foreground : background;
                    screenArray[((Y + y) * 64 + X + x) * 4 + 0].color = color;
                    screenArray[((Y + y) * 64 + X + x) * 4 + 1].color = color;
                    screenArray[((Y + y) * 64 + X + x) * 4 + 2].color = color;
                    screenArray[((Y + y) * 64 + X + x) * 4 + 3].color = color;
                }
            }
        }
    }
}

/**
 * @brief This function updates the timers (count down)
 * 
 */
void Interpreter::timerUpdate(void)
{
    if (delayTimer) delayTimer--;
    if (soundTimer) soundTimer--;
}

/**
 * @brief This function fetches and executes a single instruction
 * 
 * @return -1 is returned on critical errors, 0 otherwise
 */
int Interpreter::Update(void)
{
    // Fetch an opcode
    if (PC > 0xFFE) {
        cout << ERROR << " PC out of range: ";
        cout << hex << setw(4) << setfill('0') << PC << "h\n";
        return -1;
    }

    opcode = (memoryArray[PC] << 8) + memoryArray[PC + 1];
    PC += 2;

    // Decode the opcode
    switch (opcode & 0xF000) {

        case 0x0000: // Return from subroutine and screen clear
            if (opcode == 0xE0) {
                for (unsigned i = 0; i < 8192; i++)
                    screenArray[i].color = sf::Color::Black;
                for (unsigned i = 0; i < 2048; i++) 
                    screenData[i] = 0;
            } else if (opcode == 0xEE) {
                if (SP == 0) {
                    if (!hideWarnings) cout << ERROR << " Call stack underflow at address: " << uppercase << hex << PC-2 << "h\n";
                    return -1;
                }
                PC = callStack[--SP];
            } else {
                if (!hideWarnings) cout << WARNING << " Machine code call at address: " << uppercase << hex << PC-2 << "h, skipping...\n";
            }
            break;
        
        case 0x1000: // "GOTO" Command
            PC = opcode & 0xFFF;
            break;

        case 0x2000: // Call a subroutine
            if (SP == 11) {
                if (!hideWarnings) std::cout << ERROR << " Call stack overflow on memory address " << uppercase << hex << PC-2 << "h\n";
                return -1;
            }
            callStack[SP++] = PC;
            PC = opcode & 0xFFF;
            break;

        case 0x3000: // Skip if equal to immediate
            if (V[(opcode >> 8) & 0xF] == (opcode & 0xFF))
                PC += 2;
            break;

        case 0x4000: // Skip if not equal to immediate
            if (V[(opcode >> 8) & 0xF] != (opcode & 0xFF))
                PC += 2;
            break;

        case 0x5000: // Skip if equal
            if (opcode & 0xF) {
                if (!hideWarnings) cout << WARNING << " Invalid skip opcode: " << uppercase << hex << opcode << "h\n";
            }
            if (V[(opcode >> 8) & 0xF] == V[(opcode >> 4) & 0xF])
                PC += 2;
            break;

        case 0x6000: // Load immediate value
            V[(opcode >> 8) & 0xF] = opcode & 0xFF;
            break;

        case 0x7000: // Add immediate value
            V[(opcode >> 8) & 0xF] += (uint8)(opcode & 0xFF);
            break;

        case 0x8000: // Math operations
            handleMathOpcode();
            break;

        case 0x9000: // Skip if not equal
            if (opcode & 0xF) {
                if (!hideWarnings) cout << WARNING << " Invalid skip opcode: " << uppercase << hex << opcode << "h\n";
            }
            if (V[(opcode >> 8) & 0xF] != V[(opcode >> 4) & 0xF])
                PC += 2;
            break;

        case 0xA000: // Set the index pointer
            I = opcode & 0xFFF;
            break;
        
        case 0xB000: // Indexed jump
            PC = V[0] + (opcode & 0xFFF);
            break;

        case 0xC000: // Random
            V[(opcode >> 8) & 0xF] = rand() & (opcode & 0xFF);
            break;

        case 0xD000: // Draw on the screen
            drawSprite();
            break;

        case 0xE000: // Keyboard instructions
            if ((opcode & 0xFF) == 0x9E) { // Key equal to register
                if (sf::Keyboard::isKeyPressed(((flipControls)? keysLUTflipped : keysLUT)[V[(opcode >> 8) & 0xF] & 0xF])) {
                    PC += 2;
                }
            } else if ((opcode & 0xFF) == 0xA1) { // Key equal to register
                if (!(sf::Keyboard::isKeyPressed(((flipControls)? keysLUTflipped : keysLUT)[V[(opcode >> 8) & 0xF] & 0xF]))) {
                    PC += 2;
                }
            } else {
                if (!hideWarnings) cout << WARNING << " Invalid keyboard opcode: " << uppercase << hex << opcode << "h\n";
            }
            break;

        case 0xF000: // Misc instructions
            handleMiscOpcode();
            break;

        default:
            break;
    }

    return 0;
}
