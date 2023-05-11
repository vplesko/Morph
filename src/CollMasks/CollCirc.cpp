#include <Morph/CollMasks/CollCirc.h>
using namespace morph;

CollCirc::CollCirc(float circleRadius, bool setOriginToCenter) : radius(circleRadius) {
	if (setOriginToCenter)
		origin = sf::Vector2f(circleRadius, circleRadius);
}

void CollCirc::setOrigin(float x, float y) {
	sf::Vector2f offset(x - origin.x, y - origin.y);
	origin += offset;
	coords -= offset;
}

bool CollCirc::containsPoint(const sf::Vector2f &p) const {
	sf::Vector2f center = getCenter();

	if (p.x < center.x - radius ||
		p.x > center.x + radius ||
		p.y < center.y - radius ||
		p.y > center.y + radius)
		return false;

	return (center.x - p.x) * (center.x - p.x) + (center.y - p.y) * (center.y - p.y) < radius * radius;
}