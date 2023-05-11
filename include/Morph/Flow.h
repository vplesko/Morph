#ifndef _MORPH_FLOW_H_
#define _MORPH_FLOW_H_

#include <SFML/Graphics.hpp>

namespace morph {
	class Room;
	
	class Flow : public sf::Drawable, public sf::Transformable {
		//no copying
		Flow(const Flow&);
		void operator=(const Flow&);

		sf::RenderWindow *owner;

		Room *currRoom, *nextRoom;
		void goToNextRoomIfAvailable();

		sf::Clock clock;
		sf::Time timeQuantum, leftover;

		void onTick();

		sf::Color backColor;

		bool over;

	public:
		static const sf::Int32 defaultTimeQuantumInMilliseconds;

	public:
		Flow(sf::RenderWindow &window, 
			const sf::Time &quant = sf::milliseconds(defaultTimeQuantumInMilliseconds), 
			sf::Color backgroundColor = sf::Color(0, 0, 0, 255));

		sf::RenderWindow* getWindow() const { return owner; }

		sf::Color getBackgroundColor() const { return backColor; }
		void setBackgroundColor(const sf::Color &color) { backColor = color; }
		sf::Time getTimeQuantum() const { return timeQuantum; }
		
		sf::Vector2i mapCoordsToViewPixel(const sf::Vector2f &coords);
		sf::Vector2f mapViewPixelToCoords(const sf::Vector2i &pixel);
		sf::Vector2f mapViewPixelToCoords(const sf::Event::MouseButtonEvent &e);

		void changeRoom(Room *r);

		void onEvent(const sf::Event &e);
		void onTime();
		void setRate(sf::Time quant) { timeQuantum = quant; }
		sf::Time getRate() const { return timeQuantum; }
		unsigned getRatePerSec() const { return 1000 / timeQuantum.asMilliseconds(); }
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		void endGame() { over = true; }
		bool isEnded() const { return over; }

		~Flow();
	};
}

#endif