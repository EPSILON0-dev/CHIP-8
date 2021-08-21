/**
 * SCREEN.CPP
 * 
 * This file contains the functions responsible for drawing the screen.
 * 
 * EPSILON0, 20.08.2021
 */

#include <SFML/Graphics.hpp>
#include <iostream>
#include "screen.h" 

unsigned char screenData[2048];
sf::VertexArray screenArray(sf::Quads, 8192);

/**
 * @brief This generates vertex cordinates for each pixel
 * 
 */
void generateScreenArray(void) {
    for (unsigned i = 0; i < 32; i++) {
        for (unsigned j = 0; j < 64; j++) {
            screenArray[(i * 64 + j) * 4 + 0].position = sf::Vector2f(j * 10.0f,         i * 10.0f);
            screenArray[(i * 64 + j) * 4 + 1].position = sf::Vector2f(j * 10.0f + 10.0f, i * 10.0f);
            screenArray[(i * 64 + j) * 4 + 2].position = sf::Vector2f(j * 10.0f + 10.0f, i * 10.0f + 10.0f);
            screenArray[(i * 64 + j) * 4 + 3].position = sf::Vector2f(j * 10.0f,         i * 10.0f + 10.0f);
            sf::Color color = (rand()%2) ? sf::Color::White : sf::Color::Black;
            screenArray[(i * 64 + j) * 4 + 0].color = color;
            screenArray[(i * 64 + j) * 4 + 1].color = color;
            screenArray[(i * 64 + j) * 4 + 2].color = color;
            screenArray[(i * 64 + j) * 4 + 3].color = color;
        }
    }
}