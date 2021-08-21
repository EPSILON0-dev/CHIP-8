/**
 * INTERPRETER.H
 * 
 * This file contains headers for the functions responsible for interpreting
 *  and executing CHIP-8 instructions.
 * 
 * EPSILON0, 19.08.2021
 */

// Some typedefs to make it easier to read
typedef unsigned char uint8;
typedef unsigned short uint16;

class Interpreter 
{
// Internal VM state
public:
    uint8 V[16];
    uint16 I;
    uint16 PC;
    uint16 callStack[12];
    uint8 SP;
    uint16 opcode;
    uint8 delayTimer = 0;
    uint8 soundTimer = 0;

// Memory array
public:
    uint8 memoryArray[4096];

// Constructor and destructor
public:
    Interpreter(void);
    ~Interpreter(void) {}

// Internal VM functions
private:
    void handleMathOpcode(void);
    void handleMiscOpcode(void);
    void drawSprite(void);
    
// Public functions
public:
    int readProgram(char* filename); 
    int Update(void);
    void timerUpdate(void);
};