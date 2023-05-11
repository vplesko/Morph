#include <SFML/Graphics.hpp>
#include <Morph/Flow.h>
#include "YourStartGameRoom.h"

int main() {
	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Window", sf::Style::None);
	//sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML Fullscreen Window", sf::Style::Fullscreen);
    //window.setFramerateLimit(60);
	
	morph::Flow game(window);
	game.changeRoom(new YourStartGameRoom());

    while (window.isOpen()) {
        sf::Event polledEvent;
        while (window.pollEvent(polledEvent)) {
            if (polledEvent.type == sf::Event::Closed)
                window.close();
			else
				game.onEvent(polledEvent);
        }

		game.onTime();

		window.clear(game.getBackgroundColor());
        window.draw(game);
        window.display();

		if (game.isEnded() && window.isOpen())
			window.close();
    }

    return 0;
}