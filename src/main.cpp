#include <stdio.h>
#include "graphics.h"
#include "calc.h"

// Each AV_FRAME make FPS update.
const int AV_FRAME = 500;

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

        for (int i = 0; window.isOpen(); i++) {
                sf::Event event;
                while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed) {
                                window.close();
                        }
                }

                gr_frame(&window, &image, bk_pix, fr_pix, fps, &font);

                if (i % AV_FRAME == 0) {
                        currentTime = clock.getElapsedTime();
                        fps = i / (currentTime.asSeconds() - previousTime.asSeconds());
                        previousTime = currentTime;
                        i = 0;
                }
        }

        return 0;
}
