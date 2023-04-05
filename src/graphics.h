#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SFML/Graphics.hpp>

// Draws single frame.
void
gr_frame(sf::RenderWindow *window, sf::Image *image, const sf::Uint8 *bk, const sf::Uint8 *fr,
        float fps, sf::Font *font);

#endif // GRAPHICS_H

