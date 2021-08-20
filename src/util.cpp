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
extern unsigned killAddress;
extern Interpreter interpreter;

// Colors
#define CC  "\x1b[0m"                // Clear
#define CW  "\x1b[38;2;248;248;242m" // White
#define CG  "\x1b[38;2;117;113;94m"  // Grey 
#define CB  "\x1b[38;2;39;40;34m"    // Black
#define CY  "\x1b[38;2;230;219;116m" // Yellow
#define CO  "\x1b[38;2;253;151;31m"  // Orange 
#define CL  "\x1b[38;2;230;159;102m" // Light Orange
#define CGR "\x1b[38;2;166;242;46m"  // Green 
#define CBL "\x1b[38;2;102;235;239m" // Blue
#define CR  "\x1b[38;2;249;38;114m"  // Red

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
    std::cout << "[\x1b[38;2;166;242;46mINFO\x1b[0m]: OP: " << std::uppercase << std::hex << CR <<
    interpreter.opcode << "h\x1b[0m PC: " << CBL << interpreter.PC << "h\x1b[0m I: " << CBL << (int)interpreter.I << "h\x1b[0m\n"; 
    std::cout << "\x1b[0m[\x1b[38;2;166;242;46mINFO\x1b[0m]: DT: " << std::dec << ((interpreter.delayTimer) ? CR : CGR) <<
    (int)interpreter.delayTimer << " \x1b[0mST: " << ((interpreter.soundTimer) ? CR : CGR) <<
    (int)interpreter.soundTimer << "\x1b[0m\n";
    
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