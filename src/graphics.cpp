#include <SFML/Graphics.hpp>
#include <assert.h>
#include "graphics.h"
#include "calc.h"

// Builds general image.
static void
gr_image(sf::Image *image, const sf::Uint8 *bk, const sf::Uint8 *fr,
        void (*calc)(sf::Image *image, int x_tl, int y_tl, 
                const sf::Uint8 *bk_pix, const sf::Uint8 *fr_pix))
{
        assert(image);
        assert(bk && fr);
        
        calc(image, (WINDOW_WIDTH - BACK_WIDTH) / 2, (WINDOW_HEIGHT - BACK_HEIGHT) / 2, bk, fr);
}

void
gr_frame(sf::RenderWindow *window, sf::Image *image, const sf::Uint8 *bk, const sf::Uint8 *fr,
        float fps, sf::Font *font)
{
        assert(window);
        assert(bk && fr);
        assert(font);
        
        void (*calc)(sf::Image *image, int x_tl, int y_tl, 
                const sf::Uint8 *bk_pix, const sf::Uint8 *fr_pix) = &calc_no_avx;
        // Choose function to process image(AVX/no AVX).
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
                calc = &calc_avx_4pix;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
                calc = &calc_avx_8pix;


        gr_image(image, bk, fr, calc);

        // A long transformation of image to window, because
        // SFML works this way.
        sf::Texture texture;
        texture.loadFromImage(*image);
        sf::Sprite sprite;
        sprite.setTexture(texture);

        sf::Text text;
        text.setFont(*font);
        char fps_str[5] = {};
        sprintf(fps_str, "%.0f", fps);
        text.setString(fps_str);

        window->clear();
        window->draw(sprite);
        window->draw(text);
        window->display();
}

