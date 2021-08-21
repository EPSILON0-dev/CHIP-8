/**
 * COLOR.H
 * 
 * This file contains escape sequences for the colors and the header color set functon.
 * 
 * EPSILON0, 20.08.2021
 */

// Enum for color scheme
enum colorSchemeEnum {
    colorMonokai = 0,
    colorXterm,
    colorMono
};

// Color escape sequences for xterm and monokai scheme
#define mn_CC  "\x1b[0m"                   // Clear
#define mn_CW  "\x1b[38;2;248;248;242m"    // White
#define mn_CG  "\x1b[38;2;117;113;94m"     // Grey 
#define mn_CB  "\x1b[38;2;39;40;34m"       // Black
#define mn_CY  "\x1b[38;2;230;219;116m"    // Yellow
#define mn_CO  "\x1b[38;2;253;151;31m"     // Orange 
#define mn_CLO "\x1b[38;2;230;159;102m"    // Light Orange
#define mn_CGR "\x1b[38;2;166;242;46m"     // Green 
#define mn_CBL "\x1b[38;2;102;235;239m"    // Blue
#define mn_CR  "\x1b[38;2;249;38;114m"     // Red
#define x_CC  "\x1b[0m"                    // Clear
#define x_CW  "\x1b[0m"                    // White
#define x_CG  "\x1b[0m"                    // Grey 
#define x_CB  "\x1b[30m"                   // Black
#define x_CY  "\x1b[33m"                   // Yellow
#define x_CO  "\x1b[35m"                   // Orange 
#define x_CLO "\x1b[33m"                   // Light Orange
#define x_CGR "\x1b[32m"                   // Green 
#define x_CBL "\x1b[34m"                   // Blue
#define x_CR  "\x1b[31m"                   // Red

// Function
void setColorScheme(void);

