#ifndef CALC_H
#define CALC_H

#include <SFML/Graphics.hpp>

// Window and background sizes.
const float WINDOW_WIDTH  = 800;
const float WINDOW_HEIGHT = 600;
// Front image sizes.
const float BACK_WIDTH = 236;
const float BACK_HEIGHT = 126;
// Zero byte for shuffle masks.
const int ZERO_BYTE = 0x80;

// Function to compute color of overlaying images.
// Accepts pointers to tl of overlaying region.
void
calc_no_avx(sf::Image *image, int x_tl, int y_tl, const sf::Uint8 *bk_pix, const sf::Uint8 *fr_pix);
// Same as calc_no_avx(), but using AVX/AVX2 optimization.
void
calc_avx_4pix(sf::Image *image, int x_tl, int y_tl, const sf::Uint8 *bk_pix, const sf::Uint8 *fr_pix);
// Enhanced version of calc_avx_4pix().
void
calc_avx_8pix(sf::Image *image, int x_tl, int y_tl, const sf::Uint8 *bk_pix, const sf::Uint8 *fr_pix);
#endif // CALC_H

