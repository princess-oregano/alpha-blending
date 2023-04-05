#include <SFML/Graphics.hpp>
#include <assert.h>
#include "graphics.h"
#include "calc.h"

// Builds general image.
static void
gr_image(sf::Image *image, const sf::Uint8 *bk, const sf::Uint8 *fr)
{
        assert(image);
        assert(bk && fr);
        
        calc_no_avx(image, (WINDOW_WIDTH - BACK_WIDTH) / 2, (WINDOW_HEIGHT - BACK_HEIGHT) / 2, bk, fr);
}

void
gr_frame(sf::RenderWindow *window, sf::Image *image, const sf::Uint8 *bk, const sf::Uint8 *fr,
        float fps, sf::Font *font)
{
        assert(window);
        assert(bk && fr);
        assert(font);

        gr_image(image, bk, fr);

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

