#include <stdio.h>
#include <immintrin.h>
#include <assert.h>
#include "calc.h"

void
calc_no_avx(sf::Image *image, int x_tl, int y_tl, const sf::Uint8 *bk_pix, const sf::Uint8 *fr_pix)
{
        assert(image);
        assert(bk_pix);
        assert(fr_pix);

        for (int y = y_tl; y < (WINDOW_HEIGHT + BACK_HEIGHT) / 2 - 1; y++) {
                for (int x = x_tl; x < (WINDOW_WIDTH + BACK_WIDTH) / 2; x++) {
                        sf::Color *fr_c = (sf::Color *) &fr_pix[((y - y_tl) * (int) BACK_WIDTH + (x - x_tl)) * 4];
                        sf::Color *bk_c = (sf::Color *) &bk_pix[(y * (int) WINDOW_WIDTH + x) * 4];
                        sf::Color color;

                        color.r = (bk_c->r * (255 - fr_c->a) + fr_c->r * fr_c->a) >> 8;
                        color.g = (bk_c->g * (255 - fr_c->a) + fr_c->g * fr_c->a) >> 8;
                        color.b = (bk_c->b * (255 - fr_c->a) + fr_c->b * fr_c->a) >> 8;
                        color.a = (bk_c->a * (255 - fr_c->a) + fr_c->a * fr_c->a) >> 8;

                        image->setPixel(x, y, color);
                }
        }
}
