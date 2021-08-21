/**
 * UTIL.CPP
 * 
 * This file contains headers for all of the miscellaneous functions.
 * 
 * EPSILON0, 20.08.2021
 */

#include <SFML/Graphics.hpp>
#include <iomanip>
#include <iostream>
#include "interpreter.h"
#include "util.h"

// Variables
bool verbose = 0;
bool flipControls = 0;
bool stepByStep = 0;
bool hideWarnings = 0;
unsigned killAddress = 0x10000;
unsigned cyclesPerFrame = 8;

// External variables
extern unsigned colorScheme;
extern Interpreter interpreter;
extern sf::Color foreground;
extern sf::Color background;

// Color 
extern std::string CC, CW, CG, CB, CY, CO, CLO, CGR, CBL, CR;
#define INFO '[' << CGR << "INFO" << CC << "]:"
#define ERROR '[' << CR << "ERROR" << CC << "]:"
#define WARNING '[' << CY << "WARNING" << CC << "]:"

// Header for static functions
static void printMnemonic(void);
static unsigned char getHex(char hex);

// Static variables
static bool foregroundChange = 0;
static bool backgroundChange = 0;

// Help message
#define help \
"    Usage:\n\
        chip8 [-vsmxcfbk] filename\n\
\n\
    Arguments:\n\
        -h          - Show this help\n\
        -v          - Run in verbose mode\n\
        -s          - Run step by step (use space to advance)\n\
        -c value    - Cycles per frame (default: 8)\n\
        -w          - Hide warnings\n\
        -n          - Flip controls\n\
        -x          - Use 8bit color scheme for messages\n\
        -m          - Use monochrome for messages\n\
        -f color    - Set foreground color (HTML format without #)\n\
        -b color    - Set background color (HTML format without #)\n\
        -k address  - Kill address (decimal value)\n\
    \n\
    Examples:\n\
        chip8 test.hex\n\
        chip8 -c 8 -n -f ffffff -b 000000 test.hex\n\
        chip8 -s -v -k 1024 test.hex"

/**
 * @brief This function changes a hex character into a value
 * 
 * @param hex character
 * 
 * @return parsed character value
 */
static unsigned char getHex(char hex)
{
    if (hex >= '0' && hex <= '9') return hex - '0';
    if (hex >= 'A' && hex <= 'F') return hex + 10 - 'A';
    if (hex >= 'a' && hex <= 'f') return hex + 10 - 'a';
    return 0;
}


/**
 * @brief This function parses the arguments given in the command line
 * 
 * @param argc CLI argument count
 * @param argv CLI argument addresses
 * 
 * @return -1 when critical error occured, 0 otherwise
 */
int parse_arguments(int argc, char** argv) {
    // Check all arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];  
         
        // Check for help
        if (arg[0] == '-' && arg[1] == 'h' && arg[2] == '\0') {
            std::cout << help << '\n';
            return -1;

        // Check for verbose flag
        } else if (arg[0] == '-' && arg[1] == 'v' && arg[2] == '\0') {
            verbose = 1;

        // Check for stepByStep flag
        } else if (arg[0] == '-' && arg[1] == 's' && arg[2] == '\0') {
            stepByStep = 1;

        // Check for cycles per frame flag
        } else if (arg[0] == '-' && arg[1] == 'c' && arg[2] == '\0') {
            cyclesPerFrame = 0;

        // Check for hideWarnings flag
        } else if (arg[0] == '-' && arg[1] == 'w' && arg[2] == '\0') {
            hideWarnings = 1;

        // Check for flipControls flag
        } else if (arg[0] == '-' && arg[1] == 'n' && arg[2] == '\0') {
            flipControls = 1;

        // Check for xterm color scheme flag
        } else if (arg[0] == '-' && arg[1] == 'x' && arg[2] == '\0') {
            colorScheme = 1;

        // Check for monochrome color scheme flag
        } else if (arg[0] == '-' && arg[1] == 'm' && arg[2] == '\0') {
            colorScheme = 2;

        // Parse the cycles per frame flag
        } else if (!cyclesPerFrame) {
            if (arg[0] >= '0' && arg[0] <= '9') {
                cyclesPerFrame = stoi(arg);
            } else {
                std::cout << ERROR << " Missing argument for cycles per frame\n";
                return -1;
            }

        // Check for the foreground color
        } else if (arg[0] == '-' && arg[1] == 'f' && arg[2] == '\0') {
            foregroundChange = 1;
            std::cout << "parsing color\n";

        // Parse the foreground color
        } else if (foregroundChange) {
            if (arg.size() == 6) {
                unsigned red   = (getHex(arg[1]) << 4) + getHex(arg[2]);
                unsigned green = (getHex(arg[3]) << 4) + getHex(arg[4]);
                unsigned blue  = (getHex(arg[5]) << 4) + getHex(arg[6]);
                std::cout << "Parsing color" << (int)red << " " << (int)green << " " << (int)blue << '\n';
                foreground = sf::Color(red, green, blue);
                foregroundChange = 0;
            } else {
                std::cout << ERROR << " Missing or wrong argument for foreground color\n";
                return -1;
            }

        // Check for the background color
        } else if (arg[0] == '-' && arg[1] == 'b' && arg[2] == '\0') {
            backgroundChange = 1;

        // Parse the background color
        } else if (backgroundChange) {
            if (arg.size() == 6) {
                unsigned red   = (getHex(arg[1]) << 4) + getHex(arg[2]);
                unsigned green = (getHex(arg[3]) << 4) + getHex(arg[4]);
                unsigned blue  = (getHex(arg[5]) << 4) + getHex(arg[6]);
                background = sf::Color(red, green, blue);
                backgroundChange = 0;
            } else {
                std::cout << ERROR << " Missing or wrong argument for foreground color\n";
                return -1;
            } // TODO: fix colors
        
        // Check for kill address
        } else if (arg[0] == '-' && arg[1] == 'k' && arg[2] == '\0') {
            killAddress = 0;

        // Parse kill address
        } else if (!killAddress) {
            if (arg[0] >= '0' && arg[0] <= '9') {
                killAddress = stoi(arg);
            } else {
                std::cout << ERROR << " Missing argument for kill address\n";
                return -1;
            }
        } else {
            if (arg[0] == '-') {
                std::cout << ERROR << " Unknown argument: " << arg << '\n';
                return -1;
            }
        }
    }

    return 0;
}

