/**
 * UTIL.CPP
 * 
 * This file contains headers for all of the miscellaneous functions.
 * 
 * EPSILON0, 20.08.2021
 */

#include <iomanip>
#include <iostream>
#include "interpreter.h"
#include "util.h"

// External variables
extern bool verbose;
extern bool stepByStep;
extern unsigned killAddress;
extern Interpreter interpreter;

// Colors
#define CC  "\x1b[0m"                   // Clear
#define CW  "\x1b[38;2;248;248;242m"    // White
#define CG  "\x1b[38;2;117;113;94m"     // Grey 
#define CB  "\x1b[38;2;39;40;34m"       // Black
#define CY  "\x1b[38;2;230;219;116m"    // Yellow
#define CO  "\x1b[38;2;253;151;31m"     // Orange 
#define CLO  "\x1b[38;2;230;159;102m"   // Light Orange
#define CGR "\x1b[38;2;166;242;46m"     // Green 
#define CBL "\x1b[38;2;102;235;239m"    // Blue
#define CR  "\x1b[38;2;249;38;114m"     // Red

// Header for static mnemonic function
static void printMnemonic(void);

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
    for (int i = 1; i < argc-1; i++) {
        std::string arg = argv[i];   

        // Check for verbose flag
        if (arg[0] == '-' && arg[1] == 'v' && arg[2] == '\0') {
            verbose = 1;

        // Check for stepByStep flag
        } else if (arg[0] == '-' && arg[1] == 's' && arg[2] == '\0') {
            stepByStep = 1;
        
        // Check for kill address
        } else if (arg[0] == '-' && arg[1] == 'k' && arg[2] == '\0') {
            killAddress = 0;

        // Parse kill address
        } else if (!killAddress) {
            if (arg[0] >= '0' && arg[0] <= '9') {
                killAddress = stoi(arg);
            } else {
                std::cout << "[\x1b[38;2;249;38;114mERROR\x1b[0m]: Missing argument\n";
                std::cout << "[\x1b[38;2;166;242;46mINFO\x1b[0m]: Usage: chip8 [-v] [-k kill address (dec)] filename\n";
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
    std::cout << "[\x1b[38;2;166;242;46mINFO\x1b[0m]: OP: " << std::uppercase << std::hex << CR <<
    interpreter.opcode << "h\x1b[0m PC: " << CBL << interpreter.PC-2 << "h\x1b[0m I: " << CBL << (int)interpreter.I << "h\x1b[0m\n"; 
    std::cout << "\x1b[0m[\x1b[38;2;166;242;46mINFO\x1b[0m]: DT: " << std::dec << ((interpreter.delayTimer) ? CR : CGR) <<
    (int)interpreter.delayTimer << " \x1b[0mST: " << ((interpreter.soundTimer) ? CR : CGR) <<
    (int)interpreter.soundTimer << "\x1b[0m\n";
    
    // Print out the registers
    std::cout << "\x1b[0m[\x1b[38;2;166;242;46mINFO\x1b[0m]: V: ";
    for (unsigned j = 0; j < 4; j++) {
        for (unsigned k = 0; k < 4; k++) {
            std::cout << ((interpreter.V[j*4+k]) ? CO : CY);
            std::cout << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << (int)interpreter.V[j*4+k] << "h "; 
        }
        std::cout << "\n\x1b[0m[\x1b[38;2;166;242;46mINFO\x1b[0m]:    ";
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
    cout << "[\x1b[38;2;166;242;46mINFO\x1b[0m]: ";
    cout << "Operation: ";

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