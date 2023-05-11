#ifndef _MORPH_OBJ_LOOK_H_
#define _MORPH_OBJ_LOOK_H_

#include <SFML/Graphics.hpp>

namespace morph {
	class CollisionMask;

	class Look : public sf::Drawable, public sf::Transformable {
		friend class Object;

	public:
		// Deep copy. Note that this will copy all the transformations, 
		// including the position. Think about that if copying a look for another object.
		virtual Look* clone() const =0;

		virtual void onTick() {}

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const =0;

		virtual ~Look() {}
	};
}

#endif