/**
 * @brief This function logs a single instruction
 * 
 */
void log()
{
    // Print the mnemonic
    printMnemonic();

    // Print out OPCODE and PC and I addresses
    std::cout << INFO << " OP: " << std::uppercase << std::hex << CR <<
    interpreter.opcode << "h\x1b[0m PC: " << CBL << interpreter.PC-2 << "h\x1b[0m I: " << CBL << (int)interpreter.I << "h\x1b[0m\n"; 
    std::cout << INFO << " DT: " << std::dec << ((interpreter.delayTimer) ? CR : CGR) <<
    (int)interpreter.delayTimer << " \x1b[0mST: " << ((interpreter.soundTimer) ? CR : CGR) <<
    (int)interpreter.soundTimer << "\x1b[0m\n";
    
    // Print out the registers
    std::cout << INFO << " V: ";
    for (unsigned j = 0; j < 4; j++) {
        for (unsigned k = 0; k < 4; k++) {
            std::cout << ((interpreter.V[j*4+k]) ? CO : CY);
            std::cout << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << (int)interpreter.V[j*4+k] << "h "; 
        }
        std::cout << CC << '\n' << INFO << "    ";
    }

    std::cout << '\n';
}

/**
 * @brief This prints the mnemonic and operands
 * 
 */
