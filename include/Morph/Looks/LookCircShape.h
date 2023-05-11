#ifndef _MORPH_LOOK_CIRC_SHAPE_H_
#define _MORPH_LOOK_CIRC_SHAPE_H_

#include <Morph/Look.h>

namespace morph {
	class CollCirc;

	class LookCircShape : public Look {
	public:
		sf::CircleShape shape;

	public:
		explicit LookCircShape(float circleRadius, bool setOriginToCenter = false);

		Look* clone() const { return new LookCircShape(*this); }

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	};
}

#endif