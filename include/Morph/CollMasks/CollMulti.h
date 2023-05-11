#ifndef _MORPH_COLL_MULTI_RECT_H_
#define _MORPH_COLL_MULTI_RECT_H_

#include <Morph/CollMask.h>
#include <vector>

namespace morph {
	class CollMulti : public CollisionMask {
		sf::Vector2f coords, origin;

		void justMove(const sf::Vector2f &offset);

		CollMulti(const CollMulti&);
		void operator=(const CollMulti&);

	public:
		void move(const sf::Vector2f &offset) { justMove(offset); coords += offset; }

		sf::Vector2f getPosition() const { return coords + origin; }

		void setOrigin(float x, float y);
		const sf::Vector2f& getOrigin() const { return origin; }

	public:
		std::vector<CollisionMask*> masks;

		CollMulti() {}
		explicit CollMulti(CollisionMask *m, bool clone);
		explicit CollMulti(std::vector<CollisionMask*> M, bool clone);

		void add(CollisionMask *m, bool clone);

		CollisionMask* clone() const;

		CollType getType() const { return C_MULT; }

		bool containsPoint(const sf::Vector2f &p) const;

		void onTick();

		~CollMulti();
	};
}

#endif