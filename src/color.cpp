#include <iostream>
#include "color.h"

unsigned colorScheme = 0;

std::string CC, CW, CG, CB, CY, CO, CLO, CGR, CBL, CR;

void setColorScheme(void)
{
    switch (colorScheme) {
        case colorMonokai: colorSetMonokai(); break;
        case colorXterm: colorSetXterm(); break;
        default: colorSetMono(); break;
    }
}

void colorSetMonokai(void) 
{
    CC  = mn_CC;  CW  = mn_CW;
    CG  = mn_CG;  CB  = mn_CB;
    CY  = mn_CY;  CO  = mn_CO;
    CLO = mn_CLO; CGR = mn_CGR;
    CBL = mn_CBL; CR  = mn_CR;
}

void colorSetXterm(void)
{
    CC  = x_CC;  CW  = x_CW;
    CG  = x_CG;  CB  = x_CB;
    CY  = x_CY;  CO  = x_CO;
    CLO = x_CLO; CGR = x_CGR;
    CBL = x_CBL; CR  = x_CR;
}

void colorSetMono(void)
{
    CC  = "";
    CW  = "";
    CG  = "";
    CB  = "";
    CY  = "";
    CO  = "";
    CLO = "";
    CGR = "";
    CBL = "";
    CR  = "";
}
