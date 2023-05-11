#include <Morph/Looks/LookRectShape.h>
using namespace morph;

void LookRectShape::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Transform combinedTransform = states.transform * getTransform();
	states.transform = combinedTransform;
	target.draw(shape, states);
}