#include <Morph/CollMasks/CollRect.h>
using namespace morph;

void CollRect::setOrigin(float x, float y) {
	sf::Vector2f offset(x - origin.x, y - origin.y);

	origin += offset;

	r.left -= offset.x;
	r.top -= offset.y;
}