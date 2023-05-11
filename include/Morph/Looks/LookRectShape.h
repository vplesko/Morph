#ifndef _MORPH_LOOK_RECT_SHAPE_H_
#define _MORPH_LOOK_RECT_SHAPE_H_

#include <Morph/Look.h>

namespace morph {
	class LookRectShape : public Look {
	public:
		sf::RectangleShape shape;

	public:
		explicit LookRectShape(const sf::Vector2f &size) : shape(size) {}

		Look* clone() const { return new LookRectShape(*this); }

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	};
}

#endif