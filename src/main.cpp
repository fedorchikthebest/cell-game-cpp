#include <SFML/Graphics.hpp>
#include "celltable.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 1000), "SFML works!");
    sf::CircleShape shape(100.f);

	CellTable celltable(&window);
	window.setFramerateLimit(100);
	celltable.rand_cells();
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(0,0,0));
		celltable.update();
		celltable.draw();
        window.display();
    }

    return 0;
}
