#include <Morph/Flow.h>
#include <Morph/Room.h>
using namespace morph;

const sf::Int32 Flow::defaultTimeQuantumInMilliseconds = 17;

Flow::Flow(sf::RenderWindow &window, const sf::Time &quant, sf::Color backgroundColor) 
	: timeQuantum(quant), leftover(sf::milliseconds(0)), backColor(backgroundColor) {
	owner = &window;
	currRoom = nextRoom = 0;
	over = false;
}

sf::Vector2i Flow::mapCoordsToViewPixel(const sf::Vector2f &coords) {
	return getWindow()->mapCoordsToPixel(coords);
}

sf::Vector2f Flow::mapViewPixelToCoords(const sf::Vector2i &pixel) {
	return getWindow()->mapPixelToCoords(pixel);
}

sf::Vector2f Flow::mapViewPixelToCoords(const sf::Event::MouseButtonEvent &e) {
	return mapViewPixelToCoords(sf::Vector2i(e.x, e.y));
}

void Flow::changeRoom(Room *r) {
	nextRoom = r;
	if (r) r->owner = this;
}

void Flow::goToNextRoomIfAvailable() {
	if (nextRoom == 0) return;

	std::vector<Object*> persObjs;

	if (currRoom && currRoom->owner == this) {
		currRoom->leavingRoom();
		persObjs = currRoom->getPersistentObjects();
		delete currRoom;
	}
	
	getWindow()->setView(getWindow()->getDefaultView());

	currRoom = nextRoom;
	nextRoom = 0;

	currRoom->owner = this;
	currRoom->putPersistentObjects(persObjs);
	currRoom->onEnter();
}

void Flow::onEvent(const sf::Event &e) {
	goToNextRoomIfAvailable();

	if (currRoom) currRoom->onEvent(e);
}

void Flow::onTick() {
	goToNextRoomIfAvailable();

	if (currRoom) currRoom->onTick();
}

void Flow::onTime() {
	/*
	leftover += clock.restart();
	if (leftover >= timeQuantum)
	{
		leftover -= timeQuantum;
		onTick();
	}
	*/

	for (leftover += clock.restart(); leftover >= timeQuantum; leftover -= timeQuantum)
		onTick();
}

void Flow::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (currRoom) target.draw(*currRoom, states);
}

Flow::~Flow() {
	over = true;
	if (currRoom && currRoom->owner == this) delete currRoom;
}