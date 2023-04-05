#include <stdio.h>
#include "graphics.h"
#include "calc.h"

int
main()
{
        sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                                "Alphablending");

        float fps = 0;
        sf::Clock clock;
        sf::Time previousTime = clock.getElapsedTime();
        sf::Time currentTime;

        sf::Font font;
        font.loadFromFile("IBMPlexMono-Regular.ttf");

        // Open images; store pixels in arrays.
        sf::Image back;
        back.loadFromFile("Table.bmp");
        const sf::Uint8 *bk_pix = back.getPixelsPtr(); 	
        sf::Image front;
        front.loadFromFile("AskhatCat.png");
        const sf::Uint8 *fr_pix = front.getPixelsPtr(); 	

        sf::Image image;
        image.loadFromFile("Table.bmp");

        while (window.isOpen()) {
                sf::Event event;
                while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed) {
                                window.close();
                        }
                }

                // Choose function to process image(AVX/no AVX).
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
                        ;

                gr_frame(&window, &image, bk_pix, fr_pix, fps, &font);

                currentTime = clock.getElapsedTime();
                fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds());
                previousTime = currentTime;
        }

        return 0;
}
