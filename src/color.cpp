/**
 * UTIL.CPP
 * 
 * This file contains escape sequences for the colors and the header color set functon.
 * 
 * EPSILON0, 20.08.2021
 */

#include <iostream>
#include "color.h"

// Color scheme value
unsigned colorScheme = 0;

// Escape sequence strings
std::string CC, CW, CG, CB, CY, CO, CLO, CGR, CBL, CR;

/**
 * @brief This function sets escape sequence strings based on colorScheme value
 * 
 */
void setColorScheme(void)
{
    switch (colorScheme) {
        case colorMonokai: 
            CC  = mn_CC;  CW  = mn_CW;
            CG  = mn_CG;  CB  = mn_CB;
            CY  = mn_CY;  CO  = mn_CO;
            CLO = mn_CLO; CGR = mn_CGR;
            CBL = mn_CBL; CR  = mn_CR;
            break;

        case colorXterm: 
            CC  = x_CC;  CW  = x_CW;
            CG  = x_CG;  CB  = x_CB;
            CY  = x_CY;  CO  = x_CO;
            CLO = x_CLO; CGR = x_CGR;
            CBL = x_CBL; CR  = x_CR;
            break; 

        default: 
            CC  = ""; CW  = "";
            CG  = ""; CB  = "";
            CY  = ""; CO  = "";
            CLO = ""; CGR = "";
            CBL = ""; CR  = "";
            break;
    }
}