static void printMnemonic(void) 
{
    using namespace std;

    // Get opcode
    unsigned short opcode = interpreter.opcode;
    unsigned short nibble0 = opcode & 0xF;
    unsigned short nibble1 = (opcode >> 4) & 0xF;
    unsigned short nibble2 = (opcode >> 8) & 0xF;
    
    // Print the log info
    cout << INFO << " Operation: ";

    switch (opcode & 0xF000) {
        case 0x0000:
            if (opcode == 0xE0) {
                cout << CR << "CLS" << CC << '\n';
            } else if (opcode == 0xEE) {
                cout << CR << "RET" << CC << '\n';
            } else {
                cout << CR << "SYS " << uppercase << hex << CBL << nibble2 << nibble1 << nibble0 << 'h' << CC << '\n';
            }
            break;

        case 0x1000:
            cout << CR << "JMP " << uppercase << hex << CBL << nibble2 << nibble1 << nibble0 << 'h' << CC << '\n';
            break;

        case 0x2000:
            cout << CR << "CALL " << uppercase << hex << CBL << nibble2 << nibble1 << nibble0 << 'h' << CC << '\n';
            break;

        case 0x3000:
            cout << CR << "SE " << CLO << 'V' << uppercase << hex << nibble2 << CC << ", ";
            cout << ((nibble1 | nibble0) ? CO : CY) << '#' << uppercase << hex << nibble1 << nibble0 << 'h' << CC << '\n';
            break;

        case 0x4000:
            cout << CR << "SNE " << CLO << 'V' << uppercase << hex << nibble2 << CC << ", ";
            cout << ((nibble1 | nibble0) ? CO : CY) << '#' << uppercase << hex << nibble1 << nibble0 << 'h' << CC << '\n';
            break;

        case 0x5000:
            cout << CR << "SE " << CLO << 'V' << uppercase << hex << nibble2 << CC << ", " << CLO << 'V' << nibble1 << CC << '\n';
            break;

        case 0x6000:
            cout << CR << "LD " << CLO << 'V' << uppercase << hex << nibble2 << CC << ", ";
            cout << ((nibble1 | nibble0) ? CO : CY) << '#' << uppercase << hex << nibble1 << nibble0 << 'h' << CC << '\n';
            break;

        case 0x7000:
            cout << CR << "ADD " << CLO << 'V' << uppercase << hex << nibble2 << CC << ", ";
            cout << ((nibble1 | nibble0) ? CO : CY) << '#' << uppercase << hex << nibble1 << nibble0 << 'h' << CC << '\n';
            break;

        case 0x8000:
            cout << CR;
            switch (nibble0) {
                case 0x0: cout << "LD ";   break;
                case 0x1: cout << "OR ";   break;
                case 0x2: cout << "AND ";  break;
                case 0x3: cout << "XOR ";  break;
                case 0x4: cout << "ADD ";  break;
                case 0x5: cout << "SUB ";  break;
                case 0x6: cout << "SHR ";  break;
                case 0x7: cout << "SUBN "; break;
                case 0xE: cout << "SHL ";  break;
                default:  cout << "--- ";  break;
            }
            cout << CLO << 'V' << uppercase << hex << nibble2 << CC << ", " << CLO << 'V' << nibble1 << CC << '\n';
            break;

        case 0x9000:
            cout << CR << "SNE " << CLO << 'V' << uppercase << hex << nibble2 << CC << ", " << CLO << 'V' << nibble1 << CC << '\n';
            break;

        case 0xA000:
            cout << CR << "LD I" << CC << ", ";
            cout << ((nibble1 | nibble0) ? CO : CY) << '#' << uppercase << hex << nibble2 << nibble1 << nibble0 << 'h' << CC << '\n';
            break;

        case 0xB000:
            cout << CR << "JMP " << CLO << "V0" << CC << " + " << uppercase << hex << CBL << nibble2 << nibble1 << nibble0 << 'h' << CC << '\n';
            break;

        case 0xC000:
            cout << CR << "RND " << CLO << 'V' << uppercase << hex << nibble2 << CC << ", ";
            cout << ((nibble1 | nibble0) ? CO : CY) << '#' << uppercase << hex << nibble1 << nibble0 << 'h' << CC << '\n';
            break;

        case 0xD000:
            cout << CR << "DRW " << CLO << 'V' << uppercase << hex << nibble2 << CC << ", ";
            cout << CLO << 'V' << uppercase << hex << nibble1 << CC << ", ";
            cout << CC << uppercase << hex << nibble0 << CC << '\n';
            break;

        case 0xE000:
            if ((opcode & 0xFF) == 0x9E) {
                cout << CR << "SKP " << CLO << 'V' << uppercase << hex << nibble2 << CC << '\n';
            } else if ((opcode & 0xFF) == 0xA1) {
                cout << CR << "SKP " << CLO << 'V' << uppercase << hex << nibble2 << CC << '\n';
            } else {
                cout << CR << "---" << CC << '\n';
            }
            break;

        case 0xF000:
            switch (opcode & 0xFF) {
                case 0x07:
                    cout << CR << "LD " << CLO << 'V' << uppercase << hex << nibble2 << ", " << CR << "DT" << CC << '\n';
                    break;

                case 0x0A:
                    cout << CR << "LD " << CLO << 'V' << uppercase << hex << nibble2 << ", " << CR << "KBD" << CC << '\n';
                    break;

                case 0x15:
                    cout << CR << "LD DT" << CC << ", " << CLO << 'V' << uppercase << hex << nibble2 << CC << '\n';
                    break;

                case 0x18:
                    cout << CR << "LD ST" << CC << ", " << CLO << 'V' << uppercase << hex << nibble2 << CC << '\n';
                    break;
                    
                case 0x1E:
                    cout << CR << "ADD I" << CC << ", " << CLO << 'V' << uppercase << hex << nibble2 << CC << '\n';
                    break;
                    
                case 0x29:
                    cout << CR << "LD F" << CC << ", " << CLO << 'V' << uppercase << hex << nibble2 << CC << '\n';
                    break;
                
                case 0x33:
                    cout << CR << "LD BCD" << CC << ", " << CLO << 'V' << uppercase << hex << nibble2 << CC << '\n';
                    break;

                case 0x55:
                    cout << CR << "LD [I]" << CC << ", " << CLO << 'V' << uppercase << hex << nibble2 << CC << '\n';
                    break;

                case 0x65:
                    cout << CR << "LD " << CLO << 'V' << uppercase << hex << nibble2 << CC << ", " << CR << "[I]" << CC << '\n';
                    break;

                default:
                    cout << CR << "---" << CC << '\n';
                    break;
            }
            break;
    }
    
}