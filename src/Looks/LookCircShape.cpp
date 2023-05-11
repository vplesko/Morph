#include <Morph/Looks/LookCircShape.h>
using namespace morph;

LookCircShape::LookCircShape(float circleRadius, bool setOriginToCenter) : shape(circleRadius) {
	if (setOriginToCenter)
		shape.setOrigin(circleRadius, circleRadius);
}

void LookCircShape::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Transform combinedTransform = states.transform * getTransform();
	states.transform = combinedTransform;
	target.draw(shape, states);
}