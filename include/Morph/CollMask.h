#ifndef _MORPH_COLL_MASK_H_
#define _MORPH_COLL_MASK_H_

#include <Morph/Collisions.h>
#include <SFML/System/Vector2.hpp>

namespace morph {
	class CollisionMask {
		friend class Object;
		friend class Room;
		friend class Look;
		friend class CollMulti;

	public:
		virtual void move(const sf::Vector2f &offset) =0;
		virtual sf::Vector2f getPosition() const =0;
		
		virtual void setOrigin(float x, float y) =0;
		void setOrigin(const sf::Vector2f &origin) { setOrigin(origin.x, origin.y); }

		virtual const sf::Vector2f& getOrigin() const =0;

		// Deep copy.
		virtual CollisionMask* clone() const =0;

		virtual CollType getType() const =0;

		virtual bool containsPoint(const sf::Vector2f &p) const =0;
		bool containsPoint(float x, float y) const { return containsPoint(sf::Vector2f(x, y)); }

		virtual void onTick() {}

		bool collidesWith(CollisionMask *m) { return areColliding(this, m); }

		virtual ~CollisionMask() {}

	public:
		static bool canCollide(ObjType t1, ObjType t2);
		static bool areColliding(const CollisionMask *m1, const CollisionMask *m2);
	};
}

#endif