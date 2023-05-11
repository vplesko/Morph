#ifndef _MORPH_COLL_CIRC_H_
#define _MORPH_COLL_CIRC_H_

#include <Morph/CollMask.h>
#include <SFML/System/Vector2.hpp>

namespace morph {
	class CollCirc : public CollisionMask {
		sf::Vector2f coords;
		float radius;
		sf::Vector2f origin;

	protected:
		friend class LookCircShape;

	public:
		void move(const sf::Vector2f &offset) { coords.x += offset.x; coords.y += offset.y; }
		
		sf::Vector2f getPosition() const { return sf::Vector2f(coords.x + origin.x, coords.y + origin.y); }

		void setOrigin(float x, float y);
		const sf::Vector2f& getOrigin() const { return origin; }

	public:
		explicit CollCirc(float circleRadius, bool setOriginToCenter = false);

		CollisionMask* clone() const { return new CollCirc(*this); }

		CollType getType() const { return C_CIRC; }

		bool containsPoint(const sf::Vector2f &p) const;

		sf::Vector2f getCenter() const { return coords + sf::Vector2f(radius, radius); }

		float getRadius() const { return radius; }
	};
}

#endif