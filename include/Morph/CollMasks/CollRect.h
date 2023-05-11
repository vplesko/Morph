#ifndef _MORPH_COLL_RECT_H_
#define _MORPH_COLL_RECT_H_

#include <Morph/CollMask.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace morph {
	class CollRect : public CollisionMask {
		sf::Rect<float> r;
		sf::Vector2f origin;

	public:
		void move(const sf::Vector2f &offset) { r.left += offset.x; r.top += offset.y; }

		sf::Vector2f getPosition() const { return sf::Vector2f(r.left + origin.x, r.top + origin.y); }

		void setOrigin(float x, float y);
		const sf::Vector2f& getOrigin() const { return origin; }

	public:
		CollRect(const sf::Vector2f &size) : r(0, 0, size.x, size.y) {}
		CollRect(float rectWidth, float rectHeight) : r(0, 0, rectWidth, rectHeight) {}

		CollisionMask* clone() const { return new CollRect(*this); }

		CollType getType() const { return C_RECT; }

		bool containsPoint(const sf::Vector2f &p) const { return r.contains(p); }

		const sf::Rect<float>& getRect() const { return r; }

		float getCenterX() const { return r.left + r.width / 2; }
		float getCenterY() const { return r.left + r.height / 2; }
		sf::Vector2f getCenter() const { return sf::Vector2f(getCenterX(), getCenterY()); }
	};
}

#